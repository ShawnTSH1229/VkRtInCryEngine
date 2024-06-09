// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <GamePlatformHelper.h>
#include <Types/Leaderboard.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>

namespace Cry::GamePlatform::Leaderboards
{

bool DownloadEntries(const LeaderboardIdentifier& leaderboardId, ILeaderboards::ERequest request, int minRange, int maxRange, bool bTimeBased)
{
	if (IPlugin* pPlugin = CPluginHelper<IPlugin>::Get())
	{
		if (IService* pService = pPlugin->GetService(leaderboardId.Service()))
		{
			if (ILeaderboards* pLeaderboard = pService->GetLeaderboards())
			{
				pLeaderboard->DownloadEntries(leaderboardId, request, minRange, maxRange, bTimeBased);
				return true;
			}
		}
	}

	return false;
}

bool UpdateScore(const LeaderboardIdentifier& leaderboardId, int score, ILeaderboards::EScoreType scoreType, bool bForce)
{
	if (IPlugin* pPlugin = CPluginHelper<IPlugin>::Get())
	{
		if (IService* pService = pPlugin->GetService(leaderboardId.Service()))
		{
			if (ILeaderboards* pLeaderboard = pService->GetLeaderboards())
			{
				pLeaderboard->UpdateScore(leaderboardId, score, scoreType, bForce);
				return true;
			}
		}
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{E0925F5A-2454-4ECC-BC52-D5A75F580F6A}"_cry_guid, "Leaderboards"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DownloadEntries, "{315E348E-AF77-4435-A8BB-43CBD7CA5E01}"_cry_guid, "DownloadEntries");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'lid', "Leaderboard Id");
		pFunction->BindInput(2, 'req', "Request");
		pFunction->BindInput(3, 'minr', "Min Range");
		pFunction->BindInput(4, 'maxr', "Max Range");
		pFunction->BindInput(5, 'tb', "Is Time Based");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&UpdateScore, "{D09DA96B-2229-4824-8EF2-801E42AEB86A}"_cry_guid, "UpdateScore");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'lid', "Leaderboard Id");
		pFunction->BindInput(2, 'scr', "Score");
		pFunction->BindInput(3, 'scrt', "Score Type");
		pFunction->BindInput(4, 'frc', "Force");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Leaderboards
