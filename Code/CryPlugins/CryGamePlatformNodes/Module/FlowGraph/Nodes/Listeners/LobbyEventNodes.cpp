// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "LobbyEventNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnChatMessage::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			OutputPortConfig<string>("Message", _HELP("The message that was received.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more messages were sent from other players in the lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnChatMessage::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdStr;
	string lobbyIdStr;

	event.GetAccountId().GetAsString(accountIdStr);
	event.GetLobbyId().GetAsString(lobbyIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
	ActivateOutput(pActInfo, eOut_Message, string(event.GetChatMessage().c_str()));
}

void CFlowNode_OnGameCreated::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("ServerId", _HELP("Server identifier of the game instance.")),
			OutputPortConfig<string>("IPAddress", _HELP("Public IP Address.")),
			OutputPortConfig<int>("Port", _HELP("Network port for clients to connect.")),
			OutputPortConfig<bool>("IsLocal", _HELP("IF the game instance is a local only instance.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more game instances were created since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnGameCreated::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_ServerId, ToString(event.GetServerId()));
	ActivateOutput(pActInfo, eOut_IPAddress, ToString(event.GetIPAddress()));
	ActivateOutput(pActInfo, eOut_Port, int(event.GetPort()));
	ActivateOutput(pActInfo, eOut_IsLocal, event.IsLocal());
}

void CFlowNode_OnPlayerLeft::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more players left the lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPlayerLeft::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	string accountIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);
	event.GetAccountId().GetAsString(accountIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
}

void CFlowNode_OnLeave::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if we left one or more lobbies since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLeave::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	event.GetLobbyId().GetAsString(lobbyIdStr);
	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
}

void CFlowNode_OnLobbyDataUpdate::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more lobby update events were received since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnLobbyDataUpdate::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	event.GetLobbyId().GetAsString(lobbyIdStr);
	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
}

void CFlowNode_OnPlayerBanned::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			OutputPortConfig<string>("ModeratorId", _HELP("The account identifier of the moderator.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more players were banned from the lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPlayerBanned::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	string accountIdStr;
	string moderatorIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);
	event.GetAccountId().GetAsString(accountIdStr);
	event.GetModeratorId().GetAsString(moderatorIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, moderatorIdStr);
}

void CFlowNode_OnPlayerDisconnected::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more players disconnected from the lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPlayerDisconnected::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	string accountIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);
	event.GetAccountId().GetAsString(accountIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
}

void CFlowNode_OnPlayerEntered::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more players entered the lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPlayerEntered::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	string accountIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);
	event.GetAccountId().GetAsString(accountIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
}

void CFlowNode_OnPlayerKicked::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			OutputPortConfig<string>("ModeratorId", _HELP("The account identifier of the moderator of the action.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more players were kicked from the lobby since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPlayerKicked::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	string accountIdStr;
	string moderatorIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);
	event.GetAccountId().GetAsString(accountIdStr);
	event.GetModeratorId().GetAsString(moderatorIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, moderatorIdStr);
}

void CFlowNode_OnUserDataUpdate::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("LobbyId", _HELP("The lobby identifier for the listener that the event was triggered from.")),
			OutputPortConfig<string>("AccountId", _HELP("The account identifier.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more user-specific lobby metadata change events were received since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnUserDataUpdate::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string lobbyIdStr;
	string accountIdStr;

	event.GetLobbyId().GetAsString(lobbyIdStr);
	event.GetAccountId().GetAsString(accountIdStr);

	ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
}

void CFlowNode_StartStopLobbyListener::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Start", _HELP("Start listening to the lobby events.")),
		InputPortConfig_Void("Stop", _HELP("Stop listening to the lobby events.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby to start listening to.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig_Void("OnStarted", _HELP("Triggered when the Start node was activated.")),
		OutputPortConfig_Void("OnStopped", _HELP("Triggered when the Stop node was activated.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Starts or stops lobby event listeners.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_StartStopLobbyListener::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		const bool isStart = IsPortActive(pActInfo, eIn_Start);
		const bool isStop = IsPortActive(pActInfo, eIn_Stop);

		if (isStart || isStop)
		{
			const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
			{
				if (IService* pService = Helper::GetService(platform))
				{
					if (IPlatformEventManager* pListener = pPlugin->GetPlatformEventManager())
					{
						LobbyIdentifier lobbyId;
						pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));
						if (IsPortActive(pActInfo, eIn_Start))
						{
							pListener->StartListening(lobbyId);
							ActivateOutput(pActInfo, eOut_OnStarted, true);
						}
						else if (IsPortActive(pActInfo, eIn_Stop))
						{
							pListener->StopListening(lobbyId);
							ActivateOutput(pActInfo, eOut_OnStopped, true);
						}
					}
				}
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnChatMessage", CFlowNode_OnChatMessage);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnGameCreated", CFlowNode_OnGameCreated);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnPlayerLeft", CFlowNode_OnPlayerLeft);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnLeave", CFlowNode_OnLeave);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnLobbyDataUpdate", CFlowNode_OnLobbyDataUpdate);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnPlayerBanned", CFlowNode_OnPlayerBanned);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnPlayerDisconnected", CFlowNode_OnPlayerDisconnected);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnPlayerEntered", CFlowNode_OnPlayerEntered);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnPlayerKicked", CFlowNode_OnPlayerKicked);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:OnUserDataUpdate", CFlowNode_OnUserDataUpdate);
REGISTER_FLOW_NODE("GamePlatform:Listener:Lobby:StartStopLobbyListener", CFlowNode_StartStopLobbyListener);

} // Cry::GamePlatform
