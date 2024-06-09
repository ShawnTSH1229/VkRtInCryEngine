// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnPlayerEntered : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPlayerEntered;

	COnPlayerEntered()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
	{}

	COnPlayerEntered(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPlayerEntered>& typeDesc)
	{
		typeDesc.SetGUID("{392A1BE2-BD92-49D5-9744-67C17B2EEA84}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnPlayerEntered");
		typeDesc.AddMember(&COnPlayerEntered::m_lobbyId, 'lid', "lobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnPlayerEntered::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
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