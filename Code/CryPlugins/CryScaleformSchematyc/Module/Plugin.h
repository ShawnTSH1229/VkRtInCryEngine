// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/ICryPlugin.h>
#include "FlashModule.h"

namespace Cry::Scaleform
{

class CPlugin_CryScaleformSchematyc
	: public Cry::IEnginePlugin
	, public ISystemEventListener
{

public:

	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CPlugin_CryScaleformSchematyc, "CryScaleformSchematyc", "{EE22745A-6E2F-4657-B606-AD65D5E0051C}"_cry_guid)

	virtual ~CPlugin_CryScaleformSchematyc();

	CFlashModule* GetFlashModule();

	// Cry::IEnginePlugin
	virtual const char* GetCategory() const override { return "Game"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	static CPlugin_CryScaleformSchematyc* GetInstance()
	{
		return cryinterface_cast<CPlugin_CryScaleformSchematyc>(CPlugin_CryScaleformSchematyc::s_factory.CreateClassInstance().get());
	}

private:

	std::unique_ptr<CFlashModule> m_pFlashEnv;

};

} // Cry::Scaleform