// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnChatMessage : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnChatMessage;

	COnChatMessage()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_accountId()
		, m_message("")
	{}

	COnChatMessage(EGamePlatform platform, const LobbyIdentifier& lobbyId, const AccountIdentifier& accountId, const char* szMessage)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_accountId(accountId)
		, m_message(szMessage)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnChatMessage>& typeDesc)
	{
		typeDesc.SetGUID("{802A2FCF-BC81-460D-884D-323C9F24553F}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnChatMessage");
		typeDesc.AddMember(&COnChatMessage::m_lobbyId, 'ldtl', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnChatMessage::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnChatMessage::m_message, 'msg', "message", "Message", nullptr, "");
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	const Schematyc::CSharedString& GetChatMessage() const
	{
		return m_message;
	}

private:

	LobbyIdentifier          m_lobbyId;
	AccountIdentifier        m_accountId;
	Schematyc::CSharedString m_message;

};

} // Cry::GamePlatform