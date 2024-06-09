// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnPlayerKicked : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPlayerKicked;

	COnPlayerKicked()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
		, m_moderatorId()
	{}

	COnPlayerKicked(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId, const AccountIdentifier& moderatorId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
		, m_moderatorId(moderatorId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPlayerKicked>& typeDesc)
	{
		typeDesc.SetGUID("{E4AE83B6-65C0-4551-8856-2B8AF28EAA23}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnPlayerKicked");
		typeDesc.AddMember(&COnPlayerKicked::m_lobbyId, 'lid', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnPlayerKicked::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnPlayerKicked::m_moderatorId, 'mid', "moderator", "Moderator", nullptr, AccountIdentifier());
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	const AccountIdentifier& GetModeratorId() const
	{
		return m_moderatorId;
	}

private:

	LobbyIdentifier   m_lobbyId;
	AccountIdentifier m_accountId;
	AccountIdentifier m_moderatorId;

};

} // Cry::GamePlatform