// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/ICryPlugin.h>

namespace Cry::GamePlatform
{

class IPlatformEventManager;
class IRemoteStorageCache;
class IStatisticsManager;

class IPlugin_GamePlatformNodes : public Cry::IEnginePlugin
{

public:

	CRYINTERFACE_DECLARE_GUID(IPlugin_GamePlatformNodes, "{BA64DAEE-1CDA-4970-8108-2D443ECB2B33}"_cry_guid);
	virtual ~IPlugin_GamePlatformNodes() {}

	virtual IPlatformEventManager* GetPlatformEventManager() = 0;
	virtual IRemoteStorageCache*   GetRemoteStorageCache() = 0;
	virtual IStatisticsManager*    GetStatisticsManager() = 0;

};

} // Cry::GamePlatform
