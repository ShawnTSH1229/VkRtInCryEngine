// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnPlayerLeft : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPlayerLeft;

	COnPlayerLeft()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
	{}

	COnPlayerLeft(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPlayerLeft>& typeDesc)
	{
		typeDesc.SetGUID("{44AB2ED0-EAA3-4736-92D5-CA69EEF98C52}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnPlayerLeft");
		typeDesc.AddMember(&COnPlayerLeft::m_lobbyId, 'lid', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnPlayerLeft::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

private:

	LobbyIdentifier   m_lobbyId;
	AccountIdentifier m_accountId;

};

} // Cry::GamePlatform