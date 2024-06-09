// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "PlatformEventManager.h"
#include <IPlugin.h>
#include <GamePlatformHelper.h>
#include <Events/OnAccountAdded.h>
#include <Events/OnAccountNameChanged.h>
#include <Events/OnAccountRemoved.h>
#include <Events/OnAuthTokenReceived.h>
#include <Events/OnAvatarImageLoaded.h>
#include <Events/OnCatalogItemsReceived.h>
#include <Events/OnCatalogReceived.h>
#include <Events/OnEnvironmentVariableChanged.h>
#include <Events/OnGetSteamAuthTicketResponse.h>
#include <Events/OnInventoryItemConsumed.h>
#include <Events/OnInventoryReceived.h>
#include <Events/OnLicensesChanged.h>
#include <Events/OnLogInStateChanged.h>
#include <Events/OnMicroTransactionAuthorizationResponse.h>
#include <Events/OnNetworkStatusChanged.h>
#include <Events/OnOverlayActivated.h>
#include <Events/OnPersonaStateChanged.h>
#include <Events/OnPlatformServiceError.h>
#include <Events/OnShutdown.h>
#include <Events/OnUserPermissionChanged.h>
#include <Events/OnLeaderboardEntryDownloaded.h>
#include <Events/OnLobbyQueryComplete.h>
#include <Events/OnCreatedLobby.h>
#include <Events/OnJoinedLobby.h>
#include <Events/OnLobbyJoinFailed.h>
#include <Events/OnPreJoinLobby.h>
#include <Events/OnPlayerEntered.h>
#include <Events/OnPlayerLeft.h>
#include <Events/OnPlayerDisconnected.h>
#include <Events/OnPlayerKicked.h>
#include <Events/OnPlayerBanned.h>
#include <Events/OnLeave.h>
#include <Events/OnGameCreated.h>
#include <Events/OnChatMessage.h>
#include <Events/OnLobbyDataUpdate.h>
#include <Events/OnUserDataUpdate.h>
#include <Events/OnFileShared.h>
#include <Events/OnDownloadedSharedFile.h>
#include <Events/OnContentCreated.h>
#include <Events/OnTextFiltered.h>
#include <Events/OnCheckMultiplayerAccess.h>

namespace Cry::GamePlatform
{

CPlatformEventManager::SEmbeddedServiceListener::SEmbeddedServiceListener(CPlatformEventManager* pManager, EGamePlatform platform)
	: m_pManager(pManager)
	, m_platform(platform)
{
	RegisterWithService();
}

CPlatformEventManager::SEmbeddedServiceListener::SEmbeddedServiceListener(const SEmbeddedServiceListener& other) 
	: m_pManager(other.m_pManager)
	, m_platform(other.m_platform)
{
	RegisterWithService();
}

CPlatformEventManager::SEmbeddedServiceListener::SEmbeddedServiceListener(SEmbeddedServiceListener&& other) 
	: m_pManager(other.m_pManager)
	, m_platform(other.m_platform)
{
	RegisterWithService();
}

CPlatformEventManager::SEmbeddedServiceListener& CPlatformEventManager::SEmbeddedServiceListener::operator=(const CPlatformEventManager::SEmbeddedServiceListener& other)
{
	if (this == &other)
		return *this;

	m_pManager = other.m_pManager;
	m_platform = other.m_platform;

	RegisterWithService();

	return *this;
}

CPlatformEventManager::SEmbeddedServiceListener& CPlatformEventManager::SEmbeddedServiceListener::operator=(CPlatformEventManager::SEmbeddedServiceListener&& other)
{
	if (this == &other)
		return *this;

	m_pManager = other.m_pManager;
	m_platform = other.m_platform;

	RegisterWithService();

	return *this;
}

CPlatformEventManager::SEmbeddedServiceListener::~SEmbeddedServiceListener()
{
	UnregisterWithService();
}

void CPlatformEventManager::SEmbeddedServiceListener::RegisterWithService()
{
	CRY_ASSERT(m_pManager != nullptr);
	CRY_ASSERT(m_platform != EGamePlatform::None);
	CRY_ASSERT(m_platform != EGamePlatform::Main);

	if (IService* pService = Helper::GetService(m_platform))
	{
		string name;
		name.Format("SEmbeddedServiceListener[%s]", Helper::GetServiceName(m_platform));
		pService->AddListener(*this, name);

		if (ILeaderboards* pLeaderboards = pService->GetLeaderboards())
		{
			pLeaderboards->AddListener(*this, name);
		}

		if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
		{
			pMatchmaking->AddListener(*this, name);
		}

		if (IRemoteStorage* pRemoteStorage = pService->GetRemoteStorage())
		{
			if (IUserGeneratedContentManager* pUGCManager = pRemoteStorage->GetUserGeneratedContentManager())
			{
				pUGCManager->AddListener(*this, name);
			}
		}
	}
	else
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not register as a game platform listener for platform '%s'.", Helper::GetServiceName(m_platform));
	}
}

void CPlatformEventManager::SEmbeddedServiceListener::UnregisterWithService()
{
	if (IService* pService = Helper::GetService(m_platform))
	{
		if (IRemoteStorage* pRemoteStorage = pService->GetRemoteStorage())
		{
			if (IUserGeneratedContentManager* pUGCManager = pRemoteStorage->GetUserGeneratedContentManager())
			{
				pUGCManager->RemoveListener(*this);
			}
		}

		if (ILeaderboards* pLeaderboards = pService->GetLeaderboards())
		{
			pLeaderboards->RemoveListener(*this);
		}

		if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
		{
			pMatchmaking->RemoveListener(*this);
		}

		pService->RemoveListener(*this);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Service events
//

void CPlatformEventManager::SEmbeddedServiceListener::OnAccountAdded(IAccount& account)
{
	m_pManager->HandleEvent(COnAccountAdded(m_platform, account.GetIdentifier()));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnAccountNameChanged(const AccountIdentifier& accountId, const char* szName)
{
	m_pManager->HandleEvent(COnAccountNameChanged(m_platform, accountId, szName));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnAccountRemoved(IAccount& account)
{
	m_pManager->HandleEvent(COnAccountRemoved(m_platform, account.GetIdentifier()));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnAuthTokenReceived(bool success, const char* szToken)
{
	m_pManager->HandleEvent(COnAuthTokenReceived(m_platform, success, szToken));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnAvatarImageLoaded(const AccountIdentifier& accountId)
{
	m_pManager->HandleEvent(COnAvatarImageLoaded(m_platform, accountId));
}
void CPlatformEventManager::SEmbeddedServiceListener::OnCatalogItemsReceived(bool success, const DynArray<IService::SStoreItemDetails>& items)
{
	m_pManager->HandleEvent(COnCatalogItemsReceived(m_platform, success, items));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnCatalogReceived(bool success, const DynArray<IService::SStoreItem>& items, bool remainingItemsInCatalog)
{
	m_pManager->HandleEvent(COnCatalogReceived(m_platform, success, items, remainingItemsInCatalog));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnEnvironmentVariableChanged(const char* szVarName, const char* szNewValue)
{
	m_pManager->HandleEvent(COnEnvironmentVariableChanged(m_platform, szVarName, szNewValue));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnGetSteamAuthTicketResponse(bool success, uint32 authTicket)
{
	m_pManager->HandleEvent(COnGetSteamAuthTicketResponse(m_platform, success, authTicket));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnInventoryItemConsumed(bool success, const InventoryItemIdentifier& id, uint32 quantity)
{
	m_pManager->HandleEvent(COnInventoryItemConsumed(m_platform, success, id, quantity));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnInventoryReceived(bool success, const DynArray<IService::SInventoryItem>& items)
{
	m_pManager->HandleEvent(COnInventoryReceived(m_platform, success, items));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnLicensesChanged(const DynArray<InventoryItemIdentifier>& newLicenses)
{
	m_pManager->HandleEvent(COnLicensesChanged(m_platform, newLicenses));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnLogInStateChanged(const AccountIdentifier& accountId, bool isLoggedIn)
{
	m_pManager->HandleEvent(COnLogInStateChanged(m_platform, accountId, isLoggedIn));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnMicroTransactionAuthorizationResponse(const TransactionIdentifier& orderId, bool authorized)
{
	m_pManager->HandleEvent(COnMicroTransactionAuthorizationResponse(m_platform, orderId, authorized));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnNetworkStatusChanged(const EConnectionStatus& connectionStatus)
{
	m_pManager->HandleEvent(COnNetworkStatusChanged(m_platform, connectionStatus));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnOverlayActivated(const ServiceIdentifier& serviceId, bool active)
{
	m_pManager->HandleEvent(COnOverlayActivated(m_platform, active));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnPersonaStateChanged(const IAccount& account, CEnumFlags<EPersonaChangeFlags> changeFlags)
{
	m_pManager->HandleEvent(COnPersonaStateChanged(m_platform, account.GetIdentifier(), changeFlags));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnPlatformServiceError(const AccountIdentifier& accountId, int32 errorCode)
{
	m_pManager->HandleEvent(COnPlatformServiceError(m_platform, accountId, errorCode));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnShutdown(const ServiceIdentifier& serviceId)
{
	m_pManager->HandleEvent(COnShutdown(m_platform));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnUserPermissionChanged(const AccountIdentifier& accountId, IService::EPermission permission, bool hasPermission)
{
	m_pManager->HandleEvent(COnUserPermissionChanged(m_platform, accountId, permission, hasPermission));
}

///////////////////////////////////////////////////////////////////////////////////////
// Leaderboard events
//

void CPlatformEventManager::SEmbeddedServiceListener::OnEntryDownloaded(const LeaderboardIdentifier& leaderboardId, const char* szPlayerName, int rank, int score, ILeaderboards::EScoreType scoreType, bool bCurrentUser)
{
	m_pManager->HandleEvent(COnLeaderboardEntryDownloaded(m_platform, leaderboardId, szPlayerName, rank, score, scoreType, bCurrentUser));
}

///////////////////////////////////////////////////////////////////////////////////////
// Matchmaking events
//

void CPlatformEventManager::SEmbeddedServiceListener::OnLobbyQueryComplete(int numLobbiesFound)
{
	m_pManager->HandleEvent(COnLobbyQueryComplete(m_platform, numLobbiesFound));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnCreatedLobby(IUserLobby* pLobby)
{
	m_pManager->HandleEvent(COnCreatedLobby(m_platform, pLobby->GetIdentifier()));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnJoinedLobby(IUserLobby* pLobby)
{
	m_pManager->HandleEvent(COnJoinedLobby(m_platform, pLobby->GetIdentifier()));
}

void CPlatformEventManager::SEmbeddedServiceListener::OnLobbyJoinFailed(const LobbyIdentifier& lobbyId)
{
	m_pManager->HandleEvent(COnLobbyJoinFailed(m_platform, lobbyId));
}

bool CPlatformEventManager::SEmbeddedServiceListener::OnPreJoinLobby(const LobbyIdentifier& lobbyId)
{
	m_pManager->HandleEvent(COnPreJoinLobby(m_platform, lobbyId));

	// Currently not able to retrieve a result from sending any events
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// Remote File events
//

bool CPlatformEventManager::SEmbeddedServiceListener::OnFileShared(IRemoteFile* pFile)
{
	m_pManager->HandleEvent(COnFileShared(m_platform, pFile));
	return true;
}

bool CPlatformEventManager::SEmbeddedServiceListener::OnDownloadedSharedFile(ISharedRemoteFile* pFile)
{
	m_pManager->HandleEvent(COnDownloadedSharedFile(m_platform, pFile));
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// User Generated Content events
//

void CPlatformEventManager::SEmbeddedServiceListener::OnContentCreated(IUserGeneratedContent* pContent, bool bRequireLegalAgreementAccept)
{
	m_pManager->HandleEvent(COnContentCreated(m_platform, CUserGeneratedContent(pContent), bRequireLegalAgreementAccept));
}

inline IUserLobby* GetUserLobby(const LobbyIdentifier& lobbyId)
{
	IUserLobby* pUserLobby = nullptr;

	if (IService* pService = Helper::GetService(lobbyId.Service()))
	{
		if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
		{
			pUserLobby = pMatchmaking->GetLobbyById(lobbyId);
		}
	}

	return pUserLobby;
}

CPlatformEventManager::SEmbeddedLobbyListener::SEmbeddedLobbyListener(CPlatformEventManager* pManager, const LobbyIdentifier& lobbyId)
	: m_pManager(pManager)
	, m_lobbyId(lobbyId)
{
	CRY_ASSERT(pManager != nullptr);

	if (IUserLobby* pUserLobby = GetUserLobby(m_lobbyId))
	{
		pUserLobby->AddListener(*this, "CEmbeddedLobbyListener");
	}
	else
	{
		string lobbyIdStr;
		lobbyId.GetAsString(lobbyIdStr);
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not register as a game platform lobby listener for lobby '%s'.", lobbyIdStr.c_str());
	}
}

CPlatformEventManager::SEmbeddedLobbyListener::~SEmbeddedLobbyListener()
{
	if (IUserLobby* pUserLobby = GetUserLobby(m_lobbyId))
	{
		pUserLobby->RemoveListener(*this);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// User Lobby events
//

void CPlatformEventManager::SEmbeddedLobbyListener::OnPlayerEntered(const AccountIdentifier& accountId)
{
	m_pManager->HandleEvent(COnPlayerEntered(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnPlayerLeft(const AccountIdentifier& accountId)
{
	m_pManager->HandleEvent(COnPlayerLeft(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnPlayerDisconnected(const AccountIdentifier& accountId)
{
	m_pManager->HandleEvent(COnPlayerDisconnected(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnPlayerKicked(const AccountIdentifier& accountId, const AccountIdentifier& moderatorId)
{
	m_pManager->HandleEvent(COnPlayerKicked(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId, moderatorId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnPlayerBanned(const AccountIdentifier& accountId, const AccountIdentifier& moderatorId)
{
	m_pManager->HandleEvent(COnPlayerBanned(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId, moderatorId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnLeave()
{
	m_pManager->HandleEvent(COnLeave(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnGameCreated(IServer::Identifier serverId, uint32 ipAddress, uint16 port, bool bLocal)
{
	m_pManager->HandleEvent(COnGameCreated(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, serverId, ipAddress, port, bLocal));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnChatMessage(const AccountIdentifier& accountId, const char* szMessage)
{
	m_pManager->HandleEvent(COnChatMessage(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId, szMessage));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnLobbyDataUpdate(const LobbyIdentifier& targetLobbyId)
{
	m_pManager->HandleEvent(COnLobbyDataUpdate(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId));
}

void CPlatformEventManager::SEmbeddedLobbyListener::OnUserDataUpdate(const AccountIdentifier& accountId)
{
	m_pManager->HandleEvent(COnUserDataUpdate(Helper::GetServiceEnum(m_lobbyId.Service()), m_lobbyId, accountId));
}

inline IRemoteStorageCache* GetRemoteStorageCache()
{
	if (IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
	{
		return pPlugin->GetRemoteStorageCache();
	}

	return nullptr;
}

void CPlatformEventManager::TryCleanContainers()
{
	auto it = m_platformContainers.begin();
	while (it != m_platformContainers.end())
	{
		if (it->m_externalListeners.Empty() && !it->m_externalListeners.IsNotifying())
		{
			it = m_platformContainers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool CPlatformEventManager::ContainerExists(EGamePlatform platform) const
{
	for (const auto& element : m_platformContainers)
		if (Helper::IsMatchingService(element.m_platform, platform))
			return true;

	return false;
}

void CPlatformEventManager::ExecuteForPlatform(EGamePlatform platform, std::function<void(SPlatformContainer&)> executor)
{
	for (auto& container : m_platformContainers)
	{
		if (Helper::IsMatchingService(container.m_platform, platform))
		{
			executor(container);
			break;
		}
	}
}

void CPlatformEventManager::HandleEvent(const CBaseEventContext& context)
{
	ExecuteForPlatform(context.GetPlatform(), [&context](SPlatformContainer& element)
	{
		for (TListenerSet::Notifier notifier(element.m_externalListeners); notifier.IsValid(); notifier.Next())
		{
			if (notifier->GetPlatformEventMask().Check(static_cast<size_t>(context.GetEvent())) && notifier.IsValid()) // <- IsValid required for every de-reference of the notifier
			{
				notifier->OnPlatformEvent(context);
			}
		}
	});
}

void CPlatformEventManager::AddListener(EGamePlatform platform, IPlatformEventListener* pListener)
{
	CRY_ASSERT(platform != EGamePlatform::None);

	if (!ContainerExists(platform))
	{
		if (platform == EGamePlatform::Main)
		{
			IService* pService = Helper::GetMainService();
			if (!pService)
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[GamePlatform] Cannot create platform listener for the main service. Main service not found.");
				return;
			}
			else
			{
				m_platformContainers.emplace_back(this, Helper::GetServiceEnum(pService->GetServiceIdentifier()));
			}
		}
		else
		{
			m_platformContainers.emplace_back(this, platform);
		}
	}

	ExecuteForPlatform(platform, [pListener](SPlatformContainer& element)
	{
		element.m_externalListeners.Add(pListener);
	});
}

void CPlatformEventManager::RemoveListener(EGamePlatform platform, IPlatformEventListener* pListener)
{
	CRY_ASSERT(platform != EGamePlatform::None);

	ExecuteForPlatform(platform, [pListener](SPlatformContainer& element)
	{
		element.m_externalListeners.Remove(pListener);
	});

	TryCleanContainers();
}

void CPlatformEventManager::StartListening(const LobbyIdentifier& lobbyId)
{
	ExecuteForPlatform(Helper::GetServiceEnum(lobbyId.Service()), [this, &lobbyId](SPlatformContainer& element)
	{
		element.m_embeddedLobbyListeners.emplace_back(this, lobbyId);
	});
}

void CPlatformEventManager::StopListening(const LobbyIdentifier& lobbyId)
{
	ExecuteForPlatform(Helper::GetServiceEnum(lobbyId.Service()), [&lobbyId](SPlatformContainer& element)
	{
		for (auto it = element.m_embeddedLobbyListeners.begin(); it != element.m_embeddedLobbyListeners.end(); it++)
		{
			if (it->GetLobbyId() == lobbyId)
			{
				element.m_embeddedLobbyListeners.erase(it);
				break;
			}
		}
	});
}

void CPlatformEventManager::StartListening(const CCachedLocal& cache)
{
	ExecuteForPlatform(cache.GetPlatform(), [&cache](SPlatformContainer& element)
	{
		cache.GetPtr()->AddListener(element.m_embeddedServiceListener, "CPlatformEventManager");
	});
}

void CPlatformEventManager::StopListening(const CCachedLocal& cache)
{
	ExecuteForPlatform(cache.GetPlatform(), [&cache](SPlatformContainer& element)
	{
		cache.GetPtr()->RemoveListener(element.m_embeddedServiceListener);
	});
}

void CPlatformEventManager::StartListening(const CCachedShared& cache)
{
	ExecuteForPlatform(cache.GetPlatform(), [&cache](SPlatformContainer& element)
	{
		cache.GetPtr()->AddListener(element.m_embeddedServiceListener, "CPlatformEventManager");
	});
}

void CPlatformEventManager::StopListening(const CCachedShared& cache)
{
	ExecuteForPlatform(cache.GetPlatform(), [&cache](SPlatformContainer& element)
	{
		cache.GetPtr()->RemoveListener(element.m_embeddedServiceListener);
	});
}

} // Cry::GamePlatform
