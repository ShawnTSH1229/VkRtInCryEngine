// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnPlayerDisconnected : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPlayerDisconnected;

	COnPlayerDisconnected()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
	{}

	COnPlayerDisconnected(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPlayerDisconnected>& typeDesc)
	{
		typeDesc.SetGUID("{F4BB7AA4-BAC2-4335-B130-4F89092E9EAF}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnPlayerDisconnected");
		typeDesc.AddMember(&COnPlayerDisconnected::m_lobbyId, 'lid', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnPlayerDisconnected::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
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