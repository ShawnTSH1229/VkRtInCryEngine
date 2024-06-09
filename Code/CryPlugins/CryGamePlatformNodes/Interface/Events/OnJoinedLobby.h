// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnJoinedLobby : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnJoinedLobby;

	COnJoinedLobby()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
	{}

	COnJoinedLobby(EGamePlatform platform, const LobbyIdentifier& lobbyId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnJoinedLobby>& typeDesc)
	{
		typeDesc.SetGUID("{9616776D-F6D3-4FAD-9857-8164610892DB}"_cry_guid);
		typeDesc.SetLabel("Matchmaking::OnJoinedLobby");
		typeDesc.AddMember(&COnJoinedLobby::m_lobbyId, 'lid', "lobby", "User Lobby", nullptr, LobbyIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

private:

	LobbyIdentifier m_lobbyId;

};

} // Cry::GamePlatform