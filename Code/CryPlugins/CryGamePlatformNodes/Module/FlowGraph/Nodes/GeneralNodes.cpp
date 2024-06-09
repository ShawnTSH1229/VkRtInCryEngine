// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "GeneralNodes.h"

#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>

namespace Cry::GamePlatform
{

void CFlowNode_GetApplicationId::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the Application Id for this game on this platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<string>("Application Id", _HELP("The Application Id of the game on the specified platform.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the Application Id for this game on this platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetApplicationId::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			string applicationId;
			pService->GetApplicationIdentifier().GetAsString(applicationId);

			ActivateOutput(pActInfo, eOut_ApplicationId, applicationId);
		}
	}
}

void CFlowNode_GetBuildId::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the Build Id for this game on this platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<int>("Build Id", _HELP("The Build Id of the game on the specified platform.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the Build Id for this game on this platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetBuildId::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			ActivateOutput(pActInfo, eOut_BuildId, pService->GetBuildIdentifier());
		}
	}
}

void CFlowNode_GetEnvironmentValue::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the Environment Variable.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("The name of the environment variable to get.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<string>("Value", _HELP("The value of the environment variable.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the specified platform specific environment variable.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetEnvironmentValue::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			string value;
			if (pService->GetEnvironmentValue(GetPortString(pActInfo, eIn_Name), value))
			{
				ActivateOutput(pActInfo, eOut_Value, value);
			}
			else
			{
				const string name = GetPortString(pActInfo, eIn_Name);
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Environment variable '%s' not found.", name.c_str());
			}
		}
	}
}

void CFlowNode_GetService::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Get platform details.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("IsAvailable", _HELP("True if the platform has been registered and loaded, false otherwise."), "Is Available"),
		OutputPortConfig<string>("PlatformName", _HELP("The human readable name."), "Platform Name"),
		OutputPortConfig<string>("PlatformUID", _HELP("The platform unique identifier."), "Platform UID"),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets details of the registered game platform service.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetService::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		bool isAvailable = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IService* pService = Helper::GetService(platform))
		{
			isAvailable = true;

			const ServiceIdentifier identifier = pService->GetServiceIdentifier();
			const string name = Helper::GetServiceName(platform);

			ActivateOutput(pActInfo, eOut_UID, identifier.ToString());
			ActivateOutput(pActInfo, eOut_Name, name);
		}

		ActivateOutput(pActInfo, eOut_IsAvailable, isAvailable);
	}
}

void CFlowNode_MarkContentCorrupt::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Set", _HELP("Mark content corrupt.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to mark against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<bool>("OnlyMissingFiles", _HELP("Whether to tell the paltform to only check for missing files.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Tells the platform to verify the data files on next launch.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_MarkContentCorrupt::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Set))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			pService->MarkContentCorrupt(GetPortBool(pActInfo, eIn_OnlyMissingFiles));
		}
	}
}

void CFlowNode_OwnsApplication::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Checks if the local user owns the specified application on the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("ApplicationId", _HELP("The platform-specific application id of the DLC or Game.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Owns Application", _HELP("True if the user owns the specified application of the specified platofrm, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if the local user owns the specified application on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OwnsApplication::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			ApplicationIdentifier applicationId;
			pService->SetIdentifierFromString(applicationId, GetPortString(pActInfo, eIn_ApplicationId));

			ActivateOutput(pActInfo, eOut_OwnsApplication, pService->OwnsApplication(applicationId));
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:General:ApplicationId", CFlowNode_GetApplicationId);
REGISTER_FLOW_NODE("GamePlatform:General:BuildId", CFlowNode_GetBuildId);
REGISTER_FLOW_NODE("GamePlatform:General:GetEnvironmentVariable", CFlowNode_GetEnvironmentValue);
REGISTER_FLOW_NODE("GamePlatform:General:GetService", CFlowNode_GetService);
REGISTER_FLOW_NODE("GamePlatform:General:MarkContentCorrupt", CFlowNode_MarkContentCorrupt);
REGISTER_FLOW_NODE("GamePlatform:General:OwnsApplication", CFlowNode_OwnsApplication);

} // Cry::GamePlatform
