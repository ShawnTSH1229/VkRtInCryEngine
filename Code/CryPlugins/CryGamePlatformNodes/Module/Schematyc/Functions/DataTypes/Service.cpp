// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <Types/PlatformEventFlags.h>
#include <Types/Service.h>
#include <Types/DynArray.h>
#include <Schematyc/GamePlatformEnv.h>
#include <Schematyc/Utils.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform
{

namespace _ApplicationIdentifier
{

ApplicationIdentifier Create(EGamePlatform platform, const Schematyc::CSharedString& id)
{
	ApplicationIdentifier applicationIdentifier;

	if (IService* pService = Helper::GetService(platform))
	{
		pService->SetIdentifierFromString(applicationIdentifier, id.c_str());
	}

	return applicationIdentifier;
}

EGamePlatform GetPlatform(const ApplicationIdentifier& applicationId)
{
	return Helper::GetServiceEnum(applicationId.Service());
}

Schematyc::CSharedString ToString(const ApplicationIdentifier& applicationId)
{
	stack_string result;
	applicationId.GetAsString(result);
	return Schematyc::CSharedString(result.c_str());
}

bool IsEqual(const ApplicationIdentifier& a, const ApplicationIdentifier& b)
{
	return a == b;
}

bool NotEqual(const ApplicationIdentifier& a, const ApplicationIdentifier& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(ApplicationIdentifier));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{7FE4A28C-6084-495C-8F2E-608EE8BCC7C3}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'app', "Application");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInput(2, 'apps', "Application Id");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{28685412-90EA-40E1-A12B-2507984950A0}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'app', "Application");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{82FCD37F-A611-4B10-9DD4-2CAA2D7B480A}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'app', "Application");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{DDBE9489-A61C-43FA-9AAE-0A015D86B162}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'appa', "A");
		pFunction->BindInput(2, 'appb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{0CA2CB76-AB3D-40B2-B8B7-04552C904DFB}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'appa', "A");
		pFunction->BindInput(2, 'appb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<ApplicationIdentifier>, "{9B98EFC9-C292-49B2-9D2F-B17161EC3517}"_cry_guid, "ToArray");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindInput(1, 'apps', "DynArray[Application Identifier]");
		pFunction->BindInputArrayType<ApplicationIdentifier>(2, 'appa', "Array[Application Identifier]");
		scope.Register(pFunction);
	}
}

} // _ApplicationIdentifier

namespace Platform
{

Schematyc::CSharedString ToString(EGamePlatform platform)
{
	switch (platform)
	{
	case EGamePlatform::Main: return "Main";
	case EGamePlatform::Steam: return "Steam";
	case EGamePlatform::Discord: return "Discord";
	case EGamePlatform::Playstation: return "Playstation";
	case EGamePlatform::Xbox: return "Xbox";
	}

	return "None";
}

EGamePlatform FromString(const Schematyc::CSharedString& platform)
{
	if (stricmp(platform.c_str(), "Main") == 0)
	{
		return EGamePlatform::Main;
	}

	if (stricmp(platform.c_str(), "Steam") == 0)
	{
		return EGamePlatform::Steam;
	}

	if (stricmp(platform.c_str(), "Discord") == 0)
	{
		return EGamePlatform::Discord;
	}

	if (stricmp(platform.c_str(), "Playstation") == 0)
	{
		return EGamePlatform::Playstation;
	}

	if (stricmp(platform.c_str(), "Xbox") == 0)
	{
		return EGamePlatform::Xbox;
	}
	
	return EGamePlatform::None;
}

bool IsEqual(EGamePlatform a, EGamePlatform b)
{
	return a == b;
}

bool NotEqual(EGamePlatform a, EGamePlatform b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(EGamePlatform));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{4BAA42A4-8904-4F51-B322-B1D6F74F5665}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'plat', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{B9A423B2-8FD6-45DE-AE51-63E65DA1494E}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{8E4B9126-6478-455A-99EF-6AC39DBFCB9C}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{9E521BF7-60B7-4839-A41F-E31931732E0E}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
}

} // Platform

