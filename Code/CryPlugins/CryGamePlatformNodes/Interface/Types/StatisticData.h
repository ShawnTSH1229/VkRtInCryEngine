// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Platform.h"
#include "StatisticValue.h"

namespace Cry::GamePlatform
{

struct SStatisticData
{

	// Unique statistic GUID
	CryGUID guid;

	// Label designed for editor display
	const char* szEditorLabel;

	// Label designed for runtime display (Localized string reference for example)
	const char* szLabel;

	// Description designed for display (Localized string reference for example)
	const char* szDesc;

	// Platform API specific Name (Should be unique in that platform)
	const char* szApiName;

	// The specific platform this achievement belongs to
	EGamePlatform platform;

	// The type of the statistic (Float or Integer)
	EStatDataType statDataType;

};

} // Cry::GamePlatform
