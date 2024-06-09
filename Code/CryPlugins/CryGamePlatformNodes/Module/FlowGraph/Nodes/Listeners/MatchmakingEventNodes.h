// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <Events/OnCreatedLobby.h>
#include <Events/OnJoinedLobby.h>
#include <Events/OnLobbyJoinFailed.h>
#include <Events/OnLobbyQueryComplete.h>
#include <Events/OnPreJoinLobby.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnCreatedLobby
	: public CFlowListenerBaseNode<COnCreatedLobby>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput
	};

public:

	CFlowNode_OnCreatedLobby(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnCreatedLobby() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnCreatedLobby(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnJoinedLobby
	: public CFlowListenerBaseNode<COnJoinedLobby>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput
	};

public:

	CFlowNode_OnJoinedLobby(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnJoinedLobby() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnJoinedLobby(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnLobbyJoinFailed
	: public CFlowListenerBaseNode<COnLobbyJoinFailed>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput
	};

public:

	CFlowNode_OnLobbyJoinFailed(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLobbyJoinFailed() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLobbyJoinFailed(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnLobbyQueryComplete
	: public CFlowListenerBaseNode<COnLobbyQueryComplete>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_NumLobbiesFound = eOut_CustomOutput
	};

public:

	CFlowNode_OnLobbyQueryComplete(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLobbyQueryComplete() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLobbyQueryComplete(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPreJoinLobby
	: public CFlowListenerBaseNode<COnPreJoinLobby>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput
	};

public:

	CFlowNode_OnPreJoinLobby(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPreJoinLobby() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPreJoinLobby(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform