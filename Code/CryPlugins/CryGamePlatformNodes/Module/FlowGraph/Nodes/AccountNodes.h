// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <queue>
#include <Types/Platform.h>
#include <CryFlowGraph/IFlowBaseNode.h>
#include <CryGamePlatform/Interface/IPlatformService.h>
#include <CryGamePlatform/Interface/PlatformTypes.h>

namespace Cry::GamePlatform
{

class CFlowNode_CheckIsFriend : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
		eIn_AccountId
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_IsFriend
	};

public:

	CFlowNode_CheckIsFriend(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_CheckIsFriend() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetAccountInfo : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_AccountId
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_Nickname,
		eOut_IsLocal
	};

public:

	CFlowNode_GetAccountInfo(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetAccountInfo() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetAvatar : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_AccountId,
		eIn_Size
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_Avatar
	};

public:

	CFlowNode_GetAvatar(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetAvatar() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_GetAvatar(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

protected:

	EAvatarSize UIConfigToSize(int uicValue) const;
	const char* AvatarSizeToString(EAvatarSize eSize) const;

};

class CFlowNode_GetBlockedAccounts : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Retrieve,
		eIn_GetNext,
		eIn_Platform
	};

	enum EOutputs
	{
		eOut_Remaining,
		eOut_AccountId
	};

public:

	CFlowNode_GetBlockedAccounts(SActivationInfo* pActInfo) : m_blockedUsers() {}
	virtual ~CFlowNode_GetBlockedAccounts() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_GetBlockedAccounts(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	std::queue<AccountIdentifier> m_blockedUsers;

};

class CFlowNode_GetFriends : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Retrieve,
		eIn_GetNext,
		eIn_Platform
	};

	enum EOutputs
	{
		eOut_Remaining,
		eOut_AccountId
	};

public:

	CFlowNode_GetFriends(SActivationInfo* pActInfo) : m_friends() {}
	virtual ~CFlowNode_GetFriends() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_GetFriends(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	std::queue<AccountIdentifier> m_friends;

};

class CFlowNode_GetLocalAccount : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_AccountId,
	};

public:

	CFlowNode_GetLocalAccount(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetLocalAccount() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetMutedAccounts : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Retrieve,
		eIn_GetNext,
		eIn_Platform
	};

	enum EOutputs
	{
		eOut_Remaining,
		eOut_AccountId
	};

public:

	CFlowNode_GetMutedAccounts(SActivationInfo* pActInfo) : m_mutedUsers() {}
	virtual ~CFlowNode_GetMutedAccounts() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_GetMutedAccounts(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	std::queue<AccountIdentifier> m_mutedUsers;

};

class CFlowNode_GetPermission : public CFlowBaseNode<eNCT_Singleton>
{

	using EPermission = IService::EPermission;
	using EPermissionResult = IService::EPermissionResult;

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
		eIn_PermissionType,
	};

	enum EOutputs
	{
		eOut_Granted,
		eOut_Denied,
		eOut_Pending,
		eOut_NotApplicable
	};

public:

	CFlowNode_GetPermission(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetPermission() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetPrivacyPermission : public CFlowBaseNode<eNCT_Singleton>
{

	using EPrivacyPermission = IService::EPrivacyPermission;
	using EPermissionResult = IService::EPermissionResult;

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
		eIn_TargetAccountId,
		eIn_PermissionType
	};

	enum EOutputs
	{
		eOut_Granted,
		eOut_Denied,
		eOut_Pending,
		eOut_NotApplicable
	};

public:

	CFlowNode_GetPrivacyPermission(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetPrivacyPermission() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RefreshBlockedAccounts : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Refresh,
		eIn_Platform,
	};

public:

	CFlowNode_RefreshBlockedAccounts(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RefreshBlockedAccounts() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RefreshMutedAccounts : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Refresh,
		eIn_Platform,
	};

public:

	CFlowNode_RefreshMutedAccounts(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RefreshMutedAccounts() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RefreshPermissions : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Refresh,
		eIn_Platform,
	};

public:

	CFlowNode_RefreshPermissions(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RefreshPermissions() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RefreshPrivacyPermissions : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Refresh,
		eIn_Platform,
		eIn_SystemContainerId
	};

public:

	CFlowNode_RefreshPrivacyPermissions(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RefreshPrivacyPermissions() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RequestUserInformation : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Request,
		eIn_Platform,
		eIn_AccountId,
		eIn_Avatar,
	};

	enum EOutputs
	{
		eOut_NeedWait,
	};

public:

	CFlowNode_RequestUserInformation(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RequestUserInformation() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_RichPresence : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Set,
		eIn_Clear,
		eIn_Platform,
		eIn_Activity,
		eIn_Headline,
		eIn_TimerType,
		eIn_Seconds,
		eIn_PartySize,
		eIn_PartyMax,
	};

	enum EOutputs
	{
		eOut_Activity,
		eOut_Headline,
		eOut_TimerType,
		eOut_Seconds,
		eOut_PartySize,
		eOut_PartyMax,
	};

public:

	CFlowNode_RichPresence(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_RichPresence() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

protected:

	IAccount* GetAccount(EGamePlatform platform) const;
	void      OutputPresence(SActivationInfo* pActInfo, const SRichPresence& presence) const;

};

class CFlowNode_SetStatus : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Set,
		eIn_Platform,
		eIn_Status,
	};

	enum EOutputs
	{
		eOut_OnSet,
	};

public:

	CFlowNode_SetStatus(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_SetStatus() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

protected:

	IAccount* GetAccount(EGamePlatform platform) const;

};

} // Cry::GamePlatform