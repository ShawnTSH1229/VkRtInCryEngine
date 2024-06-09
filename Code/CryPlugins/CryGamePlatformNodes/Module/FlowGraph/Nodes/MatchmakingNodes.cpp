// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "MatchmakingNodes.h"
#include <IPlugin.h>
#include <IPlatformEventManager.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryGamePlatform/Interface/IPlatformMatchmaking.h>

#define FLOWGRAPH_PLATFORM_COMPARECONFIG "enum_int:EqualToOrLess=0,LessThan=1,Equal=2,GreaterThan=3,EqualToOrGreaterThan=4,NotEqual=5"

namespace Cry::GamePlatform
{

using EFilterComparison = IUserLobby::EComparison;
static inline EFilterComparison ComparisonUiToEnum(int uicValue)
{
	switch (uicValue)
	{
	case 0:
		return EFilterComparison::EqualToOrLess;
	case 1:
		return EFilterComparison::LessThan;
	case 2:
		return EFilterComparison::Equal;
	case 3:
		return EFilterComparison::GreaterThan;
	case 4:
		return EFilterComparison::EqualToOrGreaterThan;
	case 5:
		return EFilterComparison::NotEqual;
	}

	return EFilterComparison::EqualToOrLess;
}

void CFlowNode_AddLobbyQueryFilterString::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Add", _HELP("Attempts to create a lobby.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),                           "Platform",   _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Key", _HELP("The name of the filter.")),
		InputPortConfig<string>("Value", _HELP("The value of the filter.")),
		InputPortConfig<int>("Comparator", 0, _HELP("The type of comparison to apply to results."),  "Comparator", _UICONFIG(FLOWGRAPH_PLATFORM_COMPARECONFIG)),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.sDescription = "Adds a lobby query filter that will be used on the next QueryLobbies call.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_AddLobbyQueryFilterString::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Add))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				const string key = GetPortString(pActInfo, eIn_Key);
				const string value = GetPortString(pActInfo, eIn_Value);
				const EFilterComparison comparator = ComparisonUiToEnum(GetPortInt(pActInfo, eIn_Comparator));

				pMatchmaking->AddLobbyQueryFilterString(key, value, comparator);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_CreateLobby::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Create", _HELP("Attempts to create a lobby.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),                                                                     "Platform",   _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("Visibility", 0, _HELP("Visibility of the lobby."),                                                               "Visibility", _UICONFIG("enum_int:Private=0,FriendsOnly=1,Public=2,Invisible=3")),
		InputPortConfig<int>("MaxMembers", 1, _HELP("Maximum number of members that can inhabit the new lobby (including the local user).")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the lobby was created successfully.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Attempts to create a new matchmaking lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CreateLobby::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Create))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				auto visibility = IUserLobby::EVisibility::Private;
				switch (GetPortInt(pActInfo, eIn_Visibility))
				{
				case 1:
					visibility = IUserLobby::EVisibility::FriendsOnly;
					break;
				case 2:
					visibility = IUserLobby::EVisibility::Public;
					break;
				case 3:
					visibility = IUserLobby::EVisibility::Invisible;
					break;
				}

				const int maxMembers = crymath::clamp(GetPortInt(pActInfo, eIn_MaxMembers), 1, std::numeric_limits<int>::max());

				const bool success = pMatchmaking->CreateLobby(visibility, maxMembers);
				ActivateOutput(pActInfo, eOut_Success, success);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_GetQueriedLobbyIdByIndex::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Attempts to get the lobby by index.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("Index", _HELP("Index of the lobby you wish to get the lobby id for.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<string>("LobbyId", _HELP("The lobby id of the lobby at the specified index.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Returns the lobby at the specified index from the QueryLobbies result (after the OnLobbyQueryComplete event).";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetQueriedLobbyIdByIndex::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				const int index = crymath::clamp(GetPortInt(pActInfo, eIn_Index), 0, std::numeric_limits<int>::max());
				LobbyIdentifier lobbyId = pMatchmaking->GetQueriedLobbyIdByIndex(index);

				string lobbyIdStr;
				if (lobbyId.GetAsString(lobbyIdStr))
				{
					ActivateOutput(pActInfo, eOut_LobbyId, lobbyIdStr);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get the string representation of the lobby identifier.");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_GetUserLobby::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Get", _HELP("Get the lobby identifier.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("Account identifier to get the lobby for.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig<bool>("Success", _HELP("True if the lobby was created successfully.")),
		OutputPortConfig<string>("LobbyId", _HELP("Identifier of the lobby.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Attempts to get the lobby identifier the specified user resides in (if any).";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetUserLobby::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				AccountIdentifier accountId;
				pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

				IUserLobby* pLobby = pMatchmaking->GetUserLobby(accountId);
				if (pLobby != nullptr)
				{
					string lobbyId;
					if (pLobby->GetIdentifier().GetAsString(lobbyId))
					{
						ActivateOutput(pActInfo, eOut_LobbyId, lobbyId);
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get string representation of the lobby identifier.");
					}
				}

				ActivateOutput(pActInfo, eOut_Success, pLobby != nullptr);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_JoinLobby::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Join", _HELP("Joins the local user to the specified lobby.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("LobbyId", _HELP("Lobby identifier of the lobby to join.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.sDescription = "Joins the local user to the specified lobby.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_JoinLobby::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Join))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				LobbyIdentifier lobbyId;
				pService->SetIdentifierFromString(lobbyId, GetPortString(pActInfo, eIn_LobbyId));
				pMatchmaking->JoinLobby(lobbyId);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

void CFlowNode_QueryLobbies::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Query", _HELP("Query for lobbies.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform type."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.sDescription = "Queries the backend for lobbies that match any and all filters added since the last QueryLobbies call. Listen for OnLobbyQueryComplete events.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_QueryLobbies::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Query))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			if (IMatchmaking* pMatchmaking = pService->GetMatchmaking())
			{
				pMatchmaking->QueryLobbies();
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get matchmaking interface.");
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Matchmaking:AddLobbyQueryFilterString", CFlowNode_AddLobbyQueryFilterString);
REGISTER_FLOW_NODE("GamePlatform:Matchmaking:CreateLobby", CFlowNode_CreateLobby);
REGISTER_FLOW_NODE("GamePlatform:Matchmaking:GetQueriedLobbyIdByIndex", CFlowNode_GetQueriedLobbyIdByIndex);
REGISTER_FLOW_NODE("GamePlatform:Matchmaking:GetUserLobby", CFlowNode_GetUserLobby);
REGISTER_FLOW_NODE("GamePlatform:Matchmaking:JoinLobby", CFlowNode_JoinLobby);
REGISTER_FLOW_NODE("GamePlatform:Matchmaking:QueryLobbies", CFlowNode_QueryLobbies);

} // Cry::GamePlatform
