// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <IPlugin.h>
#include <System/StatisticsManager.h>
#include <System/PlatformEventManager.h>
#include <System/RemoteStorageCache.h>
#include <FlowGraph/FlowNodeManager.h>

namespace Cry::GamePlatform
{

class CPlugin_GamePlatformNodes
	: public IPlugin_GamePlatformNodes
	  , public ISystemEventListener
{

	struct SDiscordId { static const CryGUID& IID() { return DiscordServiceID; }; };
	struct SSteamId { static const CryGUID& IID() { return SteamServiceID; }; };
	struct SPSNId { static const CryGUID&  IID() { return PSNServiceID; }; };
	struct SXboxId { static const CryGUID& IID() { return XboxServiceID; }; };

public:

	CRYINTERFACE_BEGIN()
	CRYINTERFACE_ADD(Cry::IEnginePlugin)
	CRYINTERFACE_ADD(IPlugin_GamePlatformNodes)
	CRYINTERFACE_END()

	CRYGENERATE_SINGLETONCLASS_GUID(CPlugin_GamePlatformNodes, "CryGamePlatformNodes", "{E555F550-B7EA-4392-89D0-CA959FC1CA1D}"_cry_guid)

	PLUGIN_FLOWNODE_REGISTER
	PLUGIN_FLOWNODE_UNREGISTER

	virtual ~CPlugin_GamePlatformNodes();

	// Cry::IEnginePlugin
	virtual const char* GetCategory() const override { return "Plugin"; }
	virtual bool        Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	// IPlugin_GamePlatformNodes
	virtual IPlatformEventManager* GetPlatformEventManager() override;
	virtual IRemoteStorageCache*   GetRemoteStorageCache() override;
	virtual IStatisticsManager*    GetStatisticsManager() override;
	// ~IPlugin_GamePlatformNodes

	static CPlugin_GamePlatformNodes* GetInstance()
	{
		return cryinterface_cast<CPlugin_GamePlatformNodes>(CPlugin_GamePlatformNodes::s_factory.CreateClassInstance().get());
	}

private:

	std::unique_ptr<CFlowNodeManager>      m_pFlowNodeManager;
	std::unique_ptr<CStatisticsManager>    m_pStatisticsManager;
	std::unique_ptr<CRemoteStorageCache>    m_pRemoteStorageCache;
	std::unique_ptr<CPlatformEventManager> m_pEventManager;

};

} // Cry::GamePlatform
