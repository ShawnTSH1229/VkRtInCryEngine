// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformService.h"
#include "SteamAccount.h"
#include "SteamServer.h"
#include "SteamLeaderboards.h"
#include "SteamStatistics.h"
#include "SteamRemoteStorage.h"
#include "SteamMatchmaking.h"
#include "SteamNetworking.h"
#include "PlatformAccountManager.h"

#include <CrySystem/ILocalizationManager.h>
#include <CrySystem/ICryPlugin.h>

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			class CService final
				: public Cry::IEnginePlugin
				, public IService
				, public IAccountManagerListener
			{
			public:
				CRYINTERFACE_BEGIN()
					CRYINTERFACE_ADD(Cry::IEnginePlugin)
				CRYINTERFACE_END()
				CRYGENERATE_SINGLETONCLASS_GUID(CService, "Plugin_CryGamePlatformSteam", SteamServiceID)

				CService();
				virtual ~CService() = default;

				// Cry::IEnginePlugin
				virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
				virtual void MainUpdate(float frameTime) override;
				virtual const char* GetName() const override { return "CryGamePlatformSteam"; }
				virtual const char* GetCategory() const override { return "GamePlatform"; }
				// ~Cry::IEnginePlugin

				// IService
				virtual void AddListener(IListener& listener, const char* szName) override { m_listeners.Add(&listener, szName); }
				virtual void RemoveListener(IListener& listener) override { m_listeners.Remove(&listener); }

				virtual void Shutdown() override;

				virtual ServiceIdentifier GetServiceIdentifier() const override;
				virtual int GetBuildIdentifier() const override;

				virtual bool SetIdentifierFromString(AccountIdentifier& out, const string& in) const override;
				virtual bool SetIdentifierFromString(LobbyIdentifier& out, const string& in) const override;
				virtual bool SetIdentifierFromString(ApplicationIdentifier& out, const string& in) const override;
				virtual bool SetIdentifierFromString(InventoryItemIdentifier& out, const string& in) const override;
				virtual bool SetIdentifierFromString(TransactionIdentifier& out, const string& in) const override { return false; }
				virtual bool SetIdentifierFromString(LeaderboardIdentifier& out, const string& in) const override;

				virtual CAccount* GetLocalAccount() const override;

				virtual const DynArray<IAccount*>& GetFriendAccounts() const override;
				virtual const DynArray<IAccount*>& GetBlockedAccounts() const override;
				virtual const DynArray<IAccount*>& GetMutedAccounts() const override;
				virtual bool GetEnvironmentValue(const char* szVarName, string& valueOut) const override;
				virtual void RefreshFriendAccounts() override { CRY_ASSERT_MESSAGE(false, "Not implemented!"); }
				virtual void RefreshBlockedAccounts() override { CRY_ASSERT_MESSAGE(false, "Not implemented!"); }
				virtual void RefreshMutedAccounts() override { CRY_ASSERT_MESSAGE(false, "Not implemented!"); }
				virtual void RefreshPermissions() override { CRY_ASSERT_MESSAGE(false, "Not implemented!"); }
				virtual void RefreshPrivacyPermissions(const DynArray<AccountIdentifier>* pTargetAccountIds) override { CRY_ASSERT_MESSAGE(false, "Not implemented!"); }
				virtual CAccount* GetAccountById(const AccountIdentifier& accountId) const override;
				virtual void AddAccountToLocalSession(const AccountIdentifier& accountId) override {};
				virtual void RemoveAccountFromLocalSession(const AccountIdentifier& accountId) override {};
				virtual bool IsFriendWith(const AccountIdentifier& accountId) const override;

				virtual CServer* CreateServer(bool bLocal) override;
				virtual CServer* GetLocalServer() const override { return m_pServer.get(); }

				virtual CLeaderboards* GetLeaderboards() const override { return m_pSteamLeaderboards.get(); }
				virtual CStatistics* GetStatistics() const override { return m_pStatistics.get(); }
				virtual CRemoteStorage* GetRemoteStorage() const override { return m_pRemoteStorage.get(); }
				virtual CMatchmaking* GetMatchmaking() const override { return m_pMatchmaking.get(); }
				virtual CNetworking* GetNetworking() const override { return m_pNetworking.get(); }
				virtual IStreamInstall* GetStreamInstall() const override { return nullptr; }

				virtual bool OwnsApplication(ApplicationIdentifier id) const override;
				virtual ApplicationIdentifier GetApplicationIdentifier() const override;

				virtual bool OpenDialog(EDialog dialog) const override;
				virtual bool OpenDialogWithTargetUser(EUserTargetedDialog dialog, const AccountIdentifier& accountId) const override;
				virtual bool OpenBrowser(const char* szURL) const override;

				virtual bool CanOpenPurchaseOverlay() const override;
				virtual bool OpenPurchaseOverlay(const ApplicationIdentifier& productId) override;

				virtual bool BrowseCatalog(ECatalogSortOrder sortOrder) const override;
				virtual bool QueryCatalogItemDetails(const SStoreItem& item) const override;
				virtual bool QueryCatalogItemDetails(const ApplicationIdentifier& productId) const override;
				virtual bool QueryCatalogItemDetails(const DynArray<SStoreItem>& items) const override;
				virtual bool QueryCatalogItemDetails(const DynArray<ApplicationIdentifier>& productIds) const override;

				virtual bool QueryInventory() const override;
				virtual bool ConsumeInventoryItem(const InventoryItemIdentifier& id, uint32 quantity) const override;

				virtual bool GetAuthToken(string &tokenOut, int &issuerId) override;

				virtual EConnectionStatus GetConnectionStatus() const override { return EConnectionStatus::Connected; }
				virtual void CanAccessMultiplayerServices(std::function<void(bool authorized)> asynchronousCallback) override { asynchronousCallback(true); }

				virtual bool RequestUserInformation(const AccountIdentifier& accountId, UserInformationMask info) override;
				virtual bool IsLoggedIn() const override;

				virtual EPermissionResult GetPermission(EPermission permission) const override { return EPermissionResult::NotApplicable; }
				virtual EPermissionResult GetPrivacyPermission(const AccountIdentifier& targetAccountId, EPrivacyPermission permission, ERelationship targetRelationship) const override { return EPermissionResult::NotApplicable; }
				virtual const DynArray<InventoryItemIdentifier>& GetLicenses() const override { return m_licenses; }

				virtual void MarkContentCorrupt(bool missingFilesOnly) const override;
			
				virtual void SetIsInMultiplayer(bool isInMultiplayer) override {};

				virtual bool FilterText(const char* szText, std::function<void(const char* szFiltered)> responseCallback) const override;
				// ~IService

				// IUserManagerListener
				virtual void OnAccountAdded(IAccount& account) override { NotifyAccountAdded(account); }
				virtual void OnAccountRemoved(IAccount& account) override { NotifyAccountRemoved(account); }
				// ~IUserManagerListener

			protected:
				STEAM_CALLBACK(CService, OnGameOverlayActivated, GameOverlayActivated_t, m_callbackGameOverlayActivated);
				STEAM_CALLBACK(CService, OnAvatarImageLoaded, AvatarImageLoaded_t, m_onAvatarImageLoadedCallback);
				STEAM_CALLBACK(CService, OnGetSteamAuthTicketResponse, GetAuthSessionTicketResponse_t, m_callbackGetSteamAuthTicketResponse);
				STEAM_CALLBACK(CService, OnPersonaStateChange, PersonaStateChange_t, m_callbackOnPersonaChange);
				STEAM_CALLBACK(CService, OnMicroTransactionAuthorizationResponse, MicroTxnAuthorizationResponse_t, m_callbackOnMicroTransactionResponse);
				STEAM_CALLBACK(CService, OnLicensesUpdated, LicensesUpdated_t, m_callbackOnLicensesUpdated);

				bool OpenDialog(const char* szPage) const;
				bool OpenDialogWithTargetUser(const char* szPage, const AccountIdentifier& accountId) const;

				CAccount* TryGetAccount(CSteamID id) const;

				void NotifyAccountAdded(IAccount& account) const;
				void NotifyAccountRemoved(IAccount& account) const;

				void UpdateLicenses(bool notifyListeners);
				void UpdateFriends();
				
				void AddBlocked(CAccount& account) const;
				void RemoveBlocked(CAccount* pAccount) const;
				void UpdateBlocked();

				void SetEnvironmentValue(const char* szVarName, const string& value);

			private:
				void InitializeLocalizationLanguage();
				void UpdateLanguages(ILocalizationManager::EPlatformIndependentLanguageID languageId);

				std::unique_ptr<CStatistics> m_pStatistics;
				std::unique_ptr<CLeaderboards> m_pSteamLeaderboards;
				std::unique_ptr<CRemoteStorage> m_pRemoteStorage;
				std::unique_ptr<CMatchmaking> m_pMatchmaking;

				std::unique_ptr<CNetworking> m_pNetworking;

				std::unique_ptr<CServer> m_pServer;

				mutable CAccountManager<CAccount> m_accountMgr;
				mutable DynArray<IAccount*> m_blockedAccounts;
				std::unordered_map<string, string, stl::hash_strcmp<string>, stl::hash_strcmp<string>> m_environment;
				DynArray<InventoryItemIdentifier> m_licenses;

				mutable CListenerSet<IListener*> m_listeners = 1;

				using HAuthTicket = uint32;
				std::set<HAuthTicket> m_pendingAuthTicketHandles;
			};
		}
	}
}
