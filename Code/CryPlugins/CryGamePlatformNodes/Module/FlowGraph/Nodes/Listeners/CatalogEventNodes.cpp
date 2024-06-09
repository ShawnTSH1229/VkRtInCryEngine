// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "CatalogEventNodes.h"
#include <FlowGraph/Nodes/ContainerBaseNode.h>

namespace Cry::GamePlatform
{

void CFlowNode_OnCatalogItemsReceived::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Success", _HELP("Whether the request was successful and this event contains valid data.")),
			OutputPortConfig<int>("ContainerId", _HELP("The id of the container that was created (use with Containers:CatalogItemDetail).")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more catalog detail requests received a response since the last frame. NOTE: Each event node will copy the entire event data (with a unique container).";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnCatalogItemsReceived::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	int containerId = CFlowContainerBaseNode<IService::SStoreItemDetails>::AddList(event.GetItems());
	ActivateOutput(pActInfo, eOut_Success, event.IsSuccessful());
	ActivateOutput(pActInfo, eOut_ContainerId, containerId);
}

void CFlowNode_OnCatalogReceived::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Success", _HELP("Whether the request was successful and this event contains valid data.")),
			OutputPortConfig<bool>("HasRemainingData", _HELP("Whether you should expect another event containing further catalog items.")),
			OutputPortConfig<int>("ContainerId", _HELP("The id of the container that was created (use with Container:CatalogItem).")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more catalog requests received a response since the last frame. NOTE: Each event node will copy the entire event data (with a unique container).";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnCatalogReceived::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	int containerId = CFlowContainerBaseNode<IService::SStoreItem>::AddList(event.GetItems());
	ActivateOutput(pActInfo, eOut_Success, event.IsSuccessful());
	ActivateOutput(pActInfo, eOut_HasRemainingItems, event.HasRemaining());
	ActivateOutput(pActInfo, eOut_ContainerId, containerId);
}

void CFlowNode_OnMicroTransactionAuthorizationResponse::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("OrderId", _HELP("The unique order id of the transaction.")),
			OutputPortConfig<bool>("Authorized", _HELP("True if the transaction was authorized, false otherwise.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more micro transaction responses were received since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnMicroTransactionAuthorizationResponse::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string orderId;
	event.GetOrderId().GetAsString(orderId);
	ActivateOutput(pActInfo, eOut_OrderId, orderId);
	ActivateOutput(pActInfo, eOut_Authorized, event.IsAuthorized());
}

REGISTER_FLOW_NODE("GamePlatform:Listener:OwnedItems:OnCatalogItemsReceived", CFlowNode_OnCatalogItemsReceived);
REGISTER_FLOW_NODE("GamePlatform:Listener:OwnedItems:OnCatalogReceived", CFlowNode_OnCatalogReceived);
REGISTER_FLOW_NODE("GamePlatform:Listener:OwnedItems:OnMicroTransactionAuthResponse", CFlowNode_OnMicroTransactionAuthorizationResponse);

} // Cry::GamePlatform
