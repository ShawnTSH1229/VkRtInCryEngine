// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "ServiceEventNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnEnvironmentVariableChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("VarName", _HELP("The name of the variable that was changed.")),
			OutputPortConfig<string>("VarValue", _HELP("The n ew value of the variable.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more environment variables were changed since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnEnvironmentVariableChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	ActivateOutput(pActInfo, eOut_VarName, string(event.GetName().c_str()));
	ActivateOutput(pActInfo, eOut_VarValue, string(event.GetValue().c_str()));
}

void CFlowNode_OnOverlayActivated::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<bool>("Active", _HELP("True if the overlay was activated/opened. False if it was deactivated/closed.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if the platform overlay was opened or closed any number of times since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnOverlayActivated::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	ActivateOutput(pActInfo, eOut_Active, event.IsActive());
}

void CFlowNode_OnPlatformServiceError::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier that received the error.")),
			OutputPortConfig<int>("ErrorCode", _HELP("The error code of the error received.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have received an error since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPlatformServiceError::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);
	ActivateOutput(pActInfo, eOut_ErrorCode, event.GetErrorCode());
}

void CFlowNode_OnShutdown::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig_Void("Shutdown", _HELP("Triggered for each shutdown event accessed.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if the service was shutdown at any time since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnShutdown::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	ActivateOutput(pActInfo, eOut_Shutdown, true);
}

REGISTER_FLOW_NODE("GamePlatform:Listener:Service:OnEnvironmentVariableChanged", CFlowNode_OnEnvironmentVariableChanged);
REGISTER_FLOW_NODE("GamePlatform:Listener:Service:OnOverlayActivated", CFlowNode_OnOverlayActivated);
REGISTER_FLOW_NODE("GamePlatform:Listener:Service:OnPlatformServiceError", CFlowNode_OnPlatformServiceError);
REGISTER_FLOW_NODE("GamePlatform:Listener:Service:OnShutdown", CFlowNode_OnShutdown);

} // Cry::GamePlatform
