// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <Events/OnAuthTokenReceived.h>
#include <Events/OnGetSteamAuthTicketResponse.h>
#include <Events/OnLogInStateChanged.h>
#include <Events/OnNetworkStatusChanged.h>
#include "ListenerBaseNode.h"

namespace Cry::GamePlatform
{

class CFlowNode_OnAuthTokenReceived
	: public CFlowListenerBaseNode<COnAuthTokenReceived>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Success = eOut_CustomOutput,
		eOut_Token
	};

public:

	CFlowNode_OnAuthTokenReceived(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnAuthTokenReceived() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnAuthTokenReceived(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnGetSteamAuthTicketResponse
	: public CFlowListenerBaseNode<COnGetSteamAuthTicketResponse>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Success = eOut_CustomOutput,
		eOut_Ticket
	};

public:

	CFlowNode_OnGetSteamAuthTicketResponse(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnGetSteamAuthTicketResponse() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnGetSteamAuthTicketResponse(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnLogInStateChanged
	: public CFlowListenerBaseNode<COnLogInStateChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput,
		eOut_IsLoggedIn
	};

public:

	CFlowNode_OnLogInStateChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLogInStateChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLogInStateChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnNetworkStatusChanged
	: public CFlowListenerBaseNode<COnNetworkStatusChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Disconnected = eOut_CustomOutput,
		eOut_Connecting,
		eOut_ObtainingIP,
		eOut_Connected
	};

public:

	CFlowNode_OnNetworkStatusChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnNetworkStatusChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnNetworkStatusChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform