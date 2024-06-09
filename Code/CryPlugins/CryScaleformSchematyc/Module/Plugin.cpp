// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "Plugin.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

#include "FlashModule.h"

namespace Cry::Scaleform
{

CPlugin_CryScaleformSchematyc::~CPlugin_CryScaleformSchematyc()
{
	// Unregister from system events
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	// Unregister components from this plugin
	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CPlugin_CryScaleformSchematyc::GetCID());
	}
}

CFlashModule* CPlugin_CryScaleformSchematyc::GetFlashModule()
{
	if (!m_pFlashEnv)
	{
		m_pFlashEnv = std::make_unique<CFlashModule>();
	}

	return m_pFlashEnv.get();
}

bool CPlugin_CryScaleformSchematyc::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	if (gEnv->IsDedicated())
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[CryScaleformSchematyc] Init Failed. Dedicated server instances are not supported.");
		return false;
	}

	// Register for engine system events, we need to register the Schematyc components at the right time
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CPlugin_CryScaleformSchematyc");

	return true;
}

void CPlugin_CryScaleformSchematyc::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{

	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		if (!gEnv->IsEditor())
		{
			GetFlashModule()->SetAllowReload(false);
		}
	}
	break;

	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		if (gEnv->pSchematyc != nullptr)
		{
			GetFlashModule()->RegisterSchematyc();
		}
	}
	break;

	case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
	{
		m_pFlashEnv->SetAllowReload(false);
	}
	break;

	case ESYSTEM_EVENT_GAME_MODE_SWITCH_END:
	{
		m_pFlashEnv->SetAllowReload(true);
	}
	break;

	}
}

} // Cry::Scaleform

CRYREGISTER_SINGLETON_CLASS(Cry::Scaleform::CPlugin_CryScaleformSchematyc)