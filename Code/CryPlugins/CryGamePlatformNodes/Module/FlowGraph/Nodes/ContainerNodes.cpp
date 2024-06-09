// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "ContainerNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_CatalogItemDetailsNode::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static bool isConfigured = false;

	if (!isConfigured)
	{
		Base::GetConfigurationBase(m_inputs, m_outputs);
		m_outputs.push_back(OutputPortConfig<string>("AppId", _HELP("Application Identifier.")));
		m_outputs.push_back(OutputPortConfig<string>("Name", _HELP("Item Name.")));
		m_outputs.push_back(OutputPortConfig<bool>("IsConsumable", _HELP("True if the item is a consumable product.")));
		m_outputs.push_back(OutputPortConfig<bool>("IsDLC", _HELP("True if the item is a DLC product.")));
		m_outputs.push_back(OutputPortConfig<string>("Description", _HELP("The description of the product.")));
		m_outputs.push_back(OutputPortConfig<bool>("IsPurchasable", _HELP("Whether the user is able to purchase the product.")));
		m_outputs.push_back(OutputPortConfig<string>("CurrencyCode", _HELP("The currency code for the price stated on the product.")));
		m_outputs.push_back(OutputPortConfig<float>("Price", _HELP("The price of the product including any discounts.")));
		m_outputs.push_back(OutputPortConfig<float>("RawPrice", _HELP("The price of the product without any extra discounts.")));
		m_outputs.push_back(OutputPortConfig<string>("DisplayPrice", _HELP("The display price as a string including any discounts.")));
		m_outputs.push_back(OutputPortConfig<string>("RawDisplayPrice", _HELP("The display price as a string without any discounts applied.")));
		m_outputs.push_back(OutputPortConfig<int>("MaxQuantity", _HELP("How many of this product the user can purchase.")));

		m_inputs.push_back({ 0 });
		m_outputs.push_back({ 0 });

		isConfigured = true;
	}

	nodeConfig.pInputPorts = m_inputs.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Container node for iterating over CatalogItemDetails received via the OnCatalogItemsReceived node.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CatalogItemDetailsNode::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	Base::ProcessEventBase(this, event, pActInfo);

	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_NextElement))
	{
		TryGetElement(pActInfo);
	}
}

void CFlowNode_CatalogItemDetailsNode::TryGetElement(SActivationInfo* pActInfo)
{
	if (Base::ContainerExists(m_containerId))
	{
		const int listSize = Base::GetListSize(m_containerId);
		ActivateOutput(pActInfo, eOut_Count, listSize);

		if (auto pElement = Base::GetListElement(m_containerId, m_curIndex))
		{
			ActivateOutput(pActInfo, eOut_Remaining, listSize - (m_curIndex + 1));

			string appId;
			pElement->id.GetAsString(appId);
			ActivateOutput(pActInfo, eOut_AppId, appId);

			ActivateOutput(pActInfo, eOut_Name, pElement->name);

			const bool isConsumable = (pElement->type == IService::EProductType::Consumable) ? true : false;
			ActivateOutput(pActInfo, eOut_IsConsumable, isConsumable);

			const bool isDLC = (pElement->type == IService::EProductType::DLC) ? true : false;
			ActivateOutput(pActInfo, eOut_IsDLC, isDLC);

			ActivateOutput(pActInfo, eOut_Description, pElement->description);
			ActivateOutput(pActInfo, eOut_IsPurchasable, pElement->isPurchasable);
			ActivateOutput(pActInfo, eOut_CurrencyCode, pElement->currencyCode);
			ActivateOutput(pActInfo, eOut_Price, static_cast<float>(pElement->price));
			ActivateOutput(pActInfo, eOut_RawPrice, static_cast<float>(pElement->listPrice));
			ActivateOutput(pActInfo, eOut_DisplayPrice, pElement->displayPrice);
			ActivateOutput(pActInfo, eOut_RawDisplayPrice, pElement->displayListPrice);
			ActivateOutput(pActInfo, eOut_MaxQuantity, pElement->consumableQuantity);
		}
	}
}

void CFlowNode_CatalogItemsNode::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static bool isConfigured = false;

	if (!isConfigured)
	{
		Base::GetConfigurationBase(m_inputs, m_outputs);
		m_outputs.push_back(OutputPortConfig<string>("ApplicationId", _HELP("Application Identifier.")));
		m_outputs.push_back(OutputPortConfig<string>("Name", _HELP("Item Name.")));
		m_outputs.push_back(OutputPortConfig<bool>("IsConsumable", _HELP("True if the item is a consumable product.")));
		m_outputs.push_back(OutputPortConfig<bool>("IsDLC", _HELP("True if the item is a DLC product.")));

		m_inputs.push_back({ 0 });
		m_outputs.push_back({ 0 });

		isConfigured = true;
	}

	nodeConfig.pInputPorts = m_inputs.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Container node for iterating over CatalogItems received via the OnCatalogReceived node.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CatalogItemsNode::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	Base::ProcessEventBase(this, event, pActInfo);

	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_NextElement))
	{
		TryGetElement(pActInfo);
	}
}

void CFlowNode_CatalogItemsNode::TryGetElement(SActivationInfo* pActInfo)
{
	if (Base::ContainerExists(m_containerId))
	{
		const int listSize = Base::GetListSize(m_containerId);
		ActivateOutput(pActInfo, eOut_Count, listSize);

		if (auto pElement = Base::GetListElement(m_containerId, m_curIndex))
		{
			ActivateOutput(pActInfo, eOut_Remaining, listSize - (m_curIndex + 1));

			string appId;
			pElement->id.GetAsString(appId);
			ActivateOutput(pActInfo, eOut_AppId, appId);

			ActivateOutput(pActInfo, eOut_Name, pElement->name);

			const bool isConsumable = (pElement->type == IService::EProductType::Consumable) ? true : false;
			ActivateOutput(pActInfo, eOut_IsConsumable, isConsumable);

			const bool isDLC = (pElement->type == IService::EProductType::DLC) ? true : false;
			ActivateOutput(pActInfo, eOut_IsDLC, isDLC);
		}
	}
}

void CFlowNode_InventoryItemsNode::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static bool isConfigured = false;

	if (!isConfigured)
	{
		Base::GetConfigurationBase(m_inputs, m_outputs);
		m_outputs.push_back(OutputPortConfig<string>("ItemId", _HELP("Unique item identifier.")));
		m_outputs.push_back(OutputPortConfig<string>("ProductId", _HELP("Associated product ID. Can be empty.")));
		m_outputs.push_back(OutputPortConfig<int>("Quantity", _HELP("Quantity of the inventory item.")));

		m_inputs.push_back({ 0 });
		m_outputs.push_back({ 0 });

		isConfigured = true;
	}

	nodeConfig.pInputPorts = m_inputs.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Container node for iterating over InvetoryItems received via the OnInventoryReceived node.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_InventoryItemsNode::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	Base::ProcessEventBase(this, event, pActInfo);

	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_NextElement))
	{
		TryGetElement(pActInfo);
	}
}

void CFlowNode_InventoryItemsNode::TryGetElement(SActivationInfo* pActInfo)
{
	if (Base::ContainerExists(m_containerId))
	{
		const int listSize = Base::GetListSize(m_containerId);
		ActivateOutput(pActInfo, eOut_Count, listSize);

		if (auto pElement = Base::GetListElement(m_containerId, m_curIndex))
		{
			ActivateOutput(pActInfo, eOut_Remaining, listSize - (m_curIndex + 1));

			string itemId;
			pElement->id.GetAsString(itemId);
			ActivateOutput(pActInfo, eOut_ItemId, itemId);

			string productId;
			pElement->productId.GetAsString(productId);
			ActivateOutput(pActInfo, eOut_ProductId, productId);

			ActivateOutput(pActInfo, eOut_Quantity, static_cast<int>(pElement->quantity));
		}
	}
}

void CFlowNode_LicensesNode::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static bool isConfigured = false;

	if (!isConfigured)
	{
		Base::GetConfigurationBase(m_inputs, m_outputs);
		m_outputs.push_back(OutputPortConfig<string>("LicenseId", _HELP("Unique item license identifier.")));

		m_inputs.push_back({ 0 });
		m_outputs.push_back({ 0 });

		isConfigured = true;
	}

	nodeConfig.pInputPorts = m_inputs.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Container node for iterating over item licenses received via the OnLicensesChanged node.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_LicensesNode::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	Base::ProcessEventBase(this, event, pActInfo);

	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_NextElement))
	{
		TryGetElement(pActInfo);
	}
}

void CFlowNode_LicensesNode::TryGetElement(SActivationInfo* pActInfo)
{
	if (Base::ContainerExists(m_containerId))
	{
		const int listSize = Base::GetListSize(m_containerId);
		ActivateOutput(pActInfo, eOut_Count, listSize);

		if (auto pElement = Base::GetListElement(m_containerId, m_curIndex))
		{
			ActivateOutput(pActInfo, eOut_Remaining, listSize - (m_curIndex + 1));

			string licenseId;
			pElement->id.GetAsString(licenseId);
			ActivateOutput(pActInfo, eOut_LicenseId, licenseId);
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Containers:CatalogItemDetail", CFlowNode_CatalogItemDetailsNode);
REGISTER_FLOW_NODE("GamePlatform:Containers:CatalogItem", CFlowNode_CatalogItemsNode);
REGISTER_FLOW_NODE("GamePlatform:Containers:InventoryItem", CFlowNode_InventoryItemsNode);
REGISTER_FLOW_NODE("GamePlatform:Containers:License", CFlowNode_LicensesNode);

} // Cry::GamePlatform
