// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Types/Achievement.h>
#include <Types/Statistic.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

namespace Achievement
{

bool Expand(const CAchievement& achievement, 
	CryGUID& guid, 
	EGamePlatform& platform, 
	Schematyc::CSharedString& editorLabel, 
	Schematyc::CSharedString& label, 
	Schematyc::CSharedString& description, 
	Schematyc::CSharedString& apiName, 
	int& apiId,
	int& minProgress,
	int& maxProgress
)
{
	const SAchievementData& achievementData = achievement.GetAchievementData();
	if (!achievementData.guid.IsNull())
	{
		guid = achievementData.guid;
		platform = achievementData.platform;
		editorLabel = achievementData.szEditorLabel;
		label = achievementData.szLabel;
		description = achievementData.szDesc;
		apiName = achievementData.szApiName;
		apiId = achievementData.apiId;
		minProgress = achievementData.minProgress;
		maxProgress = achievementData.maxProgress;

		return true;
	}

	return false;
}

Schematyc::CSharedString ToString(const CAchievement& achievement)
{
	return achievement.GetAchievementData().guid.ToString().c_str();
}

CAchievement FromString(const Schematyc::CSharedString& guid)
{
	CAchievement result(CryGUID::FromString(guid.c_str()));
	return result;
}

bool IsEqual(const CAchievement& a, const CAchievement& b)
{
	return a == b;
}

bool NotEqual(const CAchievement& a, const CAchievement& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	Schematyc::CEnvRegistrationScope scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CAchievement));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Expand, "{993FAB46-6024-47F8-9513-8F4802C1FB28}"_cry_guid, "Expand");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vald', "IsValid");
		pFunction->BindInput(1, 'ach', "Achievement");
		pFunction->BindOutput(2, 'guid', "GUID");
		pFunction->BindOutput(3, 'plat', "Platform");
		pFunction->BindOutput(4, 'elbl', "EditorLabel");
		pFunction->BindOutput(5, 'lbl', "Label");
		pFunction->BindOutput(6, 'desc', "Description");
		pFunction->BindOutput(7, 'name', "APIName");
		pFunction->BindOutput(8, 'id', "APIId");
		pFunction->BindOutput(9, 'minp', "MinProgress");
		pFunction->BindOutput(10, 'maxp', "MaxProgress");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{96D60E2C-AEC0-4CE2-90D6-BE02AF4BAE0D}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ach', "Achievement");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{73446388-9816-455A-BEF6-C8DD13DC088B}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ach', "Achievement");
		pFunction->BindInput(1, 'guid', "GUID String");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{565E2553-DF84-44A0-ADCF-55D961928129}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acha', "A");
		pFunction->BindInput(2, 'achb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{E5C8FE42-6783-41F1-BD27-78648CF77636}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acha', "A");
		pFunction->BindInput(2, 'achb', "B");
		scope.Register(pFunction);
	}
}

} // Achievement

namespace StatDataType
{

Schematyc::CSharedString ToString(EStatDataType dataType)
{
	switch (dataType)
	{
	case EStatDataType::Float: return "Float";
	case EStatDataType::Integer: return "Integer";
	}

	return "None";
}

EStatDataType FromString(const Schematyc::CSharedString& platform)
{
	if (stricmp(platform.c_str(), "Float") == 0)
	{
		return EStatDataType::Float;
	}

	if (stricmp(platform.c_str(), "Integer") == 0)
	{
		return EStatDataType::Integer;
	}
	
	return EStatDataType::None;
}

bool IsEqual(EStatDataType a, EStatDataType b)
{
	return a == b;
}

bool NotEqual(EStatDataType a, EStatDataType b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(EStatDataType));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{D3417248-6465-4856-BF0B-B18D63CEB319}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdt', "StatDataType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{0D268F9F-76B7-4882-86ED-2E3ED786FB8E}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'sdt', "StatDataType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{2FDC717A-CC70-4ECB-8BC4-585DB972CB1F}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdta', "A");
		pFunction->BindInput(2, 'sdtb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{B0B36543-B9FD-481F-AD49-0FB09624C26E}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdta', "A");
		pFunction->BindInput(2, 'sdtb', "B");
		scope.Register(pFunction);
	}
}

} // StatDataType

