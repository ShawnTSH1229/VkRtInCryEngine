// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnPreJoinLobby : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPreJoinLobby;

	COnPreJoinLobby()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
	{}

	COnPreJoinLobby(EGamePlatform platform, const LobbyIdentifier& lobbyId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPreJoinLobby>& typeDesc)
	{
		typeDesc.SetGUID("{36089FF9-9E5C-47B2-BD9F-7B21AFB326FE}"_cry_guid);
		typeDesc.SetLabel("Matchmaking::OnPreJoinLobby");
		typeDesc.AddMember(&COnPreJoinLobby::m_lobbyId, 'lid', "lobby", "User Lobby", nullptr, LobbyIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

private:

	LobbyIdentifier m_lobbyId;

};

} // Cry::GamePlatform