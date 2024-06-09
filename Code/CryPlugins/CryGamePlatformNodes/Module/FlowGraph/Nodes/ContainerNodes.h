// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "ContainerBaseNode.h"
#include <Types/Service.h>

namespace Cry::GamePlatform
{

class CFlowNode_CatalogItemDetailsNode
	: public CFlowContainerBaseNode<IService::SStoreItemDetails>
{

	enum EOutputPorts
	{
		eOut_Count = Base::eOut_CustomOutput,
		eOut_Remaining,
		eOut_AppId,
		eOut_Name,
		eOut_IsConsumable,
		eOut_IsDLC,
		eOut_Description,
		eOut_IsPurchasable,
		eOut_CurrencyCode,
		eOut_Price,
		eOut_RawPrice,
		eOut_DisplayPrice,
		eOut_RawDisplayPrice,
		eOut_MaxQuantity
	};

public:

	CFlowNode_CatalogItemDetailsNode(SActivationInfo* pActInfo) : Base(pActInfo) {}
	~CFlowNode_CatalogItemDetailsNode() {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return IFlowNodePtr(new CFlowNode_CatalogItemDetailsNode(pActInfo)); }
	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); }

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	void TryGetElement(SActivationInfo* pActInfo);

private:

	static std::vector<SInputPortConfig> m_inputs;
	static std::vector<SOutputPortConfig> m_outputs;

};

std::vector<SInputPortConfig> CFlowNode_CatalogItemDetailsNode::m_inputs = {};
std::vector<SOutputPortConfig> CFlowNode_CatalogItemDetailsNode::m_outputs = {};

class CFlowNode_CatalogItemsNode
	: public CFlowContainerBaseNode<IService::SStoreItem>
{

	enum EOutputPorts
	{
		eOut_AppId = Base::eOut_CustomOutput,
		eOut_Name,
		eOut_IsConsumable,
		eOut_IsDLC
	};

public:

	CFlowNode_CatalogItemsNode(SActivationInfo* pActInfo) : Base(pActInfo) {}
	~CFlowNode_CatalogItemsNode() {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return IFlowNodePtr(new CFlowNode_CatalogItemsNode(pActInfo)); }
	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); }

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	void TryGetElement(SActivationInfo* pActInfo);

private:

	static std::vector<SInputPortConfig> m_inputs;
	static std::vector<SOutputPortConfig> m_outputs;

};

std::vector<SInputPortConfig> CFlowNode_CatalogItemsNode::m_inputs = {};
std::vector<SOutputPortConfig> CFlowNode_CatalogItemsNode::m_outputs = {};

class CFlowNode_InventoryItemsNode
	: public CFlowContainerBaseNode<IService::SInventoryItem>
{

	enum EOutputPorts
	{
		eOut_Count = Base::eOut_CustomOutput,
		eOut_Remaining,
		eOut_ItemId,
		eOut_ProductId,
		eOut_Quantity
	};

public:

	CFlowNode_InventoryItemsNode(SActivationInfo* pActInfo) : Base(pActInfo) {}
	~CFlowNode_InventoryItemsNode() {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return IFlowNodePtr(new CFlowNode_InventoryItemsNode(pActInfo)); }
	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); }

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	void TryGetElement(SActivationInfo* pActInfo);

private:

	static std::vector<SInputPortConfig> m_inputs;
	static std::vector<SOutputPortConfig> m_outputs;

};

std::vector<SInputPortConfig> CFlowNode_InventoryItemsNode::m_inputs = {};
std::vector<SOutputPortConfig> CFlowNode_InventoryItemsNode::m_outputs = {};

class CFlowNode_LicensesNode
	: public CFlowContainerBaseNode<IService::SInventoryItem>
{

	enum EOutputPorts
	{
		eOut_Count = Base::eOut_CustomOutput,
		eOut_Remaining,
		eOut_LicenseId
	};

public:

	CFlowNode_LicensesNode(SActivationInfo* pActInfo) : Base(pActInfo) {}
	~CFlowNode_LicensesNode() {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return IFlowNodePtr(new CFlowNode_LicensesNode(pActInfo)); }
	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); }

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	void TryGetElement(SActivationInfo* pActInfo);

private:

	static std::vector<SInputPortConfig> m_inputs;
	static std::vector<SOutputPortConfig> m_outputs;

};

std::vector<SInputPortConfig> CFlowNode_LicensesNode::m_inputs = {};
std::vector<SOutputPortConfig> CFlowNode_LicensesNode::m_outputs = {};

} // Cry::GamePlatform
