// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnLobbyJoinFailed : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLobbyJoinFailed;

	COnLobbyJoinFailed()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
	{}

	COnLobbyJoinFailed(EGamePlatform platform, const LobbyIdentifier& lobbyId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLobbyJoinFailed>& typeDesc)
	{
		typeDesc.SetGUID("{8A6B2791-4C88-48E6-86FD-D7FE85CF75EB}"_cry_guid);
		typeDesc.SetLabel("Matchmaking::OnLobbyJoinFailed");
		typeDesc.AddMember(&COnLobbyJoinFailed::m_lobbyId, 'lid', "lobby", "User Lobby", nullptr, LobbyIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

private:

	LobbyIdentifier m_lobbyId;

};

} // Cry::GamePlatform