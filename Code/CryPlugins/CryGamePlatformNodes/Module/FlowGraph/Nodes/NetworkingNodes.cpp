// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "NetworkingNodes.h"
#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryGamePlatform/Interface/IPlatformAccount.h>
#include <CryGamePlatform/Interface/IPlatformServer.h>
#include <CryGamePlatform/Interface/IPlatformNetworking.h>

namespace Cry::GamePlatform
{

void CFlowNode_AddAccountToLocalSession::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Add", _HELP("Adds the account into a lobby or match context for interaction relevant updates (HasPrivacyPermission).")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to add the account on."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier for the account to add.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if there was no error, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Adds the account into a lobby or match context for interaction relevant updates (HasPrivacyPermission).");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_AddAccountToLocalSession::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Add))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

			pService->AddAccountToLocalSession(accountId);

			ActivateOutput(pActInfo, eOut_Success, true);
		}
		else
		{
			ActivateOutput(pActInfo, eOut_Success, false);
		}
	}
}

void CFlowNode_AuthenticateUser::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Authenticate", _HELP("Attempts to authenticate a user on the server.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform of the server."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("ClientIP", _HELP("Publicly accessibly IP address of the user.")),
		InputPortConfig<string>("AuthToken", _HELP("The authentication token for the user.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the authentication was successful, otherwise false.")),
		OutputPortConfig<string>("AccountId", _HELP("The client's platform-specific identifier.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Authenticates a specific user on the server using an auth token generated on the client using GetAuthToken.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_AuthenticateUser::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Authenticate))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			bool success = false;

			if (IServer* pServer = pService->GetLocalServer())
			{
				uint32 ip;
				if (ParseIPString(GetPortString(pActInfo, eIn_ClientIP), ip))
				{
					const string authToken = GetPortString(pActInfo, eIn_ClientAuthToken);

					char* token = new char[authToken.length() + 1];
					strncpy(token, authToken.c_str(), authToken.length());

					AccountIdentifier accountId;
					success = pServer->AuthenticateUser(ip, token, authToken.length(), accountId);

					string accountIdStr;
					if (accountId.GetAsString(accountIdStr))
					{
						ActivateOutput(pActInfo, eOut_ClientAccountId, accountIdStr);
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get string representation of the account id.");
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Client IP address could not be parsed (incorrect format?).");
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get the server.");
			}

			ActivateOutput(pActInfo, eOut_Success, success);
		}
	}
}

bool CFlowNode_AuthenticateUser::ParseIPString(const string& ipIN, uint32& ipOUT) const
{
	ipOUT = 0;
	int tC = 0;
	int index = 0;

	for (; index < 4; index++)
	{
		stack_string part = ipIN.Tokenize(".", tC);
		int num = atoi(part.c_str());

		if (num > 255 || (num == 0 && part != "0"))
			return false;

		ipOUT += num << ((3 - index) * 8);
	}

	return index == 4 && tC == ipIN.size() + 1;
}

void CFlowNode_CanAccessMultiplayer::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Sends an Asynchronous request to check whether this platform can access the Multiplayer Services at this time.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Response", _HELP("True if we can acces multiplayer services for this platform. Flase otherwise. NOTE: May not trigger on the same frame as the check.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Sends an Asynchronous request to check whether this platform can access the Multiplayer Services at this time.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CanAccessMultiplayer::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			pService->CanAccessMultiplayerServices([pActInfo](bool authorized)
			{
				ActivateOutput(pActInfo, eOut_Response, authorized);
			});
		}
	}
}

void CFlowNode_CloseSession::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Close", _HELP("Close the sesdsion.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier with which we are closing the session for.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Closes a network session opened with SendPacket between the local user and the specified account.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CloseSession::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Close))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (INetworking* pNetworking = pService->GetNetworking())
			{
				AccountIdentifier accountId;
				pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

				pNetworking->CloseSession(accountId);
			}
		}
	}
}

void CFlowNode_CreateServer::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Create", _HELP("Create the server.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<bool>("LocalOnly", _HELP("True to create a local only server.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the server was created, false otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Starts a server on the platform, registering it with the network - thus allowing future discovery through the matchmaking systems.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CreateServer::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Create))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			IServer* pServer = pService->CreateServer(GetPortBool(pActInfo, eIn_LocalOnly));
			ActivateOutput(pActInfo, eOut_Success, pServer != nullptr);
		}
	}
}

void CFlowNode_FilterText::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Filter", _HELP("Filter the given text.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to filter on."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Text", _HELP("The text to filter on the given platform.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("Whether the text could be filtered.")),
		OutputPortConfig<string>("FilteredText", _HELP("Filter the given text.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Filters the given text on the platform. (Output may not trigger on the same frame). Output limited to once per frame.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_FilterText::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Initialize)
	{
		pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, true);
	}
	else if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Filter))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			const bool success = pService->FilterText(GetPortString(pActInfo, eIn_Text), [this](const char* szFiltered)
				{
					m_outputs.emplace(szFiltered);
				});

			ActivateOutput(pActInfo, eOut_Success, success);
		}
	}
	else if (event == eFE_Update)
	{
		if (m_outputs.size() > 0)
		{
			const string filteredText = m_outputs.front();
			m_outputs.pop();

			ActivateOutput(pActInfo, eOut_FilteredText, filteredText);
		}
	}
}

void CFlowNode_GetAuthToken::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the authorization token used for web services on the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if there was no error, False otherwise.")),
		OutputPortConfig<string>("Token", _HELP("The Authorization Token provided on the specified platform.")),
		OutputPortConfig<int>("Issuer Id", _HELP("The Issuer Id that issued the token on the specified platform.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the authorization token used for web services on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetAuthToken::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			string authToken;
			int issuerId = 0;
			if (pService->GetAuthToken(authToken, issuerId))
			{
				ActivateOutput(pActInfo, eOut_Success, true);
				ActivateOutput(pActInfo, eOut_AuthToken, authToken);
				ActivateOutput(pActInfo, eOut_IssuerId, issuerId);
			}
			else
			{
				ActivateOutput(pActInfo, eOut_Success, false);
			}
		}
	}
}

void CFlowNode_GetConnectionStatus::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the connection status of the service for the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig_Void("Disconnected", _HELP("Triggers if the connection status is disconnected.")),
		OutputPortConfig_Void("Connecting", _HELP("Triggers if the connection status is connecting.")),
		OutputPortConfig_Void("ObtainingIP", _HELP("Triggers if the connection status is obtaining IP address.")),
		OutputPortConfig_Void("Connected", _HELP("Triggers if the connection status is connected.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the connection status of the service for the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetConnectionStatus::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			switch (pService->GetConnectionStatus())
			{
			case EConnectionStatus::Disconnected:
				ActivateOutput(pActInfo, eOut_Disconnected, true);
				break;
			case EConnectionStatus::Connecting:
				ActivateOutput(pActInfo, eOut_Connecting, true);
				break;
			case EConnectionStatus::ObtainingIP:
				ActivateOutput(pActInfo, eOut_ObtainingIP, true);
				break;
			case EConnectionStatus::Connected:
				ActivateOutput(pActInfo, eOut_Connected, true);
				break;
			}
		}
	}
}

void CFlowNode_IsLoggedIn::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Checks if the platform is logged in and we have access to the backend.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the platform is connected to the backend, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if the platform is logged in and we have access to the backend.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_IsLoggedIn::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			const bool loggedIn = pService->IsLoggedIn();
			ActivateOutput(pActInfo, eOut_Success, loggedIn);
		}
	}
}

void CFlowNode_IsPacketAvailable::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Check for packets.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Available", _HELP("True if there is a packet available to read.")),
		OutputPortConfig<int>("DataSize", _HELP("The size of the packet.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if there is a packet available via the platform network and if so, will return the size of the data.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_IsPacketAvailable::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (INetworking* pNetworking = pService->GetNetworking())
			{
				uint32 dataSize = 0;
				const bool isAvailable = pNetworking->IsPacketAvailable(&dataSize);

				ActivateOutput(pActInfo, eOut_Available, isAvailable);

				if (isAvailable)
				{
					ActivateOutput(pActInfo, eOut_DataSize, dataSize);
				}
			}
		}
	}
}

void CFlowNode_ReadPacket::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Read", _HELP("Read the data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("MaxReadLength", _HELP("Maximum number of bytes that can be read from the packet (You can get an exact count via IsPacketAvailable).")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if there is a packet was read successfully.")),
		OutputPortConfig<string>("SourceAccountId", _HELP("The identifier of the account the packet originated from.")),
		OutputPortConfig<int>("BytesRead", _HELP("Number of bytes that were read from the packet, eg. if MaxReadLength is larger than the packet size.")),
		OutputPortConfig<string>("Data", _HELP("The packet data as a string (in case of binary packet data, it may appear truncated).")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Tries to read the next packet payload into a string and provides the source account that sent it.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReadPacket::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Read))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const int maxReadLen = GetPortInt(pActInfo, eIn_MaxReadLength);

		CRY_ASSERT(maxReadLen > 0, "Max read length should be greater than 0.");

		if (IService* pService = Helper::GetService(platform))
		{
			if (INetworking* pNetworking = pService->GetNetworking())
			{
				const int maxSize = crymath::clamp(maxReadLen + 1, 1, std::numeric_limits<int>::max());
				std::vector<char> dest;
				dest.resize(static_cast<uint32>(maxSize));

				uint32 readBytes = 0;
				AccountIdentifier sourceAccountId;
				const bool success = pNetworking->ReadPacket(&(*dest.begin()), maxSize, &readBytes, &sourceAccountId);

				CRY_ASSERT(maxSize > readBytes, "Max size was not large enough to read the packet data. Some data was probably lost.");

				// Make sure there is a null terminator at the end of the data.
				dest[maxSize < readBytes ? maxSize - 1 : readBytes] = '\0';

				if (success)
				{
					string srcAccStr = "";
					sourceAccountId.GetAsString(srcAccStr);

					ActivateOutput(pActInfo, eOut_SourceAccountId, srcAccStr);
					ActivateOutput(pActInfo, eOut_BytesRead, static_cast<int>(readBytes));
					ActivateOutput(pActInfo, eOut_Data, string(dest.data()));
				}

				ActivateOutput(pActInfo, eOut_Success, success);
			}
		}
	}
}

void CFlowNode_RemoveAccountFromLocalSession::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Add", _HELP("Removes the account from a lobby or match context.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to remove the account on."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier for the account to remove.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if there was no error, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Removes the account from a lobby or match context.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RemoveAccountFromLocalSession::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Remove))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

			pService->RemoveAccountFromLocalSession(accountId);

			ActivateOutput(pActInfo, eOut_Success, true);
		}
		else
		{
			ActivateOutput(pActInfo, eOut_Success, false);
		}
	}
}

void CFlowNode_SendPacket::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Send", _HELP("Send the data.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier to send to.")),
		InputPortConfig<string>("Data", _HELP("The data to send.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if there was no error, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Sends the string data to the specified account via the platform network.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SendPacket::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Send))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (INetworking* pNetworking = pService->GetNetworking())
			{
				AccountIdentifier accountId;
				pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

				const string data = GetPortString(pActInfo, eIn_Data);
				std::vector<char> dataV(data.begin(), data.end() + 1);

				ActivateOutput(pActInfo, eOut_Success, pNetworking->SendPacket(accountId, &(*dataV.begin()), dataV.size()));
			}
		}
	}
}

void CFlowNode_SendUserDisconnect::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("SendDisconnected", _HELP("Tell the platform this user disconnected from the server.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform of the server."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The identifier of the account that disconnected.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Should be called whenever a user leaves the game server, allowing the platform services to track what users are on which servers.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SendUserDisconnect::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_SendDisconnected))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (IServer* pServer = pService->GetLocalServer())
			{
				AccountIdentifier accountId;
				pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

				pServer->SendUserDisconnect(accountId);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get the server.");
			}
		}
	}
}

void CFlowNode_ServerInfo::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Get the info of the server.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform of the server."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<string>("PublicIP", _HELP("The public IP address of the server.")),
		OutputPortConfig<int>("Port", _HELP("The port of the server.")),
		OutputPortConfig<bool>("IsLocal", _HELP("Whether this is a local only server.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets details about the specified server.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ServerInfo::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (IServer* pServer = pService->GetLocalServer())
			{
				const string publicIP = pServer->GetPublicIPString();
				ActivateOutput(pActInfo, eOut_PublicIP, publicIP);
				ActivateOutput(pActInfo, eOut_Port, static_cast<int>(pServer->GetPort()));
				ActivateOutput(pActInfo, eOut_IsLocal, pServer->IsLocal());
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get the server.");
			}
		}
	}
}

void CFlowNode_SetIsInMultiplayer::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Set", _HELP("Change the user from being in a multiplayer environment or not.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<bool>("IsInMultiplayer", _HELP("True if the user is in a multiplayer environment, false otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Sets user as being in a multiplayer environment or not.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SetIsInMultiplayer::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Set))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			pService->SetIsInMultiplayer(GetPortBool(pActInfo, eIn_IsInMultiplayer));
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Service:AddToLocalSession", CFlowNode_AddAccountToLocalSession);
REGISTER_FLOW_NODE("GamePlatform:Network:AuthenticateUser", CFlowNode_AuthenticateUser);
REGISTER_FLOW_NODE("GamePlatform:Network:CanAccessMultiplayer", CFlowNode_CanAccessMultiplayer);
REGISTER_FLOW_NODE("GamePlatform:Network:CloseSession", CFlowNode_CloseSession);
REGISTER_FLOW_NODE("GamePlatform:Network:CreateServer", CFlowNode_CreateServer);
REGISTER_FLOW_NODE("GamePlatform:Network:FilterText", CFlowNode_FilterText);
REGISTER_FLOW_NODE("GamePlatform:Network:AuthToken", CFlowNode_GetAuthToken);
REGISTER_FLOW_NODE("GamePlatform:Network:ConnectionStatus", CFlowNode_GetConnectionStatus);
REGISTER_FLOW_NODE("GamePlatform:Network:IsLoggedIn", CFlowNode_IsLoggedIn);
REGISTER_FLOW_NODE("GamePlatform:Network:IsPacketAvailable", CFlowNode_IsPacketAvailable);
REGISTER_FLOW_NODE("GamePlatform:Network:ReadPacket", CFlowNode_ReadPacket);
REGISTER_FLOW_NODE("GamePlatform:Network:RemoveFromLocalSession", CFlowNode_RemoveAccountFromLocalSession);
REGISTER_FLOW_NODE("GamePlatform:Network:SendPacket", CFlowNode_SendPacket);
REGISTER_FLOW_NODE("GamePlatform:Network:SendUserDisconnect", CFlowNode_SendUserDisconnect);
REGISTER_FLOW_NODE("GamePlatform:Account:ServerInfo", CFlowNode_ServerInfo);
REGISTER_FLOW_NODE("GamePlatform:Network:SetIsInMultiplayer", CFlowNode_SetIsInMultiplayer);

} // Cry::GamePlatform
