// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "LobbyNodes.h"
#include <IPlugin.h>
#include <IPlatformEventManager.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryGamePlatform/Interface/IPlatformMatchmaking.h>

namespace Cry::GamePlatform
{

void CFlowNode_GetData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<string>("Key", _HELP("The key name of the lobby variable.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the data was successfully set in the platform lobby.")),
		OutputPortConfig<string>("Value", _HELP("The value of the variable.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Gets lobby data of the key specified.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const char* szValue = pLobby->GetData(GetPortString(pActInfo, eIn_Key));
					ActivateOutput(pActInfo, eOut_Success, szValue != nullptr);

					if (szValue != nullptr)
					{
						const string value(szValue);
						ActivateOutput(pActInfo, eOut_Value, value);
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_GetMemberAtIndex::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the current number of members.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<int>("Index", _HELP("The index of the user to retrieve.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<string>("AccountId", _HELP("The account identifier at the specified index in the lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Queries the lobby for the member at the specified index.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetMemberAtIndex::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const int index = GetPortInt(pActInfo, eIn_Index);
					if (index >= 0 && index < pLobby->GetNumMembers())
					{
						const AccountIdentifier accountId = pLobby->GetMemberAtIndex(GetPortInt(pActInfo, eIn_Index));

						string accountIdStr;
						accountId.GetAsString(accountIdStr);
						ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Member index is outside the bounds of the lobby's current member list.");
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_GetMemberData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<string>("AccountId", _HELP("The account identifier.")),
		InputPortConfig<string>("Key", _HELP("The key name of the user's meta-data.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the data was successfully retrieved from the platform lobby.")),
		OutputPortConfig<string>("Value", _HELP("The value of the user's meta-data.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Gets the meta-data of the user in the lobby by key name.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetMemberData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		bool success = false;
		string value = "";

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					AccountIdentifier accountId;
					pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

					if (const char* szValue = pLobby->GetMemberData(accountId, GetPortString(pActInfo, eIn_Key)))
					{
						success = true;
						value.assign(szValue);
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}

		ActivateOutput(pActInfo, eOut_Success, success);
		ActivateOutput(pActInfo, eOut_Value, value);
	}
}

void CFlowNode_GetMemberLimit::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the member limit.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<int>("Limit", _HELP("The maximum number of members that can reside in the lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Queries the lobby for the maximum number of users that can inhabit the specified lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetMemberLimit::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const int memberLimit = pLobby->GetMemberLimit();
					ActivateOutput(pActInfo, eOut_MemberLimit, memberLimit);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_GetNumMembers::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the current number of members.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<int>("MemberCount", _HELP("The current number of members in the lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Queries the lobby for the current number of users that are inhabiting the specified lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetNumMembers::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const int memberCount = pLobby->GetNumMembers();
					ActivateOutput(pActInfo, eOut_MemberCount, memberCount);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_GetOwnerId::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the current owner.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<string>("AccountId", _HELP("The account identifier of the owner of the lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Queries the lobby for the member that owns the lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetOwnerId::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const AccountIdentifier accountId = pLobby->GetOwnerId();

					string accountIdStr;
					accountId.GetAsString(accountIdStr);
					ActivateOutput(pActInfo, eOut_AccountId, accountIdStr);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_HostServer::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Create", _HELP("Attempts to create a server.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby to create the server for.")),
		InputPortConfig<string>("Level", _HELP("The level that will be loaded after creating the server.")),
		InputPortConfig<bool>("IsLocal", _HELP("Whether the server is local or public (uses local ip address or public).")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the server was created successfully.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Attempts to create a server via the platform interface and loads the specified level.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_HostServer::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Create))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const bool success = pLobby->HostServer(GetPortString(pActInfo, eIn_Level), GetPortBool(pActInfo, eIn_IsLocal));
					ActivateOutput(pActInfo, eOut_Success, success);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_InvitePlayers::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Invite", _HELP("Show the dialog.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<int>("SystemContainerId", _HELP("The system container id holding the list of account id's to invite.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("Whether the users were invited succesfully or not.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Attempts to invite the given list of accounts into the specified lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_InvitePlayers::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Invite))
	{
		IUserLobby* pUserLobby = nullptr;
		IFlowSystemContainerPtr pContainer = nullptr;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				pUserLobby = pMatchmaking->GetLobbyById(lobbyId);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}

		pContainer = gEnv->pFlowSystem->GetContainer(GetPortInt(pActInfo, eIn_SystemContainerId));

		if (pUserLobby != nullptr)
		{
			if (pContainer)
			{
				DynArray<AccountIdentifier> invitees;
				invitees.reserve(pContainer->GetItemCount());

				if (IService* pService = Helper::GetService(platform))
				{
					for (int i = 0; i < pContainer->GetItemCount(); i++)
					{
						auto item = pContainer->GetItem(i);
						string accountStr;
						if (item.GetValueWithConversion(accountStr))
						{
							AccountIdentifier accountId;
							pService->SetIdentifierFromString(accountId, accountStr);
							invitees.emplace_back(accountId);
						}
						else
						{
							CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "System container has unexpected element types.");
						}
					}
				}

				if (invitees.size() > 0)
				{
					const bool success = pUserLobby->InvitePlayers(invitees);
					ActivateOutput(pActInfo, eOut_Success, success);
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "System container with specified id does not exist.");
			}
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
		}
	}
}

void CFlowNode_IsInServer::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Check", _HELP("Check if the lobby is in a server.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("IsInServer", _HELP("True if the lobby is in a server.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Queries the lobby to check if it is currently in a server.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_IsInServer::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const bool isInServer = pLobby->IsInServer();
					ActivateOutput(pActInfo, eOut_IsInServer, isInServer);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_Leave::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Leave", _HELP("Leave the lobby.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.sDescription = "Leaves the specified lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_Leave::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Leave))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					pLobby->Leave();
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_SendChatMessage::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Send", _HELP("Send chat message.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<string>("Messsage", _HELP("The message to broadcast to the lobby.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the message was successfully sent to the platform lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Sends a message to the lobby specified.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SendChatMessage::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Send))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					string message = GetPortString(pActInfo, eIn_Message);
					if (message.size() > 0)
					{
						const bool success = pLobby->SendChatMessage(message);
						ActivateOutput(pActInfo, eOut_Success, success);
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Message was empty.");
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_SetData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Set", _HELP("Set the data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<string>("Key", _HELP("The key name of the lobby variable.")),
		InputPortConfig<string>("Value", _HELP("The value to set.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the data was successfully set in the platform lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Sets lobby data of the key to the specified value.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SetData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Set))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					const bool success = pLobby->SetData(GetPortString(pActInfo, eIn_Key), GetPortString(pActInfo, eIn_Value));
					ActivateOutput(pActInfo, eOut_Success, success);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_SetLocalMemberData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Set", _HELP("Set the data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		InputPortConfig<string>("Key", _HELP("The key name of the user's meta-data.")),
		InputPortConfig<string>("Value", _HELP("The value of the user's meta-data.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.sDescription = "Sets the meta-data of the local user in the lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SetLocalMemberData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Set))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					pLobby->SetMemberData(GetPortString(pActInfo, eIn_Key), GetPortString(pActInfo, eIn_Value));
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_ShowInviteDialog::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("ShowDialog", _HELP("Show the dialog.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("The lobby identifier.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.sDescription = "Shows the platform-specific invite dialog to invite friends to the current lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ShowInviteDialog::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_ShowDialog))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));

				if (IUserLobby* pLobby = pMatchmaking->GetLobbyById(lobbyId))
				{
					pLobby->ShowInviteDialog();
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find lobby.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Lobby:GetData", CFlowNode_GetData);
REGISTER_FLOW_NODE("GamePlatform:Lobby:GetMemberAtIndex", CFlowNode_GetMemberAtIndex);
REGISTER_FLOW_NODE("GamePlatform:Lobby:GetMemberData", CFlowNode_GetMemberData);
REGISTER_FLOW_NODE("GamePlatform:Lobby:GetMemberLimit", CFlowNode_GetMemberLimit);
REGISTER_FLOW_NODE("GamePlatform:Lobby:GetMemberCount", CFlowNode_GetNumMembers);
REGISTER_FLOW_NODE("GamePlatform:Lobby:GetOwnerId", CFlowNode_GetOwnerId);
REGISTER_FLOW_NODE("GamePlatform:Lobby:HostServer", CFlowNode_HostServer);
REGISTER_FLOW_NODE("GamePlatform:Lobby:InvitePlayers", CFlowNode_InvitePlayers);
REGISTER_FLOW_NODE("GamePlatform:Lobby:IsInServer", CFlowNode_IsInServer);
REGISTER_FLOW_NODE("GamePlatform:Lobby:Leave", CFlowNode_Leave);
REGISTER_FLOW_NODE("GamePlatform:Lobby:SendChatMessage", CFlowNode_SendChatMessage);
REGISTER_FLOW_NODE("GamePlatform:Lobby:SetData", CFlowNode_SetData);
REGISTER_FLOW_NODE("GamePlatform:Lobby:SetLocalMemberData", CFlowNode_SetLocalMemberData);
REGISTER_FLOW_NODE("GamePlatform:Lobby:ShowInviteDialog", CFlowNode_ShowInviteDialog);

} // Cry::GamePlatform
