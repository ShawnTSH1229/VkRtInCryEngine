// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/Lobby.h>

namespace Cry::GamePlatform
{

class COnLobbyQueryComplete : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLobbyQueryComplete;

	COnLobbyQueryComplete()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_numLobbiesFound(0)
	{}

	COnLobbyQueryComplete(EGamePlatform platform, int numLobbiesFound)
		: CBaseEventContext(platform, EventType)
		, m_numLobbiesFound(numLobbiesFound)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLobbyQueryComplete>& typeDesc)
	{
		typeDesc.SetGUID("{3B15A91A-65B3-4780-8681-D6F5C5AEC3AA}"_cry_guid);
		typeDesc.SetLabel("Matchmaking::OnLobbyQueryComplete");
		typeDesc.AddMember(&COnLobbyQueryComplete::m_numLobbiesFound, 'nlob', "numlobbiesfound", "Num Lobbies Found", nullptr, 0);
	}

	int GetNumLobbies() const
	{
		return m_numLobbiesFound;
	}

private:

	int m_numLobbiesFound;

};

} // Cry::GamePlatform