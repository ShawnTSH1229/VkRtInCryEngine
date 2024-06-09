// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Events/OnLeaderboardEntryDownloaded.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnLeaderboardEntryDownloaded
	: public CFlowListenerBaseNode<COnLeaderboardEntryDownloaded>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LeaderboardId = eOut_CustomOutput,
		eOut_Playername,
		eOut_Rank,
		eOut_Score,
		eOut_IsTimeBased
	};

public:

	CFlowNode_OnLeaderboardEntryDownloaded(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLeaderboardEntryDownloaded() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLeaderboardEntryDownloaded(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform