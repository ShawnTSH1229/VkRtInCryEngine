// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "NetworkingEventNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnAuthTokenReceived::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Success", _HELP("Whether the auth token request was successful.")),
			OutputPortConfig<string>("AuthToken", _HELP("The string representation of the auth token received.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more auth token requests received a response since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnAuthTokenReceived::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	ActivateOutput(pActInfo, eOut_Success, event.IsSuccessful());
	ActivateOutput(pActInfo, eOut_Token, string(event.GetToken().c_str()));
}

void CFlowNode_OnGetSteamAuthTicketResponse::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Success", _HELP("Whether the auth ticket request was successful.")),
			OutputPortConfig<int>("AuthToken", _HELP("The integer representation of the auth ticket.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more auth tickets were received from Steam since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnGetSteamAuthTicketResponse::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	ActivateOutput(pActInfo, eOut_Success, event.IsSuccessful());
	ActivateOutput(pActInfo, eOut_Ticket, static_cast<int>(event.GetTicket()));
}

void CFlowNode_OnLogInStateChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier of the account that changed state.")),
			OutputPortConfig<bool>("IsLoggedIn", _HELP("True if the account was logged in, false otherwise.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts changed their login state since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLogInStateChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);
	ActivateOutput(pActInfo, eOut_IsLoggedIn, event.IsLoggedIn());
}

void CFlowNode_OnNetworkStatusChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig_Void("Connected", _HELP("Fired on matching connection status.")),
			OutputPortConfig_Void("Connecting", _HELP("Fired on matching connection status.")),
			OutputPortConfig_Void("Disconnected", _HELP("Fired on matching connection status.")),
			OutputPortConfig_Void("ObtainingIP", _HELP("Fired on matching connection status.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have been added since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnNetworkStatusChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	bool connected = false, connecting = false, disconnected = false, obtainingIP = false;

	switch (event.GetConnectionStatus())
	{
	case EConnectionStatus::Connected:
		connected = true;
		break;
	case EConnectionStatus::Connecting:
		connecting = true;
		break;
	case EConnectionStatus::Disconnected:
		disconnected = true;
		break;
	case EConnectionStatus::ObtainingIP:
		obtainingIP = true;
		break;
	}

	ActivateOutput(pActInfo, eOut_Connected, connected);
	ActivateOutput(pActInfo, eOut_Connecting, connecting);
	ActivateOutput(pActInfo, eOut_Disconnected, disconnected);
	ActivateOutput(pActInfo, eOut_ObtainingIP, obtainingIP);
}

REGISTER_FLOW_NODE("GamePlatform:Listener:Networking:OnAuthTokenReceived", CFlowNode_OnAuthTokenReceived);
REGISTER_FLOW_NODE("GamePlatform:Listener:Networking:OnGetSteamAuthTicketResponse", CFlowNode_OnGetSteamAuthTicketResponse);
REGISTER_FLOW_NODE("GamePlatform:Listener:Networking:OnLogInStateChanged", CFlowNode_OnLogInStateChanged);
REGISTER_FLOW_NODE("GamePlatform:Listener:Networking:OnNetworkStatusChanged", CFlowNode_OnNetworkStatusChanged);

} // Cry::GamePlatform
