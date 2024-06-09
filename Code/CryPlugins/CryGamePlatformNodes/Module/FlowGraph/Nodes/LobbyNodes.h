// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <CryFlowGraph/IFlowBaseNode.h>
#include <queue>

namespace Cry::GamePlatform
{

class CFlowNode_GetData
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_LobbyId,
		eIn_Key
	};

	enum EOutputPorts
	{
		eOut_Success,
		eOut_Value
	};

public:

	CFlowNode_GetData(SActivationInfo*) {}
	~CFlowNode_GetData() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetMemberAtIndex
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_LobbyId,
		eIn_Index
	};

	enum EOutputPorts
	{
		eOut_AccountId
	};

public:

	CFlowNode_GetMemberAtIndex(SActivationInfo*) {}
	~CFlowNode_GetMemberAtIndex() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetMemberData
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_LobbyId,
		eIn_AccountId,
		eIn_Key
	};

	enum EOutputPorts
	{
		eOut_Success,
		eOut_Value
	};

public:

	CFlowNode_GetMemberData(SActivationInfo*) {}
	~CFlowNode_GetMemberData() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetMemberLimit
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_LobbyId
	};

	enum EOutputPorts
	{
		eOut_MemberLimit
	};

public:

	CFlowNode_GetMemberLimit(SActivationInfo*) {}
	~CFlowNode_GetMemberLimit() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetNumMembers
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_LobbyId
	};

	enum EOutputPorts
	{
		eOut_MemberCount
	};

public:

	CFlowNode_GetNumMembers(SActivationInfo*) {}
	~CFlowNode_GetNumMembers() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetOwnerId
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_LobbyId
	};

	enum EOutputPorts
	{
		eOut_AccountId
	};

public:

	CFlowNode_GetOwnerId(SActivationInfo*) {}
	~CFlowNode_GetOwnerId() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_HostServer
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Create,
		eIn_Platform,
		eIn_LobbyId,
		eIn_Level,
		eIn_IsLocal
	};

	enum EOutputPorts
	{
		eOut_Success
	};

public:

	CFlowNode_HostServer(SActivationInfo*) {}
	~CFlowNode_HostServer() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_InvitePlayers
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Invite,
		eIn_Platform,
		eIn_LobbyId,
		eIn_SystemContainerId
	};

	enum EOutputPorts
	{
		eOut_Success
	};

public:

	CFlowNode_InvitePlayers(SActivationInfo*) {}
	~CFlowNode_InvitePlayers() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_IsInServer
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Check,
		eIn_Platform,
		eIn_LobbyId
	};

	enum EOutputPorts
	{
		eOut_IsInServer
	};

public:

	CFlowNode_IsInServer(SActivationInfo*) {}
	~CFlowNode_IsInServer() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_Leave
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Leave,
		eIn_Platform,
		eIn_LobbyId
	};

public:

	CFlowNode_Leave(SActivationInfo*) {}
	~CFlowNode_Leave() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SendChatMessage
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Send,
		eIn_Platform,
		eIn_LobbyId,
		eIn_Message
	};

	enum EOutputPorts
	{
		eOut_Success
	};

public:

	CFlowNode_SendChatMessage(SActivationInfo*) {}
	~CFlowNode_SendChatMessage() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SetData
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Set,
		eIn_Platform,
		eIn_LobbyId,
		eIn_Key,
		eIn_Value
	};

	enum EOutputPorts
	{
		eOut_Success
	};

public:

	CFlowNode_SetData(SActivationInfo*) {}
	~CFlowNode_SetData() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SetLocalMemberData
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Set,
		eIn_Platform,
		eIn_LobbyId,
		eIn_Key,
		eIn_Value
	};

public:

	CFlowNode_SetLocalMemberData(SActivationInfo*) {}
	~CFlowNode_SetLocalMemberData() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ShowInviteDialog
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_ShowDialog,
		eIn_Platform,
		eIn_LobbyId
	};

public:

	CFlowNode_ShowInviteDialog(SActivationInfo*) {}
	~CFlowNode_ShowInviteDialog() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