namespace Statistic
{

bool Expand(const CStatistic& statistic, 
	CryGUID& guid, 
	EGamePlatform& platform, 
	Schematyc::CSharedString& editorLabel, 
	Schematyc::CSharedString& label, 
	Schematyc::CSharedString& description, 
	Schematyc::CSharedString& apiName, 
	EStatDataType& dataType
)
{
	const SStatisticData& statisticData = statistic.GetStatisticData();
	if (!statisticData.guid.IsNull())
	{
		guid = statisticData.guid;
		platform = statisticData.platform;
		editorLabel = statisticData.szEditorLabel;
		label = statisticData.szLabel;
		description = statisticData.szDesc;
		apiName = statisticData.szApiName;
		dataType = statisticData.statDataType;

		return true;
	}

	return false;
}

Schematyc::CSharedString ToString(const CStatistic& statistic)
{
	return statistic.GetStatisticData().guid.ToString().c_str();
}

CStatistic FromString(const Schematyc::CSharedString& guid)
{
	CStatistic result(CryGUID::FromString(guid.c_str()));
	return result;
}

bool IsEqual(const CStatistic& a, const CStatistic& b)
{
	return a == b;
}

bool NotEqual(const CStatistic& a, const CStatistic& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	Schematyc::CEnvRegistrationScope scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CStatistic));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Expand, "{79CA13C5-FC97-4C68-AE78-307D7E495EF5}"_cry_guid, "Expand");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vald', "IsValid");
		pFunction->BindInput(1, 'stat', "Statistic");
		pFunction->BindOutput(2, 'guid', "GUID");
		pFunction->BindOutput(3, 'plat', "Platform");
		pFunction->BindOutput(4, 'elbl', "EditorLabel");
		pFunction->BindOutput(5, 'lbl', "Label");
		pFunction->BindOutput(6, 'desc', "Description");
		pFunction->BindOutput(7, 'name', "APIName");
		pFunction->BindOutput(8, 'dtyp', "DataType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{ACB53B24-2274-410C-A611-11F85EEEB67A}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ach', "Statistic");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{F66687DD-E105-4130-A507-BF40EEC8CF75}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ach', "Statistic");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{EAE75B73-28DE-4D6E-8F43-2EF3D8391FFD}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acha', "A");
		pFunction->BindInput(2, 'achb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{61460116-8A24-4738-8B6B-27F1F0DEF7D2}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acha', "A");
		pFunction->BindInput(2, 'achb', "B");
		scope.Register(pFunction);
	}
}

} // Statistic

namespace StatisticValue
{

template <typename TYPE>
CStatisticValue Create(const TYPE value)
{
	static_assert(std::is_same_v<TYPE, float> || std::is_same_v<TYPE, int32>, "Invalid statistic value type. Must be Float or Int.");
	return CStatisticValue(value);
}

template <typename TYPE>
bool GetRawValue(const CStatisticValue& statisticValue, TYPE& rawValue)
{
	return statisticValue.GetValue(rawValue);
}

EStatDataType GetDataType(const CStatisticValue& statisticValue)
{
	return statisticValue.GetType();
}

Schematyc::CSharedString ToString(const CStatisticValue& statistic)
{
	string result;

	switch (statistic.GetType())
	{
	case EStatDataType::Float: 
	{
		float value;
		if (statistic.GetValue(value))
			result.Format("%f", value).c_str();
	} break;

	case EStatDataType::Integer:
	{
		int32 value;
		if (statistic.GetValue(value))
			result.Format("%d", value).c_str();
	} break;
	}

	return result;
}

CStatisticValue FromString(const Schematyc::CSharedString& value)
{
	CStatisticValue result;

	if (value.length() != 0)
	{
		char* pEnd = nullptr;

		float f = strtof(value.c_str(), &pEnd);
		if (pEnd != nullptr && *pEnd != '\0')
		{
			result = CStatisticValue(f);
		}
		else
		{
			int32 i = strtol(value.c_str(), &pEnd, 10);
			if (pEnd != nullptr && *pEnd != '\0')
			{
				result = CStatisticValue(i);
			}
		}
	}

	return result;
}

bool IsEqual(const CStatisticValue& a, const CStatisticValue& b)
{
	return a == b;
}

bool NotEqual(const CStatisticValue& a, const CStatisticValue& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	Schematyc::CEnvRegistrationScope scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CStatisticValue));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create<float>, "{2E9938C8-CE94-4816-BEDA-E0D24E47EED4}"_cry_guid, "CreateFloat");
		pFunction->SetDescription("Creates a float type value for a statistic.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'sval', "Statistic Value");
		pFunction->BindInput(1, 'val', "Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create<int32>, "{8B9E9523-18D1-4031-933F-07D1FEB1530F}"_cry_guid, "CreateInteger");
		pFunction->SetDescription("Creates an integer type value for a statistic.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'sval', "Statistic Value");
		pFunction->BindInput(1, 'val', "Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetRawValue<float>, "{1540C2D0-E868-494F-A9E7-DEBED52ECEBA}"_cry_guid, "GetFloat");
		pFunction->SetDescription("Reads the value from the statistic as float type.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vlid', "IsValid");
		pFunction->BindInput(1, 'sval', "Statistic Value");
		pFunction->BindOutput(2, 'val', "Raw Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetRawValue<int32>, "{58107689-6699-43DC-8C5E-9BFCCA68A8D2}"_cry_guid, "GetInteger");
		pFunction->SetDescription("Reads the value from the statistic as integer type.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vlid', "IsValid");
		pFunction->BindInput(1, 'sval', "Statistic Value");
		pFunction->BindOutput(2, 'val', "Raw Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetDataType, "{A278B1C9-8F57-47FB-9A93-4E0EC7A7DE8C}"_cry_guid, "GetDataType");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'dtyp', "DataType");
		pFunction->BindInput(1, 'sval', "Statistic Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{C95BE27B-19E3-4768-BBD8-0C3A7BDAEFEB}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ach', "Statistic Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{6BDDA540-7811-4985-B99D-2416C9FBE490}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ach', "Statistic Value");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{0C5B6E37-0C8C-4276-B666-84E6FEF3A62D}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sva', "A");
		pFunction->BindInput(2, 'svb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{01FFDD0F-569D-4E71-A36F-C3BDE56A40D5}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sva', "A");
		pFunction->BindInput(2, 'svb', "B");
		scope.Register(pFunction);
	}
}

} // StatisticValue
} // Cry::GamePlatform
