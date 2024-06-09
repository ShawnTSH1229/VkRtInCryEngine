// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <Types/RemoteStorage.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

namespace LocalCache
{

EGamePlatform GetPlatform(const CCachedLocal& cache)
{
	return cache.GetPlatform();
}

Schematyc::CSharedString GetName(const CCachedLocal& cache)
{
	return cache.GetName();
}

Schematyc::CSharedString ToString(const CCachedLocal& cache)
{
	return GetName(cache);
}

bool IsEqual(const CCachedLocal& a, const CCachedLocal& b)
{
	return a == b;
}

bool NotEqual(const CCachedLocal& a, const CCachedLocal& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CCachedLocal));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{F2A4064A-0D1F-45E9-95B5-37760036D7C7}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'lche', "Local Cache");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetName, "{C5043F8D-64D5-4050-894B-20F7BE361780}"_cry_guid, "GetName");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'lche', "Local Cache");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{16DD2F43-E9C0-4AC7-8524-B02C9994E430}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'lche', "Local Cache");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{9AB70CCF-8944-4C6F-A0AB-DD4A699ACB32}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'lcha', "A");
		pFunction->BindInput(2, 'lchb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{2AEB87A2-89EA-4CB5-9EE2-EF0CE9D47415}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'lcha', "A");
		pFunction->BindInput(2, 'lchb', "B");
		scope.Register(pFunction);
	}
}

} // LocalCache

namespace SharedCache
{

EGamePlatform GetPlatform(const CCachedShared& cache)
{
	return cache.GetPlatform();
}

ISharedRemoteFile::Identifier GetId(const CCachedShared& cache)
{
	return cache.GetSharedFileId();
}

Schematyc::CSharedString ToString(const CCachedShared& cache)
{
	Schematyc::CSharedString sharedId;
	sharedId.Format("%" PRIu64, cache.GetSharedFileId());
	return sharedId;
}

bool IsEqual(const CCachedShared& a, const CCachedShared& b)
{
	return a == b;
}

bool NotEqual(const CCachedShared& a, const CCachedShared& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CCachedShared));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{25BBEA32-305B-4BEA-ACEB-B1286DFB36E3}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'sche', "Local Cache");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetId, "{8FDC12F5-2ACB-4ED6-92D2-EDBECAAB7628}"_cry_guid, "GetId");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'id', "Identifier");
		pFunction->BindInput(1, 'sche', "Local Cache");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{E375F161-2EC7-4DDE-B520-129E046BD6C9}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sche', "Shared Cache");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{8477B089-0887-450B-BDF9-78B9AA02E0F7}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'scha', "A");
		pFunction->BindInput(2, 'schb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{8046C9D1-5EAC-41BF-B85A-1480585FEE2A}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'scha', "A");
		pFunction->BindInput(2, 'schb', "B");
		scope.Register(pFunction);
	}
}

} // SharedCache
} // Cry::GamePlatform
