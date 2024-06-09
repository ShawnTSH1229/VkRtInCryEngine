// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include <GamePlatformHelper.h>
#include <Types/Achievement.h>
#include <Types/Statistic.h>
#include <Types/StatisticValue.h>
#include <Schematyc/GamePlatformEnv.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

#include <CryGamePlatform/Interface/IPlatformStatistics.h>

namespace Cry::GamePlatform::Statistics
{

IStatistics* GetStatistics(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		return pService->GetStatistics();
	}

	return nullptr;
}

IAchievement* FindAchievement(const CAchievement& achievement)
{
	const SAchievementData& achievementData = achievement.GetAchievementData();
	if (!achievementData.guid.IsNull())
	{
		if (auto pService = Helper::GetService(achievementData.platform))
		{
			if (auto pStatistics = pService->GetStatistics())
			{
				return pStatistics->GetAchievement(achievementData.apiId);
			}
		}
	}

	return nullptr;
}

bool Upload(EGamePlatform platform)
{
	bool bResult = false;

	if (IStatistics* pStatistics = GetStatistics(platform))
	{
		bResult = pStatistics->Upload();
	}

	return bResult;
}

bool Download(EGamePlatform platform)
{
	bool bResult = false;

	if (IStatistics* pStatistics = GetStatistics(platform))
	{
		bResult = pStatistics->Download();
	}

	return bResult;
}

bool GetAchievementProgress(const CAchievement& achievement, bool& isCompleted, int& progress)
{
	bool bIsValid = false;
	if (auto pAchievement = FindAchievement(achievement))
	{
		isCompleted = pAchievement->IsCompleted();
		progress = pAchievement->GetProgress();
		bIsValid = true;
	}

	return bIsValid;
}

bool SetAchievementCompleted(const CAchievement& achievement)
{
	bool bIsValid = false;
	if (auto pAchievement = FindAchievement(achievement))
	{
		if (!pAchievement->IsCompleted())
		{
			bIsValid = pAchievement->Achieve();
		}
	}

	return bIsValid;
}

bool SetAchievementProgress(const CAchievement& achievement, int32 progress)
{
	bool bIsValid = false;
	if (auto pAchievement = FindAchievement(achievement))
	{
		bIsValid = pAchievement->SetProgress(progress);
	}

	return bIsValid;
}

bool ResetAchievementProgress(const CAchievement& achievement)
{
	bool bIsValid = false;
	if (auto pAchievement = FindAchievement(achievement))
	{
		bIsValid = pAchievement->Reset();
	}

	return bIsValid;
}

bool GetStatisticValue(const CStatistic& statistic, CStatisticValue& value)
{
	bool bIsValid = false;
#
	const SStatisticData& statisticData = statistic.GetStatisticData();
	if (!statisticData.guid.IsNull())
	{
		if (IStatistics* pStatistics = GetStatistics(statisticData.platform))
		{
			switch (statisticData.statDataType)
			{
			case EStatDataType::Float:
				float f;
				bIsValid = pStatistics->Get(statisticData.szApiName, f);
				value = f;
				break;
			case EStatDataType::Integer:
				int32 i;
				bIsValid = pStatistics->Get(statisticData.szApiName, i);
				value = i;
				break;
			}
		}
	}

	return bIsValid;
}

bool SetStatisticValue(const CStatistic& statistic, const CStatisticValue& value)
{
	CRY_ASSERT(value.GetType() != EStatDataType::None);
	if (value.GetType() != EStatDataType::None)
	{
		const SStatisticData& statisticData = statistic.GetStatisticData();
		if (!statisticData.guid.IsNull())
		{
			if (IStatistics* pStatistics = GetStatistics(statisticData.platform))
			{
				if (statisticData.statDataType != value.GetType())
				{
					const char* fromName = Helper::StatTypeToName(value.GetType());
					const char* toName = Helper::StatTypeToName(statisticData.statDataType);
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Incorrect statistic type '%s' for '%s'. Expected type '%s'.", fromName, statisticData.szLabel, toName);
				}
				else if (value.GetType() == EStatDataType::Integer)
				{
					int32 i;
					if (value.GetValue(i))
					{
						return pStatistics->Set(statisticData.szApiName, i);
					}
				}
				else if (value.GetType() == EStatDataType::Float)
				{
					float f;
					if (value.GetValue(f))
					{
						return pStatistics->Set(statisticData.szApiName, f);
					}
				}
			}
		}
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{39883567-61D5-4B0C-8E93-6EB0CC3AC26E}"_cry_guid, "Statistics"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Upload, "{46E220BE-A089-4645-818E-63FBB168C0A8}"_cry_guid, "Upload");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'plat', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Download, "{6F1862B2-E01F-4FDE-9E94-8171FAD28FEB}"_cry_guid, "Download");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'plat', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetAchievementProgress, "{4BA3FA6A-C279-4CB3-B2E8-A8F283DE1437}"_cry_guid, "GetAchievementProgress");
		pFunction->SetDescription("Gets the progress of the specified achievement.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vlid', "IsValid");
		pFunction->BindInput(1, 'ach', "Achievement");
		pFunction->BindOutput(2, 'cmpl', "Is Completed");
		pFunction->BindOutput(3, 'prog', "Progress");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetAchievementCompleted, "{BCEBA56A-B762-4629-9CD7-73CF6FF27D1E}"_cry_guid, "SetAchievementCompleted");
		pFunction->SetDescription("Sets the specified achievement completed.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'ach', "Achievement");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ResetAchievementProgress, "{43DB080D-C2D1-4388-895B-34669748E0A9}"_cry_guid, "ResetAchievementProgress");
		pFunction->SetDescription("Resets the progress for the specified achievement.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'ach', "Achievement");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetAchievementProgress, "{77694E90-E4AF-4E18-8033-EFAECB00DAC7}"_cry_guid, "SetAchievementProgress");
		pFunction->SetDescription("Sets the progress amount for the specified achievement.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'ach', "Achievement");
		pFunction->BindInput(2, 'prog', "Progress");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetStatisticValue, "{DC356B77-B9CC-4AEC-8F3F-2FCCBE422743}"_cry_guid, "GetStatisticValue");
		pFunction->SetDescription("Gets the statistic value container for the statistic on the platform service.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vlid', "IsValid");
		pFunction->BindInput(1, 'stat', "Statistic");
		pFunction->BindOutput(2, 'sval', "Statistic Value");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetStatisticValue, "{E4DC2654-19D2-43B5-8CFF-954B5045E019}"_cry_guid, "SetStatisticValue");
		pFunction->SetDescription("Sets the value for the statistic on the platform service.");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vlid', "IsValid");
		pFunction->BindInput(1, 'stat', "Statistic");
		pFunction->BindInput(2, 'sval', "Statistic Value");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Statistics
