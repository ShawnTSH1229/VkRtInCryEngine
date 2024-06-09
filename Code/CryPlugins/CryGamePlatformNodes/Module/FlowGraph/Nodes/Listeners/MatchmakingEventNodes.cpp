// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "MatchmakingEventNodes.h"
#include "../ContainerBaseNode.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnCreatedLobby::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The identifier of the lobby that was created.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more lobby created events were received since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnCreatedLobby::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyId;
	event.GetLobby().GetAsString(lobbyId);
	ActivateOutput(pActInfo, eOut_LobbyId, lobbyId);
}

void CFlowNode_OnJoinedLobby::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The identifier of the lobby that was joined.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more lobby joined events were received since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnJoinedLobby::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyId;
	event.GetLobbyId().GetAsString(lobbyId);
	ActivateOutput(pActInfo, eOut_LobbyId, lobbyId);
}

void CFlowNode_OnLobbyJoinFailed::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The identifier of the lobby that we failed to join.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if we failed to join one or more lobbies since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLobbyJoinFailed::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyId;
	event.GetLobbyId().GetAsString(lobbyId);
	ActivateOutput(pActInfo, eOut_LobbyId, lobbyId);
}

void CFlowNode_OnLobbyQueryComplete::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<int>("NumLobbiesFound", _HELP("Number of lobbies found.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more lobby queries have returned since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLobbyQueryComplete::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	ActivateOutput(pActInfo, eOut_NumLobbiesFound, event.GetNumLobbies());
}

void CFlowNode_OnPreJoinLobby::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The identifier of the lobby that we are about to join.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired for each time we attempted to join a lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPreJoinLobby::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyId;
	event.GetLobbyId().GetAsString(lobbyId);
	ActivateOutput(pActInfo, eOut_LobbyId, lobbyId);
}

REGISTER_FLOW_NODE("GamePlatform:Listener:Matchmaking:OnCreatedLobby", CFlowNode_OnCreatedLobby);
REGISTER_FLOW_NODE("GamePlatform:Listener:Matchmaking:OnJoinedLobby", CFlowNode_OnJoinedLobby);
REGISTER_FLOW_NODE("GamePlatform:Listener:Matchmaking:OnLobbyJoinFailed", CFlowNode_OnLobbyJoinFailed);
REGISTER_FLOW_NODE("GamePlatform:Listener:Matchmaking:OnLobbyQueryComplete", CFlowNode_OnLobbyQueryComplete);
REGISTER_FLOW_NODE("GamePlatform:Listener:Matchmaking:OnPreJoinLobby", CFlowNode_OnPreJoinLobby);

} // Cry::GamePlatform
