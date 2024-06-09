// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <Events/OnAccountAdded.h>
#include <Events/OnAccountNameChanged.h>
#include <Events/OnAccountRemoved.h>
#include <Events/OnAvatarImageLoaded.h>
#include <Events/OnPersonaStateChanged.h>
#include <Events/OnUserPermissionChanged.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnAccountAdded
	: public CFlowListenerBaseNode<COnAccountAdded>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput
	};

public:

	CFlowNode_OnAccountAdded(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnAccountAdded() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnAccountAdded(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnAccountNameChanged
	: public CFlowListenerBaseNode<COnAccountNameChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput,
		eOut_Name
	};

public:

	CFlowNode_OnAccountNameChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnAccountNameChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnAccountNameChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnAccountRemoved
	: public CFlowListenerBaseNode<COnAccountRemoved>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput
	};

public:

	CFlowNode_OnAccountRemoved(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnAccountRemoved() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnAccountRemoved(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnAvatarImageLoaded
	: public CFlowListenerBaseNode<COnAvatarImageLoaded>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput
	};

public:

	CFlowNode_OnAvatarImageLoaded(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnAvatarImageLoaded() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnAvatarImageLoaded(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnPersonaStateChanged
	: public CFlowListenerBaseNode<COnPersonaStateChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput,

		eOut_Name,
		eOut_Status,
		eOut_CameOnline,
		eOut_WentOffline,
		eOut_GamePlayed,
		eOut_GameServer,
		eOut_ChangeAvatar,
		eOut_JoinedSource,
		eOut_LeftSource,
		eOut_RelationshipChanged,
		eOut_BlockedStateChanged,
		eOut_MutedStateChanged,
		eOut_PrivacyStateChanged,
		eOut_NameFirstSet,
		eOut_FacebookInfo,
		eOut_Nickname,
		eOut_SteamLevel,
		eOut_RichPresence
	};

public:

	CFlowNode_OnPersonaStateChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnPersonaStateChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnPersonaStateChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnUserPermissionChanged
	: public CFlowListenerBaseNode<COnUserPermissionChanged>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_AccountId = eOut_CustomOutput,
		eOut_HasPermission,

		eOut_PermCommunication,
		eOut_PermMultiplayer,
		eOut_PermViewProfiles,
		eOut_PermWebBrowser,
		eOut_PermUserGeneratedContent,
		eOut_PermMultiplayerSubscription,
		eOut_PermCrossNetworkPlay
	};

public:

	CFlowNode_OnUserPermissionChanged(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnUserPermissionChanged() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnUserPermissionChanged(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

} // Cry::GamePlatform
