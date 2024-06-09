// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnUserDataUpdate : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnUserDataUpdate;

	COnUserDataUpdate()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
	{}

	COnUserDataUpdate(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnUserDataUpdate>& typeDesc)
	{
		typeDesc.SetGUID("{1977619A-3215-415F-AC7C-873D26E4637F}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnUserDataUpdate");
		typeDesc.AddMember(&COnUserDataUpdate::m_lobbyId, 'ldtl', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnUserDataUpdate::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
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