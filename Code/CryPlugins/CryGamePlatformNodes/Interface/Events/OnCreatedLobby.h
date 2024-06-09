// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnCreatedLobby : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnCreatedLobby;

	COnCreatedLobby()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
	{}

	COnCreatedLobby(EGamePlatform platform, const LobbyIdentifier& lobbyId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnCreatedLobby>& typeDesc)
	{
		typeDesc.SetGUID("{243300C6-F243-4D62-8B32-860529A5DB01}"_cry_guid);
		typeDesc.SetLabel("Matchmaking::OnCreatedLobby");
		typeDesc.AddMember(&COnCreatedLobby::m_lobbyId, 'lid', "lobby", "User Lobby", nullptr, LobbyIdentifier());
	}

	const LobbyIdentifier& GetLobby() const
	{
		return m_lobbyId;
	}

private:

	LobbyIdentifier m_lobbyId;

};

} // Cry::GamePlatform