// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_GetApplicationId : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_ApplicationId,
	};

public:

	CFlowNode_GetApplicationId(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetApplicationId() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_GetBuildId : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_BuildId,
	};

public:

	CFlowNode_GetBuildId(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetBuildId() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_GetEnvironmentValue : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_Name,
	};

	enum EOutputs
	{
		eOut_Value,
	};

public:

	CFlowNode_GetEnvironmentValue(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetEnvironmentValue() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_GetService : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
	};

	enum EOutputs
	{
		eOut_IsAvailable,
		eOut_Name,
		eOut_UID,
	};

public:

	CFlowNode_GetService(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetService() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_MarkContentCorrupt : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Set,
		eIn_Platform,
		eIn_OnlyMissingFiles
	};

public:

	CFlowNode_MarkContentCorrupt(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_MarkContentCorrupt() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_OwnsApplication : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
		eIn_ApplicationId,
	};

	enum EOutputs
	{
		eOut_OwnsApplication,
	};

public:

	CFlowNode_OwnsApplication(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_OwnsApplication() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

} // Cry::GamePlatform
