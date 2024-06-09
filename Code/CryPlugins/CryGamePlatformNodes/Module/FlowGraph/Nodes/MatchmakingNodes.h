// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <queue>
#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_AddLobbyQueryFilterString
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Add,
		eIn_Platform,
		eIn_Key,
		eIn_Value,
		eIn_Comparator
	};

public:

	CFlowNode_AddLobbyQueryFilterString(SActivationInfo*) {}
	~CFlowNode_AddLobbyQueryFilterString() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_CreateLobby
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Create,
		eIn_Platform,
		eIn_Visibility,
		eIn_MaxMembers
	};

	enum EOutputPorts
	{
		eOut_Success
	};

public:

	CFlowNode_CreateLobby(SActivationInfo*) {}
	~CFlowNode_CreateLobby() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetQueriedLobbyIdByIndex
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_Index
	};

	enum EOutputPorts
	{
		eOut_LobbyId
	};

public:

	CFlowNode_GetQueriedLobbyIdByIndex(SActivationInfo*) {}
	~CFlowNode_GetQueriedLobbyIdByIndex() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetUserLobby
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Get,
		eIn_Platform,
		eIn_AccountId
	};

	enum EOutputPorts
	{
		eOut_Success,
		eOut_LobbyId
	};

public:

	CFlowNode_GetUserLobby(SActivationInfo*) {}
	~CFlowNode_GetUserLobby() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_JoinLobby
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Join,
		eIn_Platform,
		eIn_LobbyId
	};

public:

	CFlowNode_JoinLobby(SActivationInfo*) {}
	~CFlowNode_JoinLobby() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_QueryLobbies
	: public CFlowBaseNode<eNCT_Singleton>
{

protected:

	enum EInputPorts
	{
		eIn_Query,
		eIn_Platform
	};

public:

	CFlowNode_QueryLobbies(SActivationInfo*) {}
	~CFlowNode_QueryLobbies() {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
