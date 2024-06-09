// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <Events/OnCatalogItemsReceived.h>
#include <Events/OnCatalogReceived.h>
#include <Events/OnMicroTransactionAuthorizationResponse.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnCatalogItemsReceived
	: public CFlowListenerBaseNode<COnCatalogItemsReceived>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Success = eOut_CustomOutput,
		eOut_ContainerId
	};

public:

	CFlowNode_OnCatalogItemsReceived(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnCatalogItemsReceived() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnCatalogItemsReceived(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnCatalogReceived
	: public CFlowListenerBaseNode<COnCatalogReceived>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Success = eOut_CustomOutput,
		eOut_HasRemainingItems,
		eOut_ContainerId
	};

public:

	CFlowNode_OnCatalogReceived(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnCatalogReceived() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnCatalogReceived(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnMicroTransactionAuthorizationResponse
	: public CFlowListenerBaseNode<COnMicroTransactionAuthorizationResponse>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_OrderId = eOut_CustomOutput,
		eOut_Authorized
	};

public:

	CFlowNode_OnMicroTransactionAuthorizationResponse(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnMicroTransactionAuthorizationResponse() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnMicroTransactionAuthorizationResponse(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform