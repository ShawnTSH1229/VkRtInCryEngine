// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "LeadboardNodes.h"

#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryGamePlatform/Interface/IPlatformLeaderboards.h>

namespace Cry::GamePlatform
{

void CFlowNode_GetLeaderboardData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get",              _HELP("Gets the connection status of the service for the specified platform.")),
		InputPortConfig<int>("Platform", 0,      _HELP("The platform to check against."), "Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LeaderboardId", _HELP("Leaderboard Identifier.")),
		InputPortConfig<int>("RequestType", 0,   _HELP("Type of the request (Scope/Category)."), "RequestType", _UICONFIG("enum_int:Global=0,GlobalRelativeToUser=1,Friends=2,Users=3")),
		InputPortConfig<int>("MinRange", 1,      _HELP("Minimum range of the results (default 1)."), "MinRange"),
		InputPortConfig<int>("MaxRange", 10,     _HELP("Maximum range of the results (default 10)."), "MaxRange"),
		InputPortConfig<bool>("IsTimeBased",     _HELP("Affects sort order and display type.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets leaderboard information as requested (Listen to OnLeaderboardEntryDownloaded events).");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetLeaderboardData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	using ERequest = ILeaderboards::ERequest;

	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (ILeaderboards* pLeaderboard = pService->GetLeaderboards())
			{
				LeaderboardIdentifier leaderboardId{};

				if (pService->SetIdentifierFromString(leaderboardId, GetPortString(pActInfo, eIn_LeaderboardId)))
				{
					ERequest requestType = ERequest::Global;

					switch (GetPortInt(pActInfo, eIn_RequestType))
					{
					case 1:
						requestType = ERequest::GlobalRelativeToUser;
						break;
					case 2:
						requestType = ERequest::Friends;
						break;
					case 3:
						requestType = ERequest::Users;
						break;
					}

					pLeaderboard->DownloadEntries(leaderboardId, requestType, GetPortInt(pActInfo, eIn_MinRange), GetPortInt(pActInfo, eIn_MaxRange), GetPortBool(pActInfo, eIn_TimeBased));
				}
			}
		}
	}
}

void CFlowNode_UpdateLeaderboardScore::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Update", _HELP("Updates the users leaderboard entry using the provided data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),               "Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LeaderboardId", _HELP("Id of the leaderboard to update.")),
		InputPortConfig<int>("Score", _HELP("The value to set the leaderboard entry to.")),
		InputPortConfig<int>("ScoreType", 0, _HELP("Type of the score (Value based/Time based)."),  "ScoreType", _UICONFIG("enum_int:Value=0,Time=1")),
		InputPortConfig<bool>("Force", _HELP("Set to true to force the update of the leaderboard entry.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Updates leaderboard data for the local user.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_UpdateLeaderboardScore::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	using EScoreType = ILeaderboards::EScoreType;

	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Update))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (ILeaderboards* pLeaderboard = pService->GetLeaderboards())
			{
				LeaderboardIdentifier leaderboardId{};

				if (pService->SetIdentifierFromString(leaderboardId, GetPortString(pActInfo, eIn_LeaderboardId)))
				{
					const EScoreType scoreType = GetPortInt(pActInfo, eIn_ScoreType) == 0 ? EScoreType::Score : EScoreType::Time;
					pLeaderboard->UpdateScore(leaderboardId, GetPortInt(pActInfo, eIn_Score), scoreType, GetPortBool(pActInfo, eIn_Force));
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not parse Leaderboard Identifier");
				}
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Leaderboard:GetData", CFlowNode_GetLeaderboardData);
REGISTER_FLOW_NODE("GamePlatform:Leaderboard:UpdateScore", CFlowNode_UpdateLeaderboardScore);

} // Cry::GamePlatform
