// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "PlatformSignalComponent.h"

#include <IPlugin.h>
#include <Types/DynArray.h>
#include <Types/Platform.h>
#include <Types/Service.h>
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

#include <CryCore/StaticInstanceList.h>
#include <CryEntitySystem/IEntityClass.h>
#include <CrySchematyc/IObject.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>

namespace Cry::GamePlatform
{

template<class EVENT>
inline void CallSignal(Schematyc::IObject* pSchematycObject, const CBaseEventContext& context, const CryGUID& senderGUID)
{
	CRY_ASSERT(context.GetEvent() == EVENT::EventType);

	pSchematycObject->ProcessSignal(static_cast<const EVENT&>(context), senderGUID);
}

CPlatformSignalComponent::~CPlatformSignalComponent()
{
	UnregisterListeners();
}

void CPlatformSignalComponent::OnPlatformEvent(const CBaseEventContext& context)
{
	if (!m_isEnabled)
		return;

	if (Schematyc::IObject* pSchematycObject = m_pEntity->GetSchematycObject())
	{
		switch (context.GetEvent())
		{
		case EPlatformEvent::OnAccountAdded:                          CallSignal<COnAccountAdded>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnAccountNameChanged:                    CallSignal<COnAccountNameChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnAccountRemoved:                        CallSignal<COnAccountRemoved>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnAuthTokenReceived:                     CallSignal<COnAuthTokenReceived>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnAvatarImageLoaded:                     CallSignal<COnAvatarImageLoaded>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnCatalogItemsReceived:                  CallSignal<COnCatalogItemsReceived>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnCatalogReceived:                       CallSignal<COnCatalogReceived>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnEnvironmentVariableChanged:            CallSignal<COnEnvironmentVariableChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnGetSteamAuthTicketResponse:            CallSignal<COnGetSteamAuthTicketResponse>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnInventoryItemConsumed:                 CallSignal<COnInventoryItemConsumed>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnInventoryReceived:                     CallSignal<COnInventoryReceived>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLicensesChanged:                       CallSignal<COnLicensesChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLogInStateChanged:                     CallSignal<COnLogInStateChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnMicroTransactionAuthorizationResponse: CallSignal<COnMicroTransactionAuthorizationResponse>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnNetworkStatusChanged:                  CallSignal<COnNetworkStatusChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnOverlayActivated:                      CallSignal<COnOverlayActivated>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPersonaStateChanged:                   CallSignal<COnPersonaStateChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPlatformServiceError:                  CallSignal<COnPlatformServiceError>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnShutdown:                              CallSignal<COnShutdown>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnUserPermissionChanged:                 CallSignal<COnUserPermissionChanged>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLeaderboardEntryDownloaded:            CallSignal<COnLeaderboardEntryDownloaded>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLobbyQueryComplete:                    CallSignal<COnLobbyQueryComplete>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnCreatedLobby:                          CallSignal<COnCreatedLobby>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnJoinedLobby:                           CallSignal<COnJoinedLobby>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLobbyJoinFailed:                       CallSignal<COnLobbyJoinFailed>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPreJoinLobby:                          CallSignal<COnPreJoinLobby>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPlayerEntered:                         CallSignal<COnPlayerEntered>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPlayerLeft:                            CallSignal<COnPlayerLeft>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPlayerDisconnected:                    CallSignal<COnPlayerDisconnected>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPlayerKicked:                          CallSignal<COnPlayerKicked>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnPlayerBanned:                          CallSignal<COnPlayerBanned>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLeave:                                 CallSignal<COnLeave>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnGameCreated:                           CallSignal<COnGameCreated>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnChatMessage:                           CallSignal<COnChatMessage>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnLobbyDataUpdate:                       CallSignal<COnLobbyDataUpdate>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnUserDataUpdate:                        CallSignal<COnUserDataUpdate>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnFileShared:                            CallSignal<COnFileShared>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnDownloadedSharedFile:                  CallSignal<COnDownloadedSharedFile>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnContentCreated:                        CallSignal<COnContentCreated>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnTextFiltered:                          CallSignal<COnTextFiltered>(pSchematycObject, context, GetGUID()); break;
		case EPlatformEvent::OnCheckMultiplayerAccess:                CallSignal<COnCheckMultiplayerAccess>(pSchematycObject, context, GetGUID()); break;
		}
	}
}

CPlatformEventFlags CPlatformSignalComponent::GetPlatformEventMask() const
{
	return m_events;
}

void CPlatformSignalComponent::StartLobbyListener(const LobbyIdentifier& lobbyId)
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		if (m_registeredLobbyListeners.insert(lobbyId).second)
		{
			pEventManager->StartListening(lobbyId);
		}
	}
}

