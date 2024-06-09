// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <queue>
#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_AddAccountToLocalSession : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Add,
		eIn_Platform,
		eIn_AccountId,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_AddAccountToLocalSession(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_AddAccountToLocalSession() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_AuthenticateUser : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Authenticate,
		eIn_Platform,
		eIn_ClientIP,
		eIn_ClientAuthToken
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_ClientAccountId
	};

public:

	CFlowNode_AuthenticateUser(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_AuthenticateUser() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

protected:

	bool ParseIPString(const string& ipIN, uint32& ipOUT) const;

};

class CFlowNode_CanAccessMultiplayer : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_Response,
	};

public:

	CFlowNode_CanAccessMultiplayer(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_CanAccessMultiplayer() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_CloseSession : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Close,
		eIn_Platform,
		eIn_AccountId
	};

public:

	CFlowNode_CloseSession(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_CloseSession() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_CreateServer : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Create,
		eIn_Platform,
		eIn_LocalOnly
	};
	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_CreateServer(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_CreateServer() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_FilterText : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Filter,
		eIn_Platform,
		eIn_Text
	};
	enum EOutputs
	{
		eOut_Success,
		eOut_FilteredText
	};

public:

	CFlowNode_FilterText(SActivationInfo* pActInfo) : m_outputs() {}
	virtual ~CFlowNode_FilterText() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_FilterText(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	std::queue<string> m_outputs;

};

class CFlowNode_GetAuthToken : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_AuthToken,
		eOut_IssuerId,
	};

public:

	CFlowNode_GetAuthToken(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetAuthToken() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetConnectionStatus : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_Disconnected,
		eOut_Connecting,
		eOut_ObtainingIP,
		eOut_Connected,
	};

public:

	CFlowNode_GetConnectionStatus(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetConnectionStatus() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_IsLoggedIn : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_IsLoggedIn(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_IsLoggedIn() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_IsPacketAvailable : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform
	};

	enum EOutputs
	{
		eOut_Available,
		eOut_DataSize
	};

public:

	CFlowNode_IsPacketAvailable(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_IsPacketAvailable() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ReadPacket : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Read,
		eIn_Platform,
		eIn_MaxReadLength
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_SourceAccountId,
		eOut_BytesRead,
		eOut_Data
	};

public:

	CFlowNode_ReadPacket(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReadPacket() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RemoveAccountFromLocalSession : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Remove,
		eIn_Platform,
		eIn_AccountId,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_RemoveAccountFromLocalSession(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RemoveAccountFromLocalSession() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SendPacket : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Send,
		eIn_Platform,
		eIn_AccountId,
		eIn_Data
	};

	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_SendPacket(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_SendPacket() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SendUserDisconnect : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_SendDisconnected,
		eIn_Platform,
		eIn_AccountId
	};

public:

	CFlowNode_SendUserDisconnect(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_SendUserDisconnect() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ServerInfo : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_PublicIP,
		eOut_Port,
		eOut_IsLocal
	};

public:

	CFlowNode_ServerInfo(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ServerInfo() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SetIsInMultiplayer : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Set,
		eIn_Platform,
		eIn_IsInMultiplayer
	};

public:

	CFlowNode_SetIsInMultiplayer(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_SetIsInMultiplayer() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
