// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Platform.h"

namespace Cry::GamePlatform
{

struct SAchievementData
{

	// Unique achievement GUID
	CryGUID guid;

	// Label designed for editor display
	const char* szEditorLabel;

	// Label designed for runtime display (Localized string reference for example)
	const char* szLabel;

	// Description designed for display (Localized string reference for example)
	const char* szDesc;

	// Platform API specific Name (Should be unique in that platform)
	const char* szApiName;

	// Platform API specific Identifier Number (Should be unique in that platform)
	int apiId;

	// Used to clamp the progress value set when setting achievement progress (Should come from the platform)
	int minProgress;
	int maxProgress;

	// The specific platform this achievement belongs to
	EGamePlatform platform;

};

} // Cry::GamePlatform
