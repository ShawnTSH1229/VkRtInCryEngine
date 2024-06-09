// Copyright 2011-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <IPlugin.h>

class CFBXPlugin : public IPlugin
{
public:
	CFBXPlugin();
	~CFBXPlugin();

	int32       GetPluginVersion() override;
	const char* GetPluginName() override;
	const char* GetPluginDescription() override;
};
