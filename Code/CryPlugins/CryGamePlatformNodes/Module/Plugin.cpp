// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "Plugin.h"

#include <Types/Platform.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

// DEBUG (Remove)
#include <CrySystem/ConsoleRegistration.h>

namespace Cry::GamePlatform
{

CPlugin_GamePlatformNodes::~CPlugin_GamePlatformNodes()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	m_pRemoteStorageCache.reset();
	m_pFlowNodeManager.reset();
	m_pEventManager.reset();
	m_pStatisticsManager.reset();

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CPlugin_GamePlatformNodes::GetCID());
	}
}

bool CPlugin_GamePlatformNodes::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	m_pStatisticsManager = std::make_unique<CStatisticsManager>();
	m_pEventManager = std::make_unique<CPlatformEventManager>();
	m_pFlowNodeManager = std::make_unique<CFlowNodeManager>();
	m_pRemoteStorageCache = std::make_unique<CRemoteStorageCache>();

	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CPlugin_GamePlatformNodes");

	return true;
}

void CPlugin_GamePlatformNodes::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{

	case ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE:
		{
			m_pStatisticsManager->Initialize();
			m_pFlowNodeManager->Initialize();

			if (gEnv->pSchematyc)
			{
				gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
					stl::make_unique<Schematyc::CEnvPackage>(
						CPlugin_GamePlatformNodes::GetCID(),
						"GamePlatformNodes",
						"Crytek GmbH",
						"Game Platform node package",
						RegisterPlatformEnv
					)
				);
			}
		} break;
	}
}

IPlatformEventManager* CPlugin_GamePlatformNodes::GetPlatformEventManager()
{
	return m_pEventManager.get();
}

IRemoteStorageCache* CPlugin_GamePlatformNodes::GetRemoteStorageCache()
{
	return m_pRemoteStorageCache.get();
}

IStatisticsManager* CPlugin_GamePlatformNodes::GetStatisticsManager()
{
	return m_pStatisticsManager.get();
}

} // Cry::GamePlatform

CRYREGISTER_SINGLETON_CLASS(Cry::GamePlatform::CPlugin_GamePlatformNodes)