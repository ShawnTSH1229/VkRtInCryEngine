// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include "ListenerBaseNode.h"
#include <Events/OnInventoryItemConsumed.h>
#include <Events/OnLicensesChanged.h>
#include <Events/OnInventoryReceived.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnInventoryItemConsumed
	: public CFlowListenerBaseNode<COnInventoryItemConsumed>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Success = eOut_CustomOutput,
		eOut_Identifier,
		eOut_Quantity
	};

public:

	CFlowNode_OnInventoryItemConsumed(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnInventoryItemConsumed() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnInventoryItemConsumed(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnInventoryReceived
	: public CFlowListenerBaseNode<COnInventoryReceived>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Success = eOut_CustomOutput,
		eOut_ContainerId
	};

public:

	CFlowNode_OnInventoryReceived(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnInventoryReceived() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnInventoryReceived(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnLicensesChanged
	: public CFlowListenerBaseNode<COnLicensesChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_ContainerId = eOut_CustomOutput
	};

public:

	CFlowNode_OnLicensesChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLicensesChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLicensesChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform