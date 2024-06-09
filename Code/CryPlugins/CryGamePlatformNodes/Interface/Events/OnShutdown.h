// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnShutdown : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnShutdown;

	COnShutdown()
		: CBaseEventContext(EGamePlatform::Main, EventType)
	{}

	COnShutdown(EGamePlatform platform)
		: CBaseEventContext(platform, EventType)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnShutdown>& typeDesc)
	{
		typeDesc.SetGUID("{7F2E255C-4C08-40E7-962B-06DA3C16C736}"_cry_guid);
		typeDesc.SetLabel("Service::OnShutdown");
	}
};

} // Cry::GamePlatform