void CPlatformSignalComponent::StopLobbyListener(const LobbyIdentifier& lobbyId)
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		pEventManager->StopListening(lobbyId);
		m_registeredLobbyListeners.erase(lobbyId);
	}
}

void CPlatformSignalComponent::StartRemoteFileListener(const CCachedLocal& localCache)
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		if (m_registeredRemoteFileListeners.insert(localCache).second)
		{
			pEventManager->StartListening(localCache);
		}
	}
}

void CPlatformSignalComponent::StopRemoteFileListener(const CCachedLocal& localCache)
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		pEventManager->StopListening(localCache);
		m_registeredRemoteFileListeners.erase(localCache);
	}
}

void CPlatformSignalComponent::StartSharedFileListener(const CCachedShared& sharedCache)
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		if (m_registeredSharedFileListeners.insert(sharedCache).second)
		{
			pEventManager->StartListening(sharedCache);
		}
	}
}

void CPlatformSignalComponent::StopSharedFileListener(const CCachedShared& sharedCache)
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		pEventManager->StopListening(sharedCache);
		m_registeredSharedFileListeners.erase(sharedCache);
	}
}

void CPlatformSignalComponent::SetEnabled(const bool enable)
{
	m_isEnabled = enable;
}

void CPlatformSignalComponent::SetPlatform(EGamePlatform platform)
{
	if (platform == m_platform)
		return;

	if (platform == EGamePlatform::None)
		return;

	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		UnregisterListeners();

		m_platform = platform;
		pEventManager->AddListener(m_platform, this);
	}
}

void CPlatformSignalComponent::SetPlatformEventMask(const CPlatformEventFlags& events)
{
	m_events = events;
}

void CPlatformSignalComponent::Initialize()
{
	if (m_platform == EGamePlatform::None)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Platform Signal Component with Platform=NONE detected in entity '%s'.", GetEntity()->GetClass()->GetName());
	}
}

EntityEventMask CPlatformSignalComponent::GetEventMask() const
{
	return EntityEventMask(EEntityEvent::GameplayStarted);
}

void CPlatformSignalComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEntityEvent::GameplayStarted:
		{
			if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
			{
				pEventManager->AddListener(m_platform, this);
			}
		} break;
	}
}

