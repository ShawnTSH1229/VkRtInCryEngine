// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_CanOpenPurchaseOverlay : public CFlowBaseNode<eNCT_Singleton>
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

	CFlowNode_CanOpenPurchaseOverlay(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_CanOpenPurchaseOverlay() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_OpenBrowser : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_OpenBrowser,
		eIn_Platform,
		eIn_Address,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_OpenBrowser(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_OpenBrowser() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_OpenDialog : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_OpenDialog,
		eIn_Platform,
		eIn_DialogType,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_OpenDialog(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_OpenDialog() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_OpenDialogWithTargetUser : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_OpenDialog,
		eIn_Platform,
		eIn_TargetAccountId,
		eIn_DialogType,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_OpenDialogWithTargetUser(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_OpenDialogWithTargetUser() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_OpenPurchaseOverlay : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Open,
		eIn_Platform,
		eIn_ProductId,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_OpenPurchaseOverlay(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_OpenPurchaseOverlay() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
