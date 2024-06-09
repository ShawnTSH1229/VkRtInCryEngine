// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "DiscordService.h"
#include "IGamePlatform.h"

#include "DiscordUserIdentifier.h"
#include "DiscordHelper.h"

#include <CrySystem/ICmdLine.h>
#include <CrySystem/ConsoleRegistration.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			namespace Predicate
			{
				struct SWithDiscordID
				{
					explicit SWithDiscordID(const discord::UserId& id) : m_id(id) {}
					bool operator()(const std::unique_ptr<CAccount>& pUser) const { return pUser->GetDiscordID() == m_id; }
					bool operator()(const IAccount* pUser) const { return static_cast<const CAccount*>(pUser)->GetDiscordID() == m_id; }

				private:
					discord::UserId m_id;
				};
			}

			inline ApplicationIdentifierValue ReadAppIdFromCVar()
			{
				const char* discord_appId = "";
				REGISTER_CVAR(discord_appId, "", VF_REQUIRE_APP_RESTART, "Discord application id. Requires application restart.");
				ApplicationIdentifierValue appId = 0;
				const bool ok = FromString(appId, discord_appId);
				if (!ok)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord] Initialization failed with empty or invalid app Id : '%s'", discord_appId);
				}
				return appId;
			}

			CService::CService()
				: m_appId(ReadAppIdFromCVar())
			{
				REGISTER_CVAR(discord_require, 0, VF_REQUIRE_APP_RESTART, "Whether discord should be required: 0 - Not required; 1 - Required");
			}

			CService::~CService()
			{
				if (gEnv && gEnv->pConsole)
				{
					gEnv->pConsole->UnregisterVariable("discord_require", true);
				}
			}

			bool CService::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
			{
				int discord_editor_support;
				REGISTER_CVAR(discord_editor_support, 0, VF_REQUIRE_APP_RESTART, "Defines whether the Discord integration is enabled in Sandbox. Requires application restart.");
				gEnv->pConsole->UnregisterVariable("discord_editor_support");

				if (env.IsEditor() && discord_editor_support == 0)
				{
					// Don't use Discord integration in Sandbox
					return true;
				}

				if (env.IsDedicated())
				{
					// Don't use Discord integration in Dedicated Server
					return true;
				}

				return Initialize();
			}

			bool CService::Initialize()
			{
				Cry::GamePlatform::IPlugin* pGamePlatform = gEnv->pSystem->GetIPluginManager()->QueryPlugin<Cry::GamePlatform::IPlugin>();
				Cry::GamePlatform::IService* pSteamService = pGamePlatform ? pGamePlatform->GetService(SteamServiceID) : nullptr;
				const bool requireDiscord = discord_require && pGamePlatform && pGamePlatform->GetMainService() == nullptr;

				if (!Helper::Initialize(m_appId, requireDiscord))
				{
					if (requireDiscord)
					{
#ifdef RELEASE
						return false;
#endif
					}

					// Allow silent failure in development mode and when discord is not required
					return true;
				}

				if (!pGamePlatform || !Helper::RunCallbacks())
				{
					// Allow silent failure
					return true;
				}

				ApplicationIdentifier steamAppId;
				if (pSteamService)
				{
					steamAppId = pSteamService->GetApplicationIdentifier();
				}
#ifdef CRY_PLATFORM_WINDOWS
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Discord][Service] Initialize(): Unable to retrieve Steam Service. Integration will be limited.");
				}
#endif

				uint64 rawSteamAppId = 0;
				if (steamAppId.GetAsUint64(rawSteamAppId))
				{
					if (discord::ActivityManager* pMgr = Helper::ActivityMgr())
					{
						const discord::Result result = pMgr->RegisterSteam(static_cast<uint32_t>(rawSteamAppId));
						if (result != discord::Result::Ok)
						{
							CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] Initialize(): Failed to register steam with error %s", Helper::ErrorString(result));
						}
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] Initialize(): Could not access Activity Manager");
					}
				}
#ifdef CRY_PLATFORM_WINDOWS
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Discord][Service] Initialize(): Unable to retrieve Steam app id. Integration will be limited.");
				}
