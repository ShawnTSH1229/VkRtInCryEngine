// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "LeaderboardEventNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnLeaderboardEntryDownloaded::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LeaderboardId", _HELP("The unique identifier of the leaderboard itself.")),
			OutputPortConfig<string>("PlayerName", _HELP("The name of the player associated with this entry.")),
			OutputPortConfig<int>("Rank", _HELP("What rank this entry is on the leaderboard.")),
			OutputPortConfig<int>("Score", _HELP("The value of the entry.")),
			OutputPortConfig<bool>("IsTimeBased", _HELP("True if the type of this entry is time based, False if this entry is score based.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more Leaderboard requests received a response since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLeaderboardEntryDownloaded::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string leaderboardId;
	event.GetIdentifier().GetAsString(leaderboardId);

	ActivateOutput(pActInfo, eOut_LeaderboardId, leaderboardId);
	ActivateOutput(pActInfo, eOut_Playername, string(event.GetPlayerName().c_str()));
	ActivateOutput(pActInfo, eOut_Rank, static_cast<int>(event.GetRank()));
	ActivateOutput(pActInfo, eOut_Score, static_cast<int>(event.GetScore()));
	ActivateOutput(pActInfo, eOut_IsTimeBased, event.GetScoreType() == ILeaderboards::EScoreType::Time);
}

REGISTER_FLOW_NODE("GamePlatform:Listener:Leaderboard:OnLeaderboardEntryDownloaded", CFlowNode_OnLeaderboardEntryDownloaded);

} // Cry::GamePlatform
