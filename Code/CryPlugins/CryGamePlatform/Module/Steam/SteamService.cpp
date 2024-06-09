// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "SteamService.h"
#include "IGamePlatform.h"

#include "SteamUserIdentifier.h"

#include <CrySystem/ICmdLine.h>
#include <CrySystem/ConsoleRegistration.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			extern "C" void __cdecl SteamAPIDebugTextHook(int severity, const char* szDebugText)
			{
				// While running under a debugger, only warnings (severity >= 1) will be sent
				// Adding -debug_steamapi to the command-line will send a lot of extra informational messages
				CryWarning(VALIDATOR_MODULE_SYSTEM, severity > 0 ? VALIDATOR_WARNING : VALIDATOR_COMMENT, "[SteamAPI] %s", szDebugText);
			}

			CService::CService()
				: m_callbackGameOverlayActivated(this, &CService::OnGameOverlayActivated)
				, m_onAvatarImageLoadedCallback(this, &CService::OnAvatarImageLoaded)
				, m_callbackGetSteamAuthTicketResponse(this, &CService::OnGetSteamAuthTicketResponse)
				, m_callbackOnPersonaChange(this, &CService::OnPersonaStateChange)
				, m_callbackOnMicroTransactionResponse(this, &CService::OnMicroTransactionAuthorizationResponse)
				, m_callbackOnLicensesUpdated(this, &CService::OnLicensesUpdated)
				, m_pServer(nullptr)
			{
			}

			bool CService::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
			{
				int steam_editor_support;
				REGISTER_CVAR(steam_editor_support, 0, VF_REQUIRE_APP_RESTART, "Defines whether the Steam integration is enabled in Sandbox. Requires application restart.");
				gEnv->pConsole->UnregisterVariable("steam_editor_support");

				if (env.IsEditor() && steam_editor_support == 0)
				{
					// Don't use Steam integration in Sandbox
					return true;
				}

				int steam_appId;
				REGISTER_CVAR(steam_appId, 0, VF_REQUIRE_APP_RESTART, "Override Steam application id, only possible during development! Requires application restart.");
				gEnv->pConsole->UnregisterVariable("steam_appId");

#if !defined(RELEASE)
				char buff[MAX_PATH];
				CryGetExecutableFolder(CRY_ARRAY_COUNT(buff), buff);
				const string appIdPath = PathUtil::Make(buff, "steam_appid", "txt");

				FILE* const pSteamAppID = fopen(appIdPath.c_str(), "wt");
				fprintf(pSteamAppID, "%d", steam_appId);
				fclose(pSteamAppID);
#endif // !defined(RELEASE)

				const bool isInitialized = SteamAPI_Init();

#if defined(RELEASE)
				// Validate that the application was started through Steam,
				// If this fails we should quit the application
				if (SteamAPI_RestartAppIfNecessary(steam_appId))
				{
					gEnv->pSystem->Quit();
					return true;
				}
#else
				// Remove the file, no longer needed
				remove(appIdPath.c_str());
#endif

				if (!isInitialized)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] SteamApi_Init failed");

#ifdef RELEASE
					return false;
#else
					// Allow silent failure (barring the error message above) in development mode
					// Otherwise we wouldn't be able to start the engine without Steam running
					return true;
#endif
				}

				EnableUpdate(Cry::IEnginePlugin::EUpdateStep::MainUpdate, true);

				m_pStatistics = stl::make_unique<CStatistics>(*this);
				m_pSteamLeaderboards = stl::make_unique<CLeaderboards>(*this);

				m_pMatchmaking = stl::make_unique<CMatchmaking>(*this);
				m_pNetworking = stl::make_unique<CNetworking>();

				m_pRemoteStorage = stl::make_unique<CRemoteStorage>(*this);

				if (ISteamClient* pClient = SteamClient())
				{
					pClient->SetWarningMessageHook(&SteamAPIDebugTextHook);
				}

				ISteamApps* pSteamApps = SteamApps();
				if (!pSteamApps)
				{
					return false;
				}
				char betaName[256];
				if (pSteamApps->GetCurrentBetaName(betaName, 256))
				{
					CryLogAlways("[Steam] Running Steam game beta %s", betaName);
				}

				ISteamUser* pSteamUser = SteamUser();
				if (!pSteamUser)
				{
					return false;
				}

				CryLogAlways("[Steam] Successfully initialized Steam API, user_id=%" PRIu64 " build_id=%i", pSteamUser->GetSteamID().ConvertToUint64(), pSteamApps->GetAppBuildId());

				if (ISteamUtils* pUtils = SteamUtils())
				{
					const bool canFilter = pUtils->InitFilterText();
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_COMMENT, "[GamePlatform] Steam text filtering is : %s", canFilter ? "initialized" : "unavailable");
				}

				string environmentFormatString;

				environmentFormatString.Format("%d", steam_appId);
				SetEnvironmentValue("ApplicationId", environmentFormatString);
				
				environmentFormatString.Format("%d", pSteamApps->GetAppBuildId());
				SetEnvironmentValue("BuildId", environmentFormatString);

				if (strlen(betaName) > 0)
				{
					SetEnvironmentValue("NetworkEnvironment", betaName);
				}

				if (Cry::GamePlatform::IPlugin* pPlugin = gEnv->pSystem->GetIPluginManager()->QueryPlugin<Cry::GamePlatform::IPlugin>())
				{
					pPlugin->RegisterMainService(*this);
				}

				// Check if user requested to join a lobby right away
				if (const ICmdLine* pCmdLine = gEnv->pSystem->GetICmdLine())
				{
					if (const ICmdLineArg* pCmdArg = pCmdLine->FindArg(eCLAT_Post, "connect_lobby"))
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] Received pre-launch join request, loading lobby");
						CSteamID lobbyId = CSteamID(static_cast<uint64>(std::atoll(pCmdArg->GetValue())));
						if (lobbyId.IsValid())
						{
							m_pMatchmaking->JoinLobby(CreateLobbyIdentifier(lobbyId));
						}
					}
				}

				// Set localization according to the settings made in Steam
				InitializeLocalizationLanguage();

				// Query DLC ownership information
				UpdateLicenses(false);

				// NOTE: Calling before CAccountManager::AddListener() to not trigger notifications
				UpdateFriends();
				UpdateBlocked();

				m_accountMgr.AddListener(*this, GetName());

				return true;
			}

			void CService::SetEnvironmentValue(const char* szVarName, const string& value)
			{
				const char* szValue = nullptr;
				bool anythingChanged = false;

				if (!value.IsEmpty())
				{
					auto currentValueIt = m_environment.find(szVarName);
					if (currentValueIt != m_environment.end())
					{
						anythingChanged = currentValueIt->second != value;
					}
					else
					{
						anythingChanged = true;
					}
					m_environment[szVarName] = value;
					szValue = value.c_str();
				}
				else
				{
					anythingChanged = m_environment.erase(szVarName) > 0;
				}

				if (anythingChanged)
				{
					m_listeners.ForEach([&](IListener* pListener)
					{
						pListener->OnEnvironmentVariableChanged(szVarName, szValue);
					});
				}
			}

			void CService::InitializeLocalizationLanguage()
			{
				if (ISteamApps* const pSteamApps = SteamApps())
				{
					// Map Steam API language codes to engine
					// Full list of Steam API language codes can be found here: https://partner.steamgames.com/doc/store/localization
					std::map<string, ILocalizationManager::EPlatformIndependentLanguageID> translationMappings;
					translationMappings["arabic"] = ILocalizationManager::ePILID_Arabic;
					translationMappings["schinese"] = ILocalizationManager::ePILID_ChineseS;
					translationMappings["tchinese"] = ILocalizationManager::ePILID_ChineseT;
					translationMappings["czech"] = ILocalizationManager::ePILID_Czech;
					translationMappings["danish"] = ILocalizationManager::ePILID_Danish;
					translationMappings["dutch"] = ILocalizationManager::ePILID_Dutch;
					translationMappings["english"] = ILocalizationManager::ePILID_English;
					translationMappings["finnish"] = ILocalizationManager::ePILID_Finnish;
					translationMappings["french"] = ILocalizationManager::ePILID_French;
					translationMappings["german"] = ILocalizationManager::ePILID_German;
					translationMappings["italian"] = ILocalizationManager::ePILID_Italian;
					translationMappings["japanese"] = ILocalizationManager::ePILID_Japanese;
					translationMappings["koreana"] = ILocalizationManager::ePILID_Korean;
					translationMappings["norwegian"] = ILocalizationManager::ePILID_Norwegian;
					translationMappings["polish"] = ILocalizationManager::ePILID_Polish;
					translationMappings["portuguese"] = ILocalizationManager::ePILID_Portuguese;
					translationMappings["russian"] = ILocalizationManager::ePILID_Russian;
					translationMappings["spanish"] = ILocalizationManager::ePILID_Spanish;
					translationMappings["swedish"] = ILocalizationManager::ePILID_Swedish;
					translationMappings["turkish"] = ILocalizationManager::ePILID_Turkish;

					const char* szDesiredLanguage = pSteamApps->GetCurrentGameLanguage();
					ILocalizationManager::EPlatformIndependentLanguageID languageId = ILocalizationManager::ePILID_English; // Default to english if we the language is not supported
					const auto remapIt = translationMappings.find(szDesiredLanguage);
					if (remapIt != translationMappings.end())
					{
						languageId = remapIt->second;
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[GamePlatform] Language mapping missing for %s", szDesiredLanguage);
					}

					UpdateLanguages(languageId);
				}
			}

			void CService::UpdateLanguages(ILocalizationManager::EPlatformIndependentLanguageID languageId)
			{
				// Only use localization info if we are the main service
				if (const IPlugin* pPlugin = gEnv->pSystem->GetIPluginManager()->QueryPlugin<IPlugin>())
				{
					if (const IService* pMainService = pPlugin->GetMainService())
					{
						if (pMainService != this)
							return;
					}
				}

				CRY_ASSERT(languageId != ILocalizationManager::ePILID_MAX_OR_INVALID, "Invalid language ID");
				ILocalizationManager* pLocalizationMgr = gEnv->pSystem->GetLocalizationManager();
				if (pLocalizationMgr && languageId != ILocalizationManager::ePILID_MAX_OR_INVALID)
				{
					const char* szLanguage = pLocalizationMgr->LangNameFromPILID(languageId);
					CRY_ASSERT(szLanguage != nullptr && szLanguage[0] != '\0');
					CryLogAlways("Attempting to automatically set user preferred language %s", szLanguage);
					ICVar* pVar = gEnv->pConsole->GetCVar("g_language");
					if (pVar != nullptr)
					{
						// Don't override the language if it was loaded from a config file.
						// We assume that it was set via an ingame-menu in that case.
						if ((pVar->GetFlags() & VF_WASINCONFIG) == 0)
						{
							pVar->Set(szLanguage);
						}
					}

					pVar = gEnv->pConsole->GetCVar("g_languageAudio");
					if (pVar != nullptr)
					{
						// Don't override the language if it was loaded from a config file.
						// We assume that it was set via an ingame-menu in that case.
						if ((pVar->GetFlags() & VF_WASINCONFIG) == 0)
						{
							pVar->Set(szLanguage);
						}
					}
				}
			}

			void CService::MainUpdate(float frameTime)
			{
				CRY_PROFILE_FUNCTION(PROFILE_SYSTEM);

				SteamAPI_RunCallbacks();
			}

			void CService::Shutdown()
			{
				if (ISteamClient* pClient = SteamClient())
				{
					pClient->SetWarningMessageHook(nullptr);
				}

				m_pServer.reset();
				m_pRemoteStorage.reset();
				m_pNetworking.reset();
				m_pMatchmaking.reset();
				m_pSteamLeaderboards.reset();
				m_pStatistics.reset();
				m_accountMgr.Clear();

				m_listeners.ForEach([](IListener* pListener) {
					pListener->OnShutdown(SteamServiceID);
				});

				SteamAPI_Shutdown();
			}

			ServiceIdentifier CService::GetServiceIdentifier() const
			{
				return SteamServiceID;
			}

			int CService::GetBuildIdentifier() const
			{
				if (ISteamApps* pSteamApps = SteamApps())
				{
					return pSteamApps->GetAppBuildId();
				}
				return 0;
			}

			bool CService::SetIdentifierFromString(AccountIdentifier& out, const string& in) const
			{
				AccountIdentifier(*pF)(AccountIdentifierValue) = &CreateAccountIdentifier;
				return CreateIdentifierFromString(pF, in, out, AccountIdentifierValue{});
			}

			bool CService::SetIdentifierFromString(LobbyIdentifier& out, const string& in) const
			{
				LobbyIdentifier(*pF)(LobbyIdentifierValue) = &CreateLobbyIdentifier;
				return CreateIdentifierFromString(pF, in, out, LobbyIdentifierValue{});
			}

			bool CService::SetIdentifierFromString(ApplicationIdentifier& out, const string& in) const
			{
				return CreateIdentifierFromString(&CreateApplicationIdentifier, in, out, ApplicationIdentifierValue{});
			}

			bool CService::SetIdentifierFromString(InventoryItemIdentifier& out, const string& in) const
			{
				return CreateIdentifierFromString(&CreateInventoryItemIdentifier, in, out, InventoryItemIdentifierValue{});
			}

			bool CService::SetIdentifierFromString(LeaderboardIdentifier& out, const string& in) const
			{
				return CreateIdentifierFromString(&CreateLeaderboardIdentifier, in, out, LeaderboardIdentifierValue{});
			}

			CAccount* CService::GetLocalAccount() const
			{
				if (ISteamUser* pSteamUser = SteamUser())
				{
					return TryGetAccount(pSteamUser->GetSteamID());
				}

				return nullptr;
			}

			void CService::OnGameOverlayActivated(GameOverlayActivated_t* pData)
			{
				// Upload statistics added during gameplay
				m_pStatistics->Upload();

				const bool isActive = (pData->m_bActive != 0);
				m_listeners.ForEach([isActive](IListener* pListener) {
					pListener->OnOverlayActivated(SteamServiceID, isActive);
				});
			}

			void CService::OnAvatarImageLoaded(AvatarImageLoaded_t* pCallback)
			{
				const AccountIdentifier accountId = CreateAccountIdentifier(pCallback->m_steamID);
				m_listeners.ForEach([&accountId](IListener* pListener) {
					pListener->OnAvatarImageLoaded(accountId);
				});
			}

			void CService::OnGetSteamAuthTicketResponse(GetAuthSessionTicketResponse_t* pData)
			{
				const HAuthTicket authTicket = pData->m_hAuthTicket;
				const auto it = m_pendingAuthTicketHandles.find(authTicket);
				if (it != std::end(m_pendingAuthTicketHandles))
				{
					const bool success = pData->m_eResult == EResult::k_EResultOK;
					m_listeners.ForEach([authTicket, success](IListener* pListener) {
						pListener->OnGetSteamAuthTicketResponse(success, authTicket);
					});
					m_pendingAuthTicketHandles.erase(it);
				}
				else
				{
					CryLog("[Steam] Ignoring auth token %u as it was not requested by GamePlatform", authTicket);
				}
			}

			void CService::OnPersonaStateChange(PersonaStateChange_t* pData)
			{
				const CSteamID steamId = pData->m_ulSteamID;
				const AccountIdentifier accountId = CreateAccountIdentifier(steamId);
				
				CAccount* pAccount = m_accountMgr.FindAccount(accountId);
				
				if (pData->m_nChangeFlags & k_EPersonaChangeGoneOffline)
				{
					if (pAccount)
					{
						RemoveBlocked(pAccount);
						m_accountMgr.RemoveAccount(accountId);
					}

					return; // skip further processing, to not add account again
				}

				if (pData->m_nChangeFlags & k_EPersonaChangeRelationshipChanged)
				{
					::EFriendRelationship relationship = k_EFriendRelationshipNone;
					if (ISteamFriends* pFriends = SteamFriends())
					{
						relationship = pFriends->GetFriendRelationship(steamId);
					}

					if (relationship == k_EFriendRelationshipFriend)
					{
						if (pAccount)
						{
							RemoveBlocked(pAccount);
							m_accountMgr.AddFriend(*pAccount);
						}
						else
						{
							pAccount = m_accountMgr.AddFriend(accountId, steamId);
						}
					}
					else if (relationship == k_EFriendRelationshipBlocked || relationship == k_EFriendRelationshipIgnored || relationship == k_EFriendRelationshipIgnoredFriend)
					{
						m_accountMgr.RemoveFriend(accountId);

						if (!pAccount)
						{
							pAccount = TryGetAccount(steamId);
						}

						AddBlocked(*pAccount);
					}
				}

				if (pAccount == nullptr)
				{
					pAccount = TryGetAccount(steamId);
				}

				const CEnumFlags<EPersonaChangeFlags> flags = ToPersonaChangeEnumFlags(pData->m_nChangeFlags);
				m_listeners.ForEach([pAccount, flags](IListener* pListener) {
						pListener->OnPersonaStateChanged(*pAccount, flags);
					});
			}

			void CService::OnMicroTransactionAuthorizationResponse(MicroTxnAuthorizationResponse_t* pData)
			{
				const bool authorized = (pData->m_bAuthorized != 0);
				TransactionIdentifier transactionId(SteamServiceID, pData->m_ulOrderID);
				m_listeners.ForEach([authorized, &transactionId](IListener* pListener) {
					pListener->OnMicroTransactionAuthorizationResponse(transactionId, authorized);
				});
			}

			void CService::OnLicensesUpdated(LicensesUpdated_t* pData)
			{
				UpdateLicenses(true);
			}

			bool CService::OwnsApplication(ApplicationIdentifier id) const
			{
				if (ISteamApps* pSteamApps = SteamApps())
				{
					return pSteamApps->BIsSubscribedApp(ExtractSteamID(id));
				}

				return false;
			}

			ApplicationIdentifier CService::GetApplicationIdentifier() const
			{
				AppId_t rawValue = k_uAppIdInvalid;

				if (ISteamUtils* pSteamUtils = SteamUtils())
				{
					rawValue = pSteamUtils->GetAppID();
				}

				return CreateApplicationIdentifier(rawValue);
			}

			bool CService::OpenDialog(EDialog dialog) const
			{
				switch (dialog)
				{
				case EDialog::Friends:
					return OpenDialog("friends");
				case EDialog::Community:
					return OpenDialog("community");
				case EDialog::Players:
					return OpenDialog("players");
				case EDialog::Settings:
					return OpenDialog("settings");
				case EDialog::Group:
					return OpenDialog("officialgamegroup");
				case EDialog::Achievements:
					return OpenDialog("achievements");
				case EDialog::Stats:
					return OpenDialog("stats");
				}

				return false;
			}

			bool CService::OpenDialogWithTargetUser(EUserTargetedDialog dialog, const AccountIdentifier& accountId) const
			{
				const CSteamID steamID = ExtractSteamID(accountId);

				switch (dialog)
				{
				case EUserTargetedDialog::UserInfo:
					return OpenDialogWithTargetUser("steamid", accountId);
				case EUserTargetedDialog::FriendAdd:
					return OpenDialogWithTargetUser("friendadd", accountId);
				case EUserTargetedDialog::FriendRemove:
					return OpenDialogWithTargetUser("friendremove", accountId);
				case EUserTargetedDialog::FriendRequestAccept:
					return OpenDialogWithTargetUser("friendrequestaccept", accountId);
				case EUserTargetedDialog::FriendRequestIgnore:
					return OpenDialogWithTargetUser("friendrequestignore", accountId);
				case EUserTargetedDialog::Chat:
					return OpenDialogWithTargetUser("chat", accountId);
				case EUserTargetedDialog::JoinTrade:
					return OpenDialogWithTargetUser("jointrade", accountId);
				case EUserTargetedDialog::Stats:
					return OpenDialogWithTargetUser("stats", accountId);
				case EUserTargetedDialog::Achievements:
					return OpenDialogWithTargetUser("achievements", accountId);
				}

				return false;
			}

			bool CService::OpenDialog(const char* szPage) const
			{
				if (ISteamFriends* pSteamFriends = SteamFriends())
				{
					pSteamFriends->ActivateGameOverlay(szPage);
					return true;
				}

				return false;
			}

			bool CService::OpenDialogWithTargetUser(const char* szPage, const AccountIdentifier& accountId) const
			{
				if (ISteamFriends* pSteamFriends = SteamFriends())
				{
					const CSteamID steamID = ExtractSteamID(accountId);
					pSteamFriends->ActivateGameOverlayToUser(szPage, steamID);
					return true;
				}

				return false;
			}

			bool CService::OpenBrowser(const char* szURL) const
			{
				if (ISteamFriends* pSteamFriends = SteamFriends())
				{
					pSteamFriends->ActivateGameOverlayToWebPage(szURL);
					return true;
				}

				return false;
			}

			bool CService::CanOpenPurchaseOverlay() const
			{
				if (ISteamUtils* pSteamUtils = SteamUtils())
				{
					return pSteamUtils->IsOverlayEnabled();
				}

				return false;
			}

			bool CService::OpenPurchaseOverlay(const ApplicationIdentifier& productId)
			{
				if (CanOpenPurchaseOverlay())
				{
					if (ISteamFriends* pFriends = SteamFriends())
					{
						const AppId_t appId = ExtractSteamID(productId);
						pFriends->ActivateGameOverlayToStore(appId, k_EOverlayToStoreFlag_None);

						return true;
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] Can't open purchase overlay as the Steam overlay is disabled");
				}

				return false;
			}

			bool CService::BrowseCatalog(ECatalogSortOrder sortOrder) const
			{
				if (ISteamApps* pSteamApps = SteamApps())
				{
					const size_t dlcCount = pSteamApps->GetDLCCount();
					DynArray<SStoreItem> items;
					items.reserve(dlcCount);

					for (size_t i = 0; i < dlcCount; ++i)
					{
						AppId_t appId;
						bool available;
						char name[128];
						if (pSteamApps->BGetDLCDataByIndex(i, &appId, &available, name, CRY_ARRAY_COUNT(name)))
						{
							SStoreItem item;
							item.id = CreateApplicationIdentifier(appId);
							item.name = name;
							item.type = EProductType::DLC;
							items.push_back(item);
						}
					}

					m_listeners.ForEach([&items](IListener* pListener) {
						pListener->OnCatalogReceived(true, items, false);
					});

					return true;
				}

				return false;
			}

			bool CService::QueryCatalogItemDetails(const SStoreItem& item) const
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] Querying catalog items is not supported on this platform");
				return false;
			}

			bool CService::QueryCatalogItemDetails(const ApplicationIdentifier& productId) const
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] Querying catalog items is not supported on this platform");
				return false;
			}

			bool CService::QueryCatalogItemDetails(const DynArray<SStoreItem>& items) const
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] Querying catalog items is not supported on this platform");
				return false;
			}

			bool CService::QueryCatalogItemDetails(const DynArray<ApplicationIdentifier>& productIds) const
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] Querying catalog items is not supported on this platform");
				return false;
			}

			bool CService::QueryInventory() const
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] Querying the inventory is not supported on this platform");
				return false;
			}

			bool CService::ConsumeInventoryItem(const InventoryItemIdentifier& id, uint32 quantity) const
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Steam] Consuming inventory items is not supported on this platform");
				return false;
			}

			void CService::UpdateFriends()
			{
				if (ISteamFriends* const pSteamFriends = SteamFriends())
				{
					m_accountMgr.ClearFriends();

					constexpr int friendFlags = k_EFriendFlagImmediate;
					const int friendCount = pSteamFriends->GetFriendCount(friendFlags);
					for (int iFriend = 0; iFriend < friendCount; ++iFriend)
					{
						const CSteamID friendId = pSteamFriends->GetFriendByIndex(iFriend, friendFlags);
						m_accountMgr.AddFriend(CreateAccountIdentifier(friendId), friendId);
					}
				}
			}

			const DynArray<IAccount*>& CService::GetFriendAccounts() const
			{
				return m_accountMgr.GetFriends();
			}

			void CService::AddBlocked(CAccount& account) const
			{
				stl::push_back_unique(m_blockedAccounts, &account);
			}

			void CService::RemoveBlocked(CAccount* pAccount) const
			{
				if (pAccount)
				{
					stl::find_and_erase_all(m_blockedAccounts, pAccount);
				}
			}

			void CService::UpdateBlocked()
			{
				if (ISteamFriends* const pSteamFriends = SteamFriends())
				{
					m_blockedAccounts.clear();

					constexpr int blockFlags = k_EFriendFlagBlocked | k_EFriendFlagIgnored | k_EFriendFlagIgnoredFriend;
					const int blockCount = pSteamFriends->GetFriendCount(blockFlags);
					for (int iFriend = 0; iFriend < blockCount; ++iFriend)
					{
						const CSteamID friendId = pSteamFriends->GetFriendByIndex(iFriend, blockFlags);
						m_blockedAccounts.push_back(TryGetAccount(friendId));
					}
				}
			}

			const DynArray<IAccount*>& CService::GetBlockedAccounts() const
			{
				return m_blockedAccounts;
			}

			const DynArray<IAccount*>& CService::GetMutedAccounts() const
			{
				// Steam does not support muting
				static const DynArray<IAccount*> dummy;
				return dummy;
			}

			bool CService::GetEnvironmentValue(const char* szVarName, string& valueOut) const
			{
				auto pos = m_environment.find(szVarName);
				if (pos != m_environment.end())
				{
					valueOut = pos->second;
					return true;
				}

				return false;
			}

			CAccount* CService::GetAccountById(const AccountIdentifier& accountId) const
			{
				return TryGetAccount(ExtractSteamID(accountId));
			}

			bool CService::IsFriendWith(const AccountIdentifier& accountId) const
			{
				if (ISteamFriends* pSteamFriends = SteamFriends())
				{
					const CSteamID steamId = ExtractSteamID(accountId);
					return pSteamFriends->HasFriend(steamId, k_EFriendFlagImmediate);
				}

				return false;
			}

			CServer* CService::CreateServer(bool bLocal)
			{
				if (m_pServer == nullptr)
				{
					m_pServer = stl::make_unique<CServer>(bLocal);
				}

				return m_pServer.get();
			}

			CAccount* CService::TryGetAccount(CSteamID id) const
			{
				if (CAccount* pAccount = m_accountMgr.FindAccount(CreateAccountIdentifier(id)))
				{
					return pAccount;
				}

				return m_accountMgr.AddAccount(id);
			}

			bool CService::GetAuthToken(string &tokenOut, int &issuerId)
			{
				char rgchToken[2048];
				uint32 unTokenLen = 0;
				ISteamUser* pSteamUser = SteamUser();
				if (!pSteamUser)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Steam user service not available");
					return false;
				}
				const HAuthTicket authTicket = pSteamUser->GetAuthSessionTicket(rgchToken, sizeof(rgchToken), &unTokenLen);
				m_pendingAuthTicketHandles.insert(authTicket);

				const char hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

				tokenOut.clear();
				for (int i = 0; i < unTokenLen; ++i)
				{
					char const byte = rgchToken[i];

					tokenOut += hex_chars[(byte & 0xF0) >> 4];
					tokenOut += hex_chars[(byte & 0x0F) >> 0];
				}

				return true;
			}

			bool CService::RequestUserInformation(const AccountIdentifier& accountId, UserInformationMask info)
			{
				if (ISteamFriends* pFriends = SteamFriends())
				{
					const bool requireNameOnly = info == eUIF_Name;
					return pFriends->RequestUserInformation(ExtractSteamID(accountId), requireNameOnly);
				}

				return false;
			}

			bool CService::IsLoggedIn() const
			{
				if (ISteamUser* pSteamUser = SteamUser())
					return pSteamUser->BLoggedOn();

				return false;
			}

			void CService::NotifyAccountAdded(IAccount& account) const
			{
				m_listeners.ForEach([&account](IListener* pListener) {
					pListener->OnAccountAdded(account);
				});
			}

			void CService::NotifyAccountRemoved(IAccount& account) const
			{
				m_listeners.ForEach([&account](IListener* pListener) {
					pListener->OnAccountRemoved(account);
				});
			}

			void CService::UpdateLicenses(bool notifyListeners)
			{
				m_licenses.clear();
				if (ISteamApps* pSteamApps = SteamApps())
				{
					const size_t dlcCount = pSteamApps->GetDLCCount();
					for (size_t i = 0; i < dlcCount; ++i)
					{
						AppId_t appId;
						bool available;
						char name[128];
						if (pSteamApps->BGetDLCDataByIndex(i, &appId, &available, name, CRY_ARRAY_COUNT(name)))
						{
							if (pSteamApps->BIsDlcInstalled(appId))
							{
								m_licenses.push_back(CreateInventoryItemIdentifier(appId));
							}
						}
					}
				}

				if (notifyListeners)
				{
					m_listeners.ForEach([this](IListener* pListener) {
						pListener->OnLicensesChanged(m_licenses);
					});
				}
			}

			void CService::MarkContentCorrupt(const bool missingFilesOnly) const
			{
				if (ISteamApps* pSteamApps = SteamApps())
				{
					pSteamApps->MarkContentCorrupt(missingFilesOnly);
				}
			}

			bool CService::FilterText(const char* szText, std::function<void(const char * szFiltered)> responseCallback) const
			{
				constexpr bool legallyRequiredOnly = true; // If false, also filter for profanity
				if (ISteamUtils* pSteamUtils = SteamUtils())
				{
					const size_t inSize = strlen(szText) + 1;
					std::vector<char> mutableText(szText, szText + inSize);
					pSteamUtils->FilterText(mutableText.data(), inSize, szText, legallyRequiredOnly);
					
					responseCallback(mutableText.data());
					return true;
				}

				return false;
			}

			CRYREGISTER_SINGLETON_CLASS(CService)
		}
	}
}
