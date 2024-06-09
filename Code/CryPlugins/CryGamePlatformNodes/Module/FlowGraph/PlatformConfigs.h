// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>

namespace Cry::GamePlatform
{

#define FLOWGRAPH_PLATFORM_UICONFIG "enum_int:Main=0,Steam=1,Playstation=2,Xbox=3,Discord=4"

static inline EGamePlatform UIConfigToPlatform(int uicValue)
{
	switch (uicValue)
	{
	case 1:
		return EGamePlatform::Steam;
	case 2:
		return EGamePlatform::Playstation;
	case 3:
		return EGamePlatform::Xbox;
	case 4:
		return EGamePlatform::Discord;
	}

	return EGamePlatform::Main;
}

static inline int PlatformToUIConfig(EGamePlatform platform)
{
	switch (platform)
	{
	case EGamePlatform::Steam:
		return 1;
	case EGamePlatform::Playstation:
		return 2;
	case EGamePlatform::Xbox:
		return 3;
	case EGamePlatform::Discord:
		return 4;
	}

	return 0;
}

} // Cry::GamePlatform
