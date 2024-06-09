// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnGameCreated : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnGameCreated;

	COnGameCreated()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_lobbyId()
		, m_serverId()
		, m_ipAddress(0)
		, m_port(0)
		, m_bLocal(false)
	{}

	COnGameCreated(EGamePlatform platform, const LobbyIdentifier& lobbyId, IServer::Identifier serverId, uint32 ipAddress, uint16 port, bool bLocal)
		: CBaseEventContext(platform, EventType)
		, m_lobbyId(lobbyId)
		, m_serverId(serverId)
		, m_ipAddress(ipAddress)
		, m_port(port)
		, m_bLocal(bLocal)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnGameCreated>& typeDesc)
	{
		typeDesc.SetGUID("{93AA7BB1-42B8-4E83-84FE-048274C1DFD1}"_cry_guid);
		typeDesc.SetLabel("Lobby::OnGameCreated");
		typeDesc.AddMember(&COnGameCreated::m_lobbyId, 'ldtl', "userlobby", "User Lobby", nullptr, LobbyIdentifier());
		typeDesc.AddMember(&COnGameCreated::m_serverId, 'sid', "server", "Server", nullptr, IServer::Identifier());
		typeDesc.AddMember(&COnGameCreated::m_ipAddress, 'ipad', "ipaddress", "IP Address", nullptr, 0u);
		typeDesc.AddMember(&COnGameCreated::m_port, 'port', "port", "Port", nullptr, uint16(0));
		typeDesc.AddMember(&COnGameCreated::m_bLocal, 'iloc', "islocal", "Is Local", nullptr, uint16(0));
	}

	const LobbyIdentifier& GetLobbyId() const
	{
		return m_lobbyId;
	}

	const IServer::Identifier& GetServerId() const
	{
		return m_serverId;
	}

	uint32 GetIPAddress() const
	{
		return m_ipAddress;
	}

	uint16 GetPort() const
	{
		return static_cast<uint16>(m_port);
	}

	bool IsLocal() const
	{
		return m_bLocal;
	}

private:

	LobbyIdentifier     m_lobbyId;
	IServer::Identifier m_serverId;
	uint32              m_ipAddress;
	uint32              m_port;
	bool                m_bLocal;

};

} // Cry::GamePlatform