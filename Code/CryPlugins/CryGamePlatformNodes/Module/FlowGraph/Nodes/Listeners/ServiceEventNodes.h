// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Events/OnEnvironmentVariableChanged.h>
#include <Events/OnOverlayActivated.h>
#include <Events/OnPlatformServiceError.h>
#include <Events/OnShutdown.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnEnvironmentVariableChanged
	: public CFlowListenerBaseNode<COnEnvironmentVariableChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_VarName = eOut_CustomOutput,
		eOut_VarValue
	};

public:

	CFlowNode_OnEnvironmentVariableChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnEnvironmentVariableChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnEnvironmentVariableChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnOverlayActivated
	: public CFlowListenerBaseNode<COnOverlayActivated>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Active = eOut_CustomOutput
	};

public:

	CFlowNode_OnOverlayActivated(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnOverlayActivated() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnOverlayActivated(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPlatformServiceError
	: public CFlowListenerBaseNode<COnPlatformServiceError>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput,
		eOut_ErrorCode
	};

public:

	CFlowNode_OnPlatformServiceError(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPlatformServiceError() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPlatformServiceError(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnShutdown
	: public CFlowListenerBaseNode<COnShutdown>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Shutdown = eOut_CustomOutput
	};

public:

	CFlowNode_OnShutdown(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnShutdown() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnShutdown(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform