// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>
#include <Types/PlatformEventFlags.h>

namespace Cry::GamePlatform
{

class CBaseEventContext
{
protected:
	CBaseEventContext(EGamePlatform platform, EPlatformEvent event)
		: m_platform(platform)
		, m_event(event)
	{}

public:
	EGamePlatform GetPlatform() const
	{
		return m_platform;
	}

	EPlatformEvent GetEvent() const
	{
		return m_event;
	}

protected:
	const EGamePlatform  m_platform;
	const EPlatformEvent m_event;
};

} // Cry::GamePlatform