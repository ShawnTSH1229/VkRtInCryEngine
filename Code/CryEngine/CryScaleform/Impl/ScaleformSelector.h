// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/ICryPlugin.h>
#include <CrySystem/Scaleform/IScaleformHelper.h>

namespace Cry {
namespace ScaleformModule {

class CSelectorModule : public ISelectorModule
{
	CRYINTERFACE_BEGIN()
	CRYINTERFACE_ADD(Cry::IEnginePlugin)
	CRYINTERFACE_ADD(Cry::ScaleformModule::ISelectorModule)
	CRYINTERFACE_END()

	CRYGENERATE_SINGLETONCLASS_GUID(CSelectorModule, "Plugin_CryScaleform", "{42A4ED98-D1EB-4443-BD8D-BDD9B69FAA96}"_cry_guid);

	static CSelectorModule& GetInstance();

	virtual const char*     GetName() const override     { return "CryScaleformSelector"; }
	virtual const char*     GetCategory() const override { return "CryEngine"; }
	virtual bool            Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	virtual bool            LoadScaleformImplementation(SSystemGlobalEnvironment& env, const SSystemInitParams& startupParams) override;

private:
	CSelectorModule();
	static CSelectorModule* s_pInstance;
};

} // ~ScaleformModule namespace
} // ~Cry namespace
