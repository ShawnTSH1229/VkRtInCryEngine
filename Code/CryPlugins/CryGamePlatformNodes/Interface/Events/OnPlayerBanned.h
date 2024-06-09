// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnPlayerBanned : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPlayerBanned;

	COnPlayerBanned()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
		, m_moderatorId()
	{}

	COnPlayerBanned(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId, const AccountIdentifier& moderatorId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
		, m_moderatorId(moderatorId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPlayerBanned>& typeDesc)
	{
		typeDesc.SetGUID("{09652074-3475-41C4-A3C5-372EA79C8EC4}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnPlayerBanned");
		typeDesc.AddMember(&COnPlayerBanned::m_lobbyId, 'lid', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnPlayerBanned::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnPlayerBanned::m_moderatorId, 'mid', "moderator", "Moderator", nullptr, AccountIdentifier());
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