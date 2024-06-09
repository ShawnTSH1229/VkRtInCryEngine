// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Types/FixedString.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Reflection/TypeDesc.h>

namespace Cry::GamePlatform::FixedString
{

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(g_stdModuleGUID).Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CryFixedStringT<128>));

	Schematyc::CEnvRegistrationScope scope = registrar.Scope(Schematyc::GetTypeDesc<CryFixedStringT<128>>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Schematyc::Helpers::FixedString::IsEqual<128>, "{960FE1A7-FBB3-47D4-ADD2-E4C76FF3A798}"_cry_guid, "IsEqual");
		pFunction->BindOutput(0, 'eql', "Equal", "Checks if the two fixed-size strings are equal");
		pFunction->BindInput(1, 'lft', "Fixed String A", "First fixed-size string to check against");
		pFunction->BindInput(2, 'rht', "Fixed String B", "Second fixed-size string to check against");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Schematyc::Helpers::FixedString::LengthOf<128>, "{DECB2EAC-6E65-46E1-B0CC-AC968F222BA4}"_cry_guid, "LengthOf");
		pFunction->BindOutput(0, 'len', "Length", "Gets the character length of the given fixed-size string");
		pFunction->BindInput(1, 'str', "Fixed String", "The fixed-size string to get the length of");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Schematyc::Helpers::FixedString::ToString<128>, "{EA0B984E-E93B-435F-8108-50605631DAA7}"_cry_guid, "ToString");
		pFunction->BindInput(1, 'ins', "Fixed String", "The fixed-size string to convert");
		pFunction->BindOutput(2, 'outs', "String", "The converted string");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Schematyc::Helpers::FixedString::FromString<128>, "{7A69149B-2CDE-4687-A12C-CA05FCE1FF2B}"_cry_guid, "FromString");
		pFunction->BindInput(1, 'ins', "String", "The string to convert");
		pFunction->BindOutput(2, 'outs', "Fixed String", "The fixed-size converted string");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::FixedString
