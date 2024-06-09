// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_ConsumeInventoryItem : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Query,
		eIn_Platform,
		eIn_ItemId,
		eIn_Quantity
	};

public:

	CFlowNode_ConsumeInventoryItem(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ConsumeInventoryItem() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_QueryInventory : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Query,
		eIn_Platform
	};

public:

	CFlowNode_QueryInventory(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_QueryInventory() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

} // Cry::GamePlatform
