// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <IPlatformEventManager.h>
#include <Events/BaseContext.h>
#include <Types/RemoteStorage.h>
#include <Types/Leaderboard.h>

namespace Cry::GamePlatform
{

class CPlatformEventManager : public IPlatformEventManager, public NoCopy
{

private:

	using TListenerSet = CListenerSet<IPlatformEventListener*>;

	//! An embedded service listener is instantiated for each actively listened platform
	struct SEmbeddedServiceListener
		: public IService::IListener
		  , public ILeaderboards::IListener
		  , public IMatchmaking::IListener
		  , public IRemoteFileBase::IListener
		  , public IUserGeneratedContentManager::IListener
	{

	public:

		SEmbeddedServiceListener(CPlatformEventManager* pManager, EGamePlatform platform);
		SEmbeddedServiceListener(const SEmbeddedServiceListener&);
		SEmbeddedServiceListener(SEmbeddedServiceListener&&);
		SEmbeddedServiceListener& operator=(const SEmbeddedServiceListener&);
		SEmbeddedServiceListener& operator=(SEmbeddedServiceListener&&);

		~SEmbeddedServiceListener();

		// IService::IListener
		virtual void OnAccountAdded(IAccount& account) override;
		virtual void OnAccountNameChanged(const AccountIdentifier& accountId, const char* szName) override;
		virtual void OnAccountRemoved(IAccount& account) override;
		virtual void OnAuthTokenReceived(bool success, const char* szToken) override;
		virtual void OnAvatarImageLoaded(const AccountIdentifier& accountId) override;
		virtual void OnCatalogItemsReceived(bool success, const DynArray<IService::SStoreItemDetails>& items) override;
		virtual void OnCatalogReceived(bool success, const DynArray<IService::SStoreItem>& items, bool remainingItemsInCatalog) override;
		virtual void OnEnvironmentVariableChanged(const char* szVarName, const char* szNewValue) override;
		virtual void OnGetSteamAuthTicketResponse(bool success, uint32 authTicket) override;
		virtual void OnInventoryItemConsumed(bool success, const InventoryItemIdentifier& id, uint32 quantity) override;
		virtual void OnInventoryReceived(bool success, const DynArray<IService::SInventoryItem>& items) override;
		virtual void OnLicensesChanged(const DynArray<InventoryItemIdentifier>& newLicenses) override;
		virtual void OnLogInStateChanged(const AccountIdentifier& accountId, bool isLoggedIn) override;
		virtual void OnMicroTransactionAuthorizationResponse(const TransactionIdentifier& orderId, bool authorized) override;
		virtual void OnNetworkStatusChanged(const EConnectionStatus& connectionStatus) override;
		virtual void OnOverlayActivated(const ServiceIdentifier& serviceId, bool active) override;
		virtual void OnPersonaStateChanged(const IAccount& account, CEnumFlags<EPersonaChangeFlags> changeFlags) override;
		virtual void OnPlatformServiceError(const AccountIdentifier& accountId, int32 errorCode) override;
		virtual void OnShutdown(const ServiceIdentifier& serviceId) override;
		virtual void OnUserPermissionChanged(const AccountIdentifier& accountId, IService::EPermission permission, bool hasPermission) override;
		// ~IService::IListener

		// ILeaderboards::IListener
		virtual void OnEntryDownloaded(const LeaderboardIdentifier& leaderboardId, const char* szPlayerName, int rank, int score, ILeaderboards::EScoreType scoreType, bool bCurrentUser) override;
		// ~ILeaderboards::IListener

		// IMatchmaking::IListener
		virtual void OnLobbyQueryComplete(int numLobbiesFound) override;
		virtual void OnCreatedLobby(IUserLobby* pLobby) override;
		virtual void OnJoinedLobby(IUserLobby* pLobby) override;
		virtual void OnLobbyJoinFailed(const LobbyIdentifier& lobbyId) override;
		virtual void OnExternalInvitationRequest(const DynArray<AccountIdentifier>& invitees) override {};
		virtual bool OnPreJoinLobby(const LobbyIdentifier& lobbyId) override;
		// ~IMatchmaking::IListener

		// IRemoteFileBase::IListener
		virtual bool OnFileShared(IRemoteFile* pFile) override;
		virtual bool OnDownloadedSharedFile(ISharedRemoteFile* pFile) override;
		// ~IRemoteFileBase::IListener

		// IUserGeneratedContent::IListener
		virtual void OnContentCreated(IUserGeneratedContent* pContent, bool bRequireLegalAgreementAccept) override;
		// ~IUserGeneratedContent::IListener

	protected:
		void RegisterWithService();
		void UnregisterWithService();

	private:

		EGamePlatform m_platform;
		CPlatformEventManager* m_pManager;
	};

