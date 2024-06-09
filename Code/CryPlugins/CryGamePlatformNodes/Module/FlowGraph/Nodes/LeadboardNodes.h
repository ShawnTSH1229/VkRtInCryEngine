// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_GetLeaderboardData : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_LeaderboardId,
		eIn_RequestType,
		eIn_MinRange,
		eIn_MaxRange,
		eIn_TimeBased
	};

public:

	CFlowNode_GetLeaderboardData(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetLeaderboardData() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_UpdateLeaderboardScore : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Update,
		eIn_Platform,
		eIn_LeaderboardId,
		eIn_Score,
		eIn_ScoreType,
		eIn_Force
	};

public:

	CFlowNode_UpdateLeaderboardScore(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_UpdateLeaderboardScore() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

} // Cry::GamePlatform
