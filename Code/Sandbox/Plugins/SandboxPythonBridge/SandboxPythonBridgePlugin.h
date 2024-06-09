// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <IPlugin.h>

class SandboxPythonBridgePlugin : public IPlugin
{
public:
	SandboxPythonBridgePlugin();
	~SandboxPythonBridgePlugin();

	virtual int32 GetPluginVersion() override;
	virtual const char* GetPluginName() override;
	virtual const char* GetPluginDescription() override;
};