	//! An embedded lobby listener is instantiated for each actively listened lobby
	struct SEmbeddedLobbyListener : public IUserLobby::IListener
	{

		SEmbeddedLobbyListener(CPlatformEventManager* pManager, const LobbyIdentifier& lobbyId);
		~SEmbeddedLobbyListener();

		const LobbyIdentifier& GetLobbyId() const
		{
			return m_lobbyId;
		}

		// IUserLobby::IListener
		virtual void OnPlayerEntered(const AccountIdentifier& userId) override final;
		virtual void OnPlayerLeft(const AccountIdentifier& userId) override final;
		virtual void OnPlayerDisconnected(const AccountIdentifier& userId) override final;
		virtual void OnPlayerKicked(const AccountIdentifier& userId, const AccountIdentifier& moderatorId) override final;
		virtual void OnPlayerBanned(const AccountIdentifier& userId, const AccountIdentifier& moderatorId) override final;
		virtual void OnLeave() override final;
		virtual void OnGameCreated(IServer::Identifier serverId, uint32 ipAddress, uint16 port, bool bLocal) override final;
		virtual void OnChatMessage(const AccountIdentifier& userId, const char* szMessage) override final;
		virtual void OnLobbyDataUpdate(const LobbyIdentifier& lobbyId) override final;
		virtual void OnUserDataUpdate(const AccountIdentifier& userId) override final;
		// ~IUserLobby::IListener

	private:

		LobbyIdentifier        m_lobbyId;
		CPlatformEventManager* m_pManager;

	};

	//! Contains embedded and external listeners for each platform
	struct SPlatformContainer
	{
		SPlatformContainer(CPlatformEventManager* pManager, EGamePlatform platform)
			: m_platform(platform)
			, m_embeddedServiceListener{pManager, platform}
			, m_embeddedLobbyListeners{}
			, m_externalListeners{0}
		{}

		~SPlatformContainer() = default;

		EGamePlatform                       m_platform;
		SEmbeddedServiceListener            m_embeddedServiceListener;
		std::vector<SEmbeddedLobbyListener> m_embeddedLobbyListeners;
		TListenerSet                        m_externalListeners;
	};

public:

	CPlatformEventManager() = default;
	~CPlatformEventManager() = default;

	CPlatformEventManager(const CPlatformEventManager&) = delete;
	CPlatformEventManager& operator=(const CPlatformEventManager&) = delete;
	CPlatformEventManager(CPlatformEventManager&&) = default;
	CPlatformEventManager& operator=(CPlatformEventManager&&) = default;

	// IPlatformEventListener
	virtual void AddListener(EGamePlatform platform, IPlatformEventListener* pListener) override;
	virtual void RemoveListener(EGamePlatform platform, IPlatformEventListener* pListener) override;

	virtual void StartListening(const LobbyIdentifier& lobbyId) override;
	virtual void StopListening(const LobbyIdentifier& lobbyId) override;

	virtual void StartListening(const CCachedLocal& localCache) override;
	virtual void StopListening(const CCachedLocal& localCache) override;

	virtual void StartListening(const CCachedShared& sharedCache) override;
	virtual void StopListening(const CCachedShared& sharedCache) override;
	// ~IPlatformEventListener

	virtual void HandleEvent(const CBaseEventContext& context) override;

protected:

	bool ContainerExists(EGamePlatform platform) const;
	void ExecuteForPlatform(EGamePlatform platform, std::function<void(SPlatformContainer&)> executor);
	void TryCleanContainers();

private:

	std::vector<SPlatformContainer> m_platformContainers;

};

} // Cry::GamePlatform