namespace _PlatformEventFlags
{

Schematyc::CSharedString ToString(const CPlatformEventFlags& eventFlags)
{
	std::vector<char> chars;
	chars.reserve(eventFlags.NumFlags()+1);
	*chars.rbegin() = 0;

	for (size_t index = 0; index < eventFlags.NumFlags(); index++)
	{
		chars[index] = eventFlags.Check(index) ? '1' : '0';
	}

	return Schematyc::CSharedString(chars.data());
}

CPlatformEventFlags FromString(const Schematyc::CSharedString& eventFlags)
{
	CPlatformEventFlags flags;

	const char* pChar = eventFlags.c_str();
	for (size_t index = 0; index < eventFlags.length(); index++, pChar++)
	{
		if (*pChar == '1')
			flags.Set(index);
	}

	return flags;
}

bool HasAny(const CPlatformEventFlags& eventFlags, const CPlatformEventFlags& checkFlags)
{
	return eventFlags.CheckAny(checkFlags);
}

bool HasAll(const CPlatformEventFlags& eventFlags, const CPlatformEventFlags& checkFlags)
{
	return eventFlags.CheckAll(checkFlags);
}

CPlatformEventFlags AddFlags(const CPlatformEventFlags& eventFlags, const CPlatformEventFlags& addFlags)
{
	CPlatformEventFlags newFlags(eventFlags);
	newFlags.Add(addFlags);
	return newFlags;
}

CPlatformEventFlags RemoveFlags(const CPlatformEventFlags& eventFlags, const CPlatformEventFlags& removeFlags)
{
	CPlatformEventFlags newFlags(eventFlags);
	newFlags.Remove(removeFlags);
	return newFlags;
}

bool IsEqual(const CPlatformEventFlags& a, const CPlatformEventFlags& b)
{
	return a == b;
}

bool NotEqual(const CPlatformEventFlags& a, const CPlatformEventFlags& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CPlatformEventFlags));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{3B2EC544-1668-4FDA-ACDC-F90900829F0D}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sodr', "SortOrder");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{AA772C22-FFDB-4DE5-8220-3BBFFB8A6DD5}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'sodr', "SortOrder");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&HasAny, "{C63CB4BC-00FB-436B-83D7-D2DDAC05F68A}"_cry_guid, "HasAny");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "EventFlags");
		pFunction->BindInput(2, 'sdrb', "CheckFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&HasAll, "{D0053925-C6E3-4FFD-A612-B214642A26DB}"_cry_guid, "HasAll");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "EventFlags");
		pFunction->BindInput(2, 'sdrb', "CheckFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&AddFlags, "{236D1E1B-1032-4E53-8ED8-799287E02DAB}"_cry_guid, "AddFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "EventFlags");
		pFunction->BindInput(2, 'sdrb', "AddFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RemoveFlags, "{EC84A42C-D7EC-4CA2-800D-2693056D0635}"_cry_guid, "RemoveFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "EventFlags");
		pFunction->BindInput(2, 'sdrb', "RemoveFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{18669B76-6363-4213-AD3A-B009F6498028}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "A");
		pFunction->BindInput(2, 'sdrb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{165EE0F5-8487-4B65-84EA-95B9391E3548}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "A");
		pFunction->BindInput(2, 'sdrb', "B");
		scope.Register(pFunction);
	}
}

} // _PlatformEventFlags

namespace _TransactionIdentifier
{

TransactionIdentifier Create(EGamePlatform platform, const Schematyc::CSharedString& id)
{
	TransactionIdentifier transactionId;

	if (IService* pService = Helper::GetService(platform))
	{
		pService->SetIdentifierFromString(transactionId, id.c_str());
	}

	return transactionId;
}

EGamePlatform GetPlatform(const TransactionIdentifier& transactionId)
{
	return Helper::GetServiceEnum(transactionId.Service());
}

Schematyc::CSharedString ToString(const TransactionIdentifier& transactionId)
{
	stack_string result;
	transactionId.GetAsString(result);
	return Schematyc::CSharedString(result.c_str());
}

bool IsEqual(const TransactionIdentifier& a, const TransactionIdentifier& b)
{
	return a == b;
}

bool NotEqual(const TransactionIdentifier& a, const TransactionIdentifier& b)
{
	return !IsEqual(a,b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(TransactionIdentifier));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{4B9056AA-AAF2-4C16-8AD2-FBD0F6B274C5}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'tid', "TransactionId");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInput(2, 'tids', "IdString");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{01360F43-AAE2-480C-A38C-51CE5B12BC10}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'tid', "TransactionId");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{B5ECAD83-A3DD-48CF-81E8-C6CD0BF9E9CF}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'tid', "TransactionId");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{1165AC92-96F3-4C5E-B7D3-4267A8586D5D}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'tida', "A");
		pFunction->BindInput(2, 'tidb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{1699E5D1-6715-4FBB-AC9F-0CBE13FE7446}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'tida', "A");
		pFunction->BindInput(2, 'tidb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<TransactionIdentifier>, "{9B8FA3DE-5AEB-4676-AFD8-419A0AF4F443}"_cry_guid, "ToArray");
		pFunction->BindInput(1, 'ts', "DynArray[Transaction Identifier]");
		pFunction->BindInputArrayType<TransactionIdentifier>(2, 'arr', "Array[Transaction Identifier]");
		scope.Register(pFunction);
	}
}

} // _TransactionIdentifier
} // Cry::GamePlatform
