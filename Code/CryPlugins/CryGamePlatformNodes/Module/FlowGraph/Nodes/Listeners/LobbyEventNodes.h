// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <Events/OnChatMessage.h>
#include <Events/OnGameCreated.h>
#include <Events/OnLeave.h>
#include <Events/OnLobbyDataUpdate.h>
#include <Events/OnPlayerBanned.h>
#include <Events/OnPlayerDisconnected.h>
#include <Events/OnPlayerEntered.h>
#include <Events/OnPlayerKicked.h>
#include <Events/OnPlayerLeft.h>
#include <Events/OnUserDataUpdate.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnChatMessage
	: public CFlowListenerBaseNode<COnChatMessage>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId,
		eOut_Message
	};

public:

	CFlowNode_OnChatMessage(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnChatMessage() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnChatMessage(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void                HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnGameCreated
	: public CFlowListenerBaseNode<COnGameCreated>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_ServerId,
		eOut_IPAddress,
		eOut_Port,
		eOut_IsLocal
	};

public:

	CFlowNode_OnGameCreated(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnGameCreated() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnGameCreated(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnLeave
	: public CFlowListenerBaseNode<COnLeave>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput
	};

public:

	CFlowNode_OnLeave(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLeave() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLeave(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnLobbyDataUpdate
	: public CFlowListenerBaseNode<COnLobbyDataUpdate>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput
	};

public:

	CFlowNode_OnLobbyDataUpdate(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnLobbyDataUpdate() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnLobbyDataUpdate(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPlayerBanned
	: public CFlowListenerBaseNode<COnPlayerBanned>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId,
		eOut_ModeratorId
	};

public:

	CFlowNode_OnPlayerBanned(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPlayerBanned() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPlayerBanned(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPlayerDisconnected
	: public CFlowListenerBaseNode<COnPlayerDisconnected>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId
	};

public:

	CFlowNode_OnPlayerDisconnected(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPlayerDisconnected() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPlayerDisconnected(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPlayerEntered
	: public CFlowListenerBaseNode<COnPlayerEntered>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId
	};

public:

	CFlowNode_OnPlayerEntered(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPlayerEntered() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPlayerEntered(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPlayerKicked
	: public CFlowListenerBaseNode<COnPlayerKicked>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId,
		eOut_ModeratorId
	};

public:

	CFlowNode_OnPlayerKicked(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPlayerKicked() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPlayerKicked(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPlayerLeft
	: public CFlowListenerBaseNode<COnPlayerLeft>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId
	};

public:

	CFlowNode_OnPlayerLeft(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPlayerLeft() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPlayerLeft(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnUserDataUpdate
	: public CFlowListenerBaseNode<COnUserDataUpdate>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_LobbyId = eOut_CustomOutput,
		eOut_AccountId
	};

public:

	CFlowNode_OnUserDataUpdate(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnUserDataUpdate() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnUserDataUpdate(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_StartStopLobbyListener
	: public CFlowBaseNode<eNCT_Instanced>
{

protected:

	enum EInputPorts
	{
		eIn_Start,
		eIn_Stop,
		eIn_Platform,
		eIn_LobbyId
	};

	enum EOutputPorts
	{
		eOut_OnStarted,
		eOut_OnStopped
	};

public:

	CFlowNode_StartStopLobbyListener(SActivationInfo*) {}
	~CFlowNode_StartStopLobbyListener() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_StartStopLobbyListener(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform