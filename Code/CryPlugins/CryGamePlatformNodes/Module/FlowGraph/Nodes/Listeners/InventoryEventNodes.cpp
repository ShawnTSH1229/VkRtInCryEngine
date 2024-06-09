// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "InventoryEventNodes.h"
#include "../ContainerBaseNode.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnInventoryItemConsumed::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Success", _HELP("Whether consumption of the inventory item was successful.")),
			OutputPortConfig<string>("Identifier", _HELP("The unique identifier of the inventory item.")),
			OutputPortConfig<int>("Quantity", _HELP("The Quantity of the items that were consumed.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more inventory items have been consumed since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnInventoryItemConsumed::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string id;
	event.GetIdentifier().GetAsString(id);

	ActivateOutput(pActInfo, eOut_Success, event.IsSuccessful());
	ActivateOutput(pActInfo, eOut_Identifier, id);
	ActivateOutput(pActInfo, eOut_Quantity, event.GetQuantity());
}

void CFlowNode_OnInventoryReceived::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Success", _HELP("Whether the request was successful and this event contains valid data.")),
			OutputPortConfig<int>("ContainerId", _HELP("The id of the container that was created (use with Container:InventoryItem).")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more inventory requests were received since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnInventoryReceived::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	// Create a new container with all the items
	int containerId = CFlowContainerBaseNode<IService::SInventoryItem>::AddList(event.GetItems());

	ActivateOutput(pActInfo, eOut_Success, event.IsSuccessful());
	ActivateOutput(pActInfo, eOut_ContainerId, containerId);
}

void CFlowNode_OnLicensesChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<int>("ContainerId", _HELP("The id of the container that was created (use with Container:License).")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more licenses to an item have been changed/added or removed since the last frame. NOTE: Each event node will copy the entire event data (with a unique container), be sure to delete each container.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLicensesChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	const int containerId = CFlowContainerBaseNode<InventoryItemIdentifier>::AddList(event.GetLicenses());
	ActivateOutput(pActInfo, eOut_ContainerId, containerId);
}

REGISTER_FLOW_NODE("GamePlatform:Listener:OwnedItems:OnInventoryItemConsumed", CFlowNode_OnInventoryItemConsumed);
REGISTER_FLOW_NODE("GamePlatform:Listener:OwnedItems:OnInventoryReceived", CFlowNode_OnInventoryReceived);
REGISTER_FLOW_NODE("GamePlatform:Listener:Inventory:OnLicensesChanged", CFlowNode_OnLicensesChanged);

} // Cry::GamePlatform
