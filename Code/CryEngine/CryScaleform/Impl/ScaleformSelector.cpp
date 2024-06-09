// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include <CryCore/Platform/platform_impl.inl>
#include <CryRenderer/IRenderer.h>
#include "ScaleformSelector.h"
#include "Module.h"

namespace Cry {
namespace ScaleformModule {

CRYREGISTER_SINGLETON_CLASS(CSelectorModule);

CSelectorModule* CSelectorModule::s_pInstance = nullptr;

CSelectorModule::CSelectorModule()
{
}

CSelectorModule& CSelectorModule::GetInstance()
{
	CRY_ASSERT(s_pInstance, "CryScaleform is not initialized yet.");
	return *s_pInstance;
}

bool CSelectorModule::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& startupParams)
{
	s_pInstance = this;
	env.pScaleformHelper = nullptr;
	return true;
}

bool CSelectorModule::LoadScaleformImplementation(SSystemGlobalEnvironment& env, const SSystemInitParams& startupParams)
{
	if (!s_pInstance) //Module wasn't initialized, fallback to load Scaleform3
	{
		return false;
	}

	CRY_ASSERT(env.pRenderer);
	if (!env.pRenderer)
	{
		CryLog("Renderer must be initialized before Scaleform4 implementation is loaded.");
		return false;
	}

	string scaleformLibraryName("CryScaleform");
	ERenderType type = env.pRenderer->GetRenderType();

	switch (type)
	{
	case ERenderType::Direct3D11:
		scaleformLibraryName += "D3D11";
		break;

	case ERenderType::Direct3D12:
		scaleformLibraryName += "D3D12";
		break;

	case ERenderType::Vulkan:
		scaleformLibraryName += "Vulkan";
		break;

	case ERenderType::GNM:
		scaleformLibraryName += "GNM";
		break;

	default:
		CryLog("Renderer not supported by Scaleform4.");
		return false;
	}

	if (ICryFactory* pFactory = env.pSystem->LoadModuleWithFactory(scaleformLibraryName, cryiidof<Cry::ScaleformModule::IModule>()))
	{
		std::shared_ptr<Cry::IDefaultModule> pModule = cryinterface_cast<Cry::IDefaultModule, ICryUnknown>(pFactory->CreateClassInstance());
		if (pModule)
		{
			if (pModule->Initialize(env, startupParams))
			{
				return true;
			}
			
			// Unload module
			env.pSystem->UnloadEngineModule(scaleformLibraryName);
		}
	}
	CryLog("Attempt to load Scaleform4 library from '%s' failed.", scaleformLibraryName.c_str());
	return false;
}

} // ~ScaleformModule namespace
} // ~Cry namespace
