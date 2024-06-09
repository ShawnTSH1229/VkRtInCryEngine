// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/PlatformTypes.h>

namespace Cry::GamePlatform
{

class COnNetworkStatusChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnNetworkStatusChanged;

	COnNetworkStatusChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_connectionStatus(EConnectionStatus::Disconnected)
	{}

	COnNetworkStatusChanged(EGamePlatform platform, EConnectionStatus connectionStatus)
		: CBaseEventContext(platform, EventType)
		, m_connectionStatus(connectionStatus)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnNetworkStatusChanged>& typeDesc)
	{
		typeDesc.SetGUID("{E501E92B-3C6E-4BA5-81A9-58C7530B1BFE}"_cry_guid);
		typeDesc.SetLabel("Networking::OnNetworkStatusChanged");
		typeDesc.AddMember(&COnNetworkStatusChanged::m_connectionStatus, 'stat', "connectionstatus", "Connection Status", nullptr, EConnectionStatus::Disconnected);
	}

	EConnectionStatus GetConnectionStatus() const
	{
		return m_connectionStatus;
	}

private:

	EConnectionStatus m_connectionStatus;

};

} // Cry::GamePlatform