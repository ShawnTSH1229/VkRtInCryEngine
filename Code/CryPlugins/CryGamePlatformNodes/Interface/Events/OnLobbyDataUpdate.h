// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnLobbyDataUpdate : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLobbyDataUpdate;

	COnLobbyDataUpdate()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
	{}

	COnLobbyDataUpdate(EGamePlatform platform, const LobbyIdentifier& lobbyId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLobbyDataUpdate>& typeDesc)
	{
		typeDesc.SetGUID("{429301F9-C732-429C-842B-4B8F4C3CA200}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnLobbyDataUpdate");
		typeDesc.AddMember(&COnLobbyDataUpdate::m_lobbyId, 'lid', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

private:

	LobbyIdentifier m_lobbyId;

};

} // Cry::GamePlatform