void CPlatformSignalComponent::UnregisterListeners()
{
	if (auto pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
	{
		for (auto& lobbyId : m_registeredLobbyListeners)
		{
			pEventManager->StopListening(lobbyId);
		}
		m_registeredLobbyListeners.clear();

		for (const CCachedLocal& localCache : m_registeredRemoteFileListeners)
		{
			pEventManager->StopListening(localCache);
		}
		m_registeredRemoteFileListeners.clear();

		for (const CCachedShared& sharedCache : m_registeredSharedFileListeners)
		{
			pEventManager->StopListening(sharedCache);
		}
		m_registeredSharedFileListeners.clear();

		pEventManager->RemoveListener(m_platform, this);
	}
}

void RegisterPlatformSignalComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope entityScope = registrar.Scope(IEntity::GetEntityScopeGUID());

	Schematyc::CEnvRegistrationScope componentScope = entityScope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlatformSignalComponent));
	{
		// Signals
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnAccountAdded));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnAccountNameChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnAccountRemoved));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnAuthTokenReceived));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnAvatarImageLoaded));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnCatalogItemsReceived));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnCatalogReceived));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnEnvironmentVariableChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnGetSteamAuthTicketResponse));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnInventoryItemConsumed));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnInventoryReceived));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLicensesChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLogInStateChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnMicroTransactionAuthorizationResponse));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnNetworkStatusChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnOverlayActivated));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPersonaStateChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPlatformServiceError));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnShutdown));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnUserPermissionChanged));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLeaderboardEntryDownloaded));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLobbyQueryComplete));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnCreatedLobby));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnJoinedLobby));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLobbyJoinFailed));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPreJoinLobby));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPlayerEntered));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPlayerLeft));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPlayerDisconnected));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPlayerKicked));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnPlayerBanned));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLeave));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnGameCreated));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnChatMessage));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnLobbyDataUpdate));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnUserDataUpdate));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnFileShared));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnDownloadedSharedFile));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnContentCreated));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnTextFiltered));
		componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(COnCheckMultiplayerAccess));
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::StartLobbyListener, "{D69A23E3-DE11-4004-8EEC-2C0BECE2284C}"_cry_guid, "StartLobbyListener");
		pFunction->BindInput(1, 'lob', "Lobby", "Lobby to listen to lobby events for.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::StopLobbyListener, "{B202A8EE-A22D-449E-9C19-D1A2A18D1C0F}"_cry_guid, "StopLobbyListener");
		pFunction->BindInput(1, 'lob', "Lobby", "Lobby to stop listening to lobby events for.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::StartRemoteFileListener, "{53390512-E63A-49F3-9CAF-7988FC9A0F22}"_cry_guid, "StartRemoteFileListener");
		pFunction->BindInput(1, 'rfil', "RemoteFile", "Remote file to listen to events for.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::StopRemoteFileListener, "{A65E3DA2-52A2-44FA-895D-9C361CE777D2}"_cry_guid, "StopRemoteFileListener");
		pFunction->BindInput(1, 'rfil', "RemoteFile", "Remote file to stop listening to events for.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::StartSharedFileListener, "{6BDC6325-84E7-4E65-A9E2-FE1B53241C88}"_cry_guid, "StartSharedFileListener");
		pFunction->BindInput(1, 'sfid', "SharedFile", "The shared file to listen to events for.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::StopSharedFileListener, "{4DE133F9-95EB-4E2C-96AD-A42DF5FB8027}"_cry_guid, "StopSharedFileListener");
		pFunction->BindInput(1, 'sfid', "SharedFile", "The shared file to stop listening to events for.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::SetEnabled, "{DDB17D6F-CFA6-4FC7-8A14-36CF6609359A}"_cry_guid, "SetEnabled");
		pFunction->BindInput(1, 'enab', "Enable", "True to enable listeners, false to disable (ignores events).");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::SetPlatform, "{F6F88BB0-C168-4E0F-820B-1EC318C4829E}"_cry_guid, "SetPlatform");
		pFunction->BindInput(1, 'plat', "Platform", "Sets the platform this component is listening to. (Cannot be 'None').");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::GetPlatformEventMask, "{B15156BF-9A65-486E-820D-ED258F590F3A}"_cry_guid, "GetEvents");
		pFunction->BindOutput(0, 'evts', "Events", "Gets the events from the listener component.");
		componentScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlatformSignalComponent::SetPlatformEventMask, "{8C3A4807-44FE-44DB-B9F2-4F1916B93D17}"_cry_guid, "SetEvents");
		pFunction->BindInput(1, 'evts', "Events", "Sets the events on the listener component.");
		componentScope.Register(pFunction);
	}
}
} // Cry::GamePlatform
