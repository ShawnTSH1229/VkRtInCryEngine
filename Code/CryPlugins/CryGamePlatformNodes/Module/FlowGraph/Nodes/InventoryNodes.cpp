// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "InventoryNodes.h"

#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>

namespace Cry::GamePlatform
{

void CFlowNode_ConsumeInventoryItem::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Consume", _HELP("Try to consume the specified quantity of the item.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to query against."),                                               "Platform",  _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("ItemId", _HELP("Inventory item Id to consume.")),
		InputPortConfig<int>("Quantity",  1, _HELP("Quantity of the specified item to consume. Clamped to a minimum value of 1."),  "Quantity"),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Consumes the quantity of the item specified. Listen for OnInventoryReceived events.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ConsumeInventoryItem::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Query))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			InventoryItemIdentifier inventoryItemId;
			pService->SetIdentifierFromString(inventoryItemId, GetPortString(pActInfo, eIn_ItemId));

			const uint32 quantity = static_cast<uint32>(crymath::clamp(GetPortInt(pActInfo, eIn_Quantity), 1, std::numeric_limits<int>::max()));
			if (!pService->ConsumeInventoryItem(inventoryItemId, quantity))
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not consume inventory item.");
			}
		}
	}
}

void CFlowNode_QueryInventory::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Query", _HELP("Send a request to retrieve the current inventory.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to query against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Queries the backend to retrieve inventory information. Listen for OnInventoryReceived events.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_QueryInventory::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Query))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (!pService->QueryInventory())
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Request for the current inventory failed.");
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Inventory:Consume", CFlowNode_ConsumeInventoryItem);
REGISTER_FLOW_NODE("GamePlatform:Inventory:Query", CFlowNode_QueryInventory);

} // Cry::GamePlatform
