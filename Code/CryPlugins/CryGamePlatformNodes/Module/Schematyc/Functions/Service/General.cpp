// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include <GamePlatformHelper.h>
#include <Schematyc/GamePlatformEnv.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform::General
{

bool GetBuildIdentifier(EGamePlatform platform, int& build)
{
	if (auto pService = Helper::GetService(platform))
	{
		build = pService->GetBuildIdentifier();
		return true;
	}

	return false;
}

bool GetApplicationIdentifier(EGamePlatform platform, ApplicationIdentifier& application)
{
	if (auto pService = Helper::GetService(platform))
	{
		application = pService->GetApplicationIdentifier();
		return true;
	}

	return false;
}

bool OwnsApplication(const ApplicationIdentifier& application)
{
	if (auto pService = Helper::GetService(application.Service()))
	{
		return pService->OwnsApplication(application);
	}

	return false;
}

bool GetEnvironmentValue(EGamePlatform platform, const Schematyc::CSharedString& varName, Schematyc::CSharedString& value)
{
	bool success = false;

	if (auto pService = Helper::GetService(platform))
	{
		string out;
		success = pService->GetEnvironmentValue(varName.c_str(), out);

		if (success)
		{
			value = out.c_str();
		}
	}

	return success;
}

bool MarkContentCorrupt(EGamePlatform platform, const bool missingFilesOnly)
{
	if (auto pService = Helper::GetService(platform))
	{
		pService->MarkContentCorrupt(missingFilesOnly);
		return true;
	}

	return false;
}

void GetAvailableServices(Schematyc::CAnyArrayPtr pServices)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"

	static std::array<EGamePlatform, 4> platforms = {
		EGamePlatform::Steam,
		EGamePlatform::Playstation,
		EGamePlatform::Xbox,
		EGamePlatform::Discord
	};
#pragma clang diagnostic pop

	if (pServices)
	{
		pServices->Clear();

		for (auto platform : platforms)
		{
			if (Helper::GetService(platform) != nullptr)
			{
				(*pServices).PushBack(Schematyc::CAnyConstRef(platform));
			}
		}
	}
}

Schematyc::CSharedString GetServiceName(EGamePlatform service)
{
	return Helper::GetServiceName(service);
}

ServiceIdentifier GetServiceIdentifier(EGamePlatform service)
{
	if (IService* pService = Helper::GetService(service))
		return pService->GetServiceIdentifier();

	return ServiceIdentifier();
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{2072F180-7312-4E7B-BD20-4EB739970D8D}"_cry_guid, "General"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetBuildIdentifier, "{47DD4046-A9EF-4D3E-872B-54745E440C77}"_cry_guid, "GetBuildIdentifier");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindOutput(2, 'bld', "Build");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetApplicationIdentifier, "{59CF2C68-70CE-47E9-A988-9DB7367839DE}"_cry_guid, "GetApplicationId");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindOutput(2, 'app', "Application");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&OwnsApplication, "{6117B6B6-485B-46FB-A42F-A00CA2A5671F}"_cry_guid, "OwnsApplication");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'app', "Application");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetEnvironmentValue, "{C85B85A1-CF5D-472B-BA98-34C42F6A31DC}"_cry_guid, "GetEnvironmentValue");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInput(2, 'key', "Name");
		pFunction->BindOutput(3, 'val', "Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&MarkContentCorrupt, "{D5ADF9B8-172D-4874-8C66-4065E9E1DCB9}"_cry_guid, "MarkContentCorrupt");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInput(2, 'mfo', "Missing Files Only");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetAvailableServices, "{CD80A312-8656-4E00-9863-34923B5DB757}"_cry_guid, "GetAvailableServices");
		pFunction->BindInputArrayType<EGamePlatform>(1, 'arr', "Array[GamePlatform]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetServiceName, "{1602F256-9DAF-4C5C-A302-8236B4F4B867}"_cry_guid, "GetServiceName");
		pFunction->BindOutput(0, 'name', "Service Name");
		pFunction->BindInput(1, 'plat', "Service");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetServiceIdentifier, "{9DA33375-D141-468B-8331-1C7B7F2CA00C}"_cry_guid, "GetServiceIdentifier");
		pFunction->BindOutput(0, 'svci', "Service Identifier");
		pFunction->BindInput(1, 'plat', "Service");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::General