#endif

				m_accountMgr.AddListener(*this, GetName());

				if (discord::UserManager* pMgr = Helper::UserMgr())
				{
					pMgr->OnCurrentUserUpdate.Connect(std::bind(&CService::OnCurrentUserUpdate, this));
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] Initialize(): Could not access User Manager");
				}

				if (discord::RelationshipManager* pMgr = Helper::RelationshipMgr())
				{
					pMgr->OnRefresh.Connect(std::bind(&CService::OnRelationshipRefresh, this));
					pMgr->OnRelationshipUpdate.Connect(std::bind(&CService::OnRelationshipUpdate, this, std::placeholders::_1));
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] Initialize(): Could not access Relationship Manager");
				}

				string appIdString;
				appIdString.Format("%d", m_appId);
				SetEnvironmentValue("ApplicationId", appIdString);

				EnableUpdate(Cry::IEnginePlugin::EUpdateStep::MainUpdate, true);

				CryLogAlways("[Discord][Service] Successfully initialized Discord Game SDK");

				pGamePlatform->RegisterService(*this);

				// Set localization according to the settings made in Discord
				InitializeLocalizationLanguage();

				return true;
			}

			void CService::InitializeLocalizationLanguage()
			{
				// Map Discord API language codes to engine
				// Full list of Discord API language codes can be found here: https://discordapp.com/developers/docs/dispatch/field-values
				std::map<string, ILocalizationManager::EPlatformIndependentLanguageID> translationMappings;
				translationMappings["en-US"] = ILocalizationManager::ePILID_English;
				translationMappings["en-GB"] = ILocalizationManager::ePILID_English;
				translationMappings["zh-CN"] = ILocalizationManager::ePILID_ChineseS;
				translationMappings["zh-TW"] = ILocalizationManager::ePILID_ChineseT;
				translationMappings["cs"] = ILocalizationManager::ePILID_Czech;
				translationMappings["da"] = ILocalizationManager::ePILID_Danish;
				translationMappings["nl"] = ILocalizationManager::ePILID_Dutch;
				translationMappings["fr"] = ILocalizationManager::ePILID_French;
				translationMappings["de"] = ILocalizationManager::ePILID_German;
				//m_translationMappings["el"] = ILocalizationManager::ePILID_Greek;
				//m_translationMappings["hu"] = ILocalizationManager::ePILID_Hungarian;
				translationMappings["it"] = ILocalizationManager::ePILID_Italian;
				translationMappings["ja"] = ILocalizationManager::ePILID_Japanese;
				translationMappings["ko"] = ILocalizationManager::ePILID_Korean;
				translationMappings["no"] = ILocalizationManager::ePILID_Norwegian;
				translationMappings["pl"] = ILocalizationManager::ePILID_Polish;
				translationMappings["pt-BR"] = ILocalizationManager::ePILID_Portuguese;
				translationMappings["ru"] = ILocalizationManager::ePILID_Russian;
				translationMappings["es-ES"] = ILocalizationManager::ePILID_Spanish;
				translationMappings["sv-SE"] = ILocalizationManager::ePILID_Swedish;
				translationMappings["tr"] = ILocalizationManager::ePILID_Turkish;
				//m_translationMappings["bg"] = ILocalizationManager::ePILID_Bulgarian;
				//m_translationMappings["uk"] = ILocalizationManager::ePILID_Ukrainian;
				translationMappings["fi"] = ILocalizationManager::ePILID_Finnish;
				//m_translationMappings["hr"] = ILocalizationManager::ePILID_Croatian;
				//m_translationMappings["ro"] = ILocalizationManager::ePILID_Romanian;
				//m_translationMappings["lt"] = ILocalizationManager::ePILID_Lithuanian;

				ILocalizationManager::EPlatformIndependentLanguageID languageId = ILocalizationManager::ePILID_English; // Default to english if we the language is not supported
				// Set localization according to the settings made in Discord
				if (discord::ApplicationManager* pMgr = Helper::ApplicationMgr())
				{
					char szDesiredLanguage[128];
					pMgr->GetCurrentLocale(szDesiredLanguage);

					const auto remapIt = translationMappings.find(szDesiredLanguage);
					if (remapIt != translationMappings.end())
					{
						languageId = remapIt->second;
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[GamePlatform] Language mapping missing for %s", szDesiredLanguage);
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] Initialize(): Could not access Application Manager");
				}

				UpdateLanguages(languageId);
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

				CRY_ASSERT_MESSAGE(languageId != ILocalizationManager::ePILID_MAX_OR_INVALID, "Invalid language ID");
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
				Helper::RunCallbacks();
			}

			void CService::Shutdown()
			{
				m_accountMgr.Clear();

				m_listeners.ForEach([](IListener* pListener)
				{
					pListener->OnShutdown(DiscordServiceID);
				});

				Helper::Finalize();

				CryLogAlways("[Discord][Service] Discord Game SDK shutdown successfully");
			}

			ServiceIdentifier CService::GetServiceIdentifier() const
			{
				return DiscordServiceID;
			}

			int CService::GetBuildIdentifier() const
			{
				// Discord does not have a build identifier unless it's sold on their store
				return 0;
			}

			bool CService::SetIdentifierFromString(AccountIdentifier& out, const string& in) const
			{
				return CreateIdentifierFromString(&CreateAccountIdentifier, in, out, AccountIdentifierValue{});
			}

			bool CService::SetIdentifierFromString(ApplicationIdentifier& out, const string& in) const
			{
				return CreateIdentifierFromString(&CreateApplicationIdentifier, in, out, ApplicationIdentifierValue{});
			}

			CAccount* CService::GetLocalAccount() const
			{
				if (!m_waitingForLocalAccount)
				{
					discord::User user{};
					if (Helper::GetCurrentUser(user))
					{
						return TryGetAccount(user);
					}
				}
				return nullptr;
			}
			
			bool CService::OwnsApplication(ApplicationIdentifier id) const
			{
				CRY_ASSERT_MESSAGE(false, "The method or operation is not implemented.");
				return false;
			}

			ApplicationIdentifier CService::GetApplicationIdentifier() const
			{
				return CreateApplicationIdentifier(m_appId);
			}

			bool CService::OpenDialog(EDialog dialog) const
			{
				return false;
			}

			bool CService::OpenDialogWithTargetUser(EUserTargetedDialog dialog, const AccountIdentifier& accountId) const
			{
				return false;
			}

			bool CService::OpenBrowser(const char* szURL) const
			{
				// Discord does not support opening browsers
				return false;
			}

			bool CService::CanOpenPurchaseOverlay() const
			{
				return false;
			}

			bool CService::OpenPurchaseOverlay(const ApplicationIdentifier& productId)
			{
				return false;
			}

			bool CService::BrowseCatalog(ECatalogSortOrder sortOrder) const
			{
				return false;
			}

			bool CService::QueryCatalogItemDetails(const SStoreItem& item) const
			{
				return false;
			}

			bool CService::QueryCatalogItemDetails(const ApplicationIdentifier& productId) const
			{
				return false;
			}

			bool CService::QueryCatalogItemDetails(const DynArray<SStoreItem>& items) const
			{
				return false;
			}

			bool CService::QueryCatalogItemDetails(const DynArray<ApplicationIdentifier>& productIds) const
			{
				return false;
			}
			
			bool CService::QueryInventory() const
			{
				return false;
			}

			bool CService::ConsumeInventoryItem(const InventoryItemIdentifier& id, uint32 quantity) const
			{
				return false;
			}

			const DynArray<IAccount*>& CService::GetFriendAccounts() const
			{
				UpdateAllRelationships();
				return m_accountMgr.GetFriends();
			}

			const DynArray<IAccount*>& CService::GetBlockedAccounts() const
			{
				return m_blockedAccounts;
			}

			const DynArray<IAccount*>& CService::GetMutedAccounts() const
			{
				// Discord does not support muting
				return m_mutedAccounts;
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
				if (!m_waitingForLocalAccount)
				{
					discord::User user{};
					if (Helper::GetCurrentUser(user))
					{
						if (user.GetId() == ExtractDiscordID(accountId))
						{
							return TryGetAccount(user);
						}
					}
				}
				return nullptr;
			}

			bool CService::IsFriendWith(const AccountIdentifier& accountId) const
			{
				UpdateAllRelationships();
				return m_accountMgr.IsFriendWith(accountId);
			}

			IServer* CService::CreateServer(bool bLocal)
			{
				return nullptr;
			}

			bool CService::GetAuthToken(string &tokenOut, int &issuerId)
			{
				return false;
			}

			bool CService::RequestUserInformation(const AccountIdentifier& accountId, UserInformationMask info)
			{
				return false;
			}

			bool CService::IsLoggedIn() const
			{
				return GetLocalAccount() && !m_waitingForLocalAccount;
			}

			void CService::NotifyAccountAdded(IAccount& pAccount) const
			{
				m_listeners.ForEach([&pAccount](IListener* pListener)
				{
					pListener->OnAccountAdded(pAccount);
				});
			}

			void CService::NotifyAccountRemoved(IAccount& pAccount) const
			{
				m_listeners.ForEach([&pAccount](IListener* pListener)
				{
					pListener->OnAccountRemoved(pAccount);
				});
			}

			CAccount* CService::TryGetAccount(discord::User user) const
			{
				if (CAccount* pAccount = m_accountMgr.FindAccount(CreateAccountIdentifier(user.GetId())))
				{
					return pAccount;
				}

				return m_accountMgr.AddAccount(*const_cast<CService*>(this), user);
			}

			
			// Callbacks

			void CService::OnAvatarImageLoaded(const AccountIdentifier userId)
			{
				m_listeners.ForEach([&userId](IListener* pListener)
				{
					pListener->OnAvatarImageLoaded(userId);
				});
			}

			void CService::OnCurrentUserUpdate()
			{
				discord::User usr = {};
				if (Helper::GetCurrentUser(usr))
				{
					CryComment("[Discord] User connected: userid='%" PRIi64 "' nick='%s'", usr.GetId(), usr.GetUsername());
					m_waitingForLocalAccount = false;
					return;
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_COMMENT, "[Discord][Service] GetCurrentUser() failed to get current user");
				}
			}

			void CService::OnRelationshipRefresh()
			{
				UpdateAllRelationships();
			}

			void CService::OnRelationshipUpdate(discord::Relationship const& relationship)
			{
				const AccountIdentifier accountId = CreateAccountIdentifier(relationship.GetUser().GetId());
				CAccount* pAccount = TryGetAccount(relationship.GetUser());

				m_accountMgr.RemoveFriend(accountId);
				stl::find_and_erase(m_blockedAccounts, pAccount);

				if (relationship.GetType() == discord::RelationshipType::Friend)
				{
					m_accountMgr.AddFriend(*pAccount);
				}
				else if (relationship.GetType() == discord::RelationshipType::Blocked)
				{
					m_blockedAccounts.push_back(pAccount);
				}
			}


			// Private Functions

			void CService::UpdateAllRelationships() const
			{
				m_accountMgr.ClearFriends();
				m_blockedAccounts.clear();

				if (discord::RelationshipManager* pMgr = Helper::RelationshipMgr())
				{
					pMgr->Filter([](const discord::Relationship& relationship) 
					{
						return relationship.GetType() == discord::RelationshipType::Friend || relationship.GetType() == discord::RelationshipType::Blocked;
					});

					std::int32_t friendCount = 0;
					discord::Relationship relationship{};
					pMgr->Count(&friendCount);
					for (size_t i = 0; i < friendCount; ++i)
					{
						const discord::Result result = pMgr->GetAt(i, &relationship);
						if (result != discord::Result::Ok)
						{
							CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] FilterRelationship(): GetAt() failed with error %s", Helper::ErrorString(result));
							continue;
						}

						CAccount* pAccount = TryGetAccount(relationship.GetUser());

						if (relationship.GetType() == discord::RelationshipType::Friend)
						{
							m_accountMgr.AddFriend(*pAccount);
						}
						else if (relationship.GetType() == discord::RelationshipType::Blocked)
						{
							stl::push_back_unique(m_blockedAccounts, pAccount);
						}
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] FilterRelationship(): Could not access Relationship Manager");
				}
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

			CRYREGISTER_SINGLETON_CLASS(CService)
		}
	}
}
