// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnLeave : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLeave;

	COnLeave()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
	{}

	COnLeave(EGamePlatform platform, const LobbyIdentifier& lobbyId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLeave>& typeDesc)
	{
		typeDesc.SetGUID("{99B4B759-B4EB-4D87-8B12-13D78B98975A}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnLeave");
		typeDesc.AddMember(&COnLeave::m_lobbyId, 'lid', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

private:

	LobbyIdentifier m_lobbyId;

};

} // Cry::GamePlatform