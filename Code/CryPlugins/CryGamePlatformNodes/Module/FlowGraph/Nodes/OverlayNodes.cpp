// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "OverlayNodes.h"

#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>

namespace Cry::GamePlatform
{

void CFlowNode_CanOpenPurchaseOverlay::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Checks if the platform service can open the Purchase Overlay.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Response", _HELP("True if the service can open the Purchase Overlay for the specified platform.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if the platform service can open the Purchase Overlay.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CanOpenPurchaseOverlay::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			ActivateOutput(pActInfo, eOut_Response, pService->CanOpenPurchaseOverlay());
		}
	}
}

void CFlowNode_OpenBrowser::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("OpenBrowser", _HELP("Opens a platform browser on the specified platform with the specified target adress.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to open the dialog with."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Address", _HELP("The URL address the browser will be directed to.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the action was successful, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Opens a platform browser on the specified platform with the specified target adress.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OpenBrowser::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_OpenBrowser))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			ActivateOutput(pActInfo, eOut_Success, pService->OpenBrowser(GetPortString(pActInfo, eIn_Address)));
		}
	}
}

void CFlowNode_OpenDialog::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("OpenDialog", _HELP("Opens the specified dialog on the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to open the dialog with."),"Platform",                                                                                       _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("DialogType", 0, _HELP("Type of dialog to open."),  _UICONFIG("enum_int:Friends=0,Community=1,Players=2,Settings=3,Group=4,Achievements=5,Stats=6")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the action was successful, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Opens the specified dialog with the specified user on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OpenDialog::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_OpenDialog))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			EDialog dialogType;
			switch (GetPortInt(pActInfo, eIn_DialogType))
			{
			case 0:
				dialogType = EDialog::Friends;
				break;
			case 1:
				dialogType = EDialog::Community;
				break;
			case 2:
				dialogType = EDialog::Players;
				break;
			case 3:
				dialogType = EDialog::Settings;
				break;
			case 4:
				dialogType = EDialog::Group;
				break;
			case 5:
				dialogType = EDialog::Achievements;
				break;
			case 6:
				dialogType = EDialog::Stats;
				break;
			}

			ActivateOutput(pActInfo, eOut_Success, pService->OpenDialog(dialogType));
		}
	}
}

void CFlowNode_OpenDialogWithTargetUser::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("OpenDialog", _HELP("Opens the specified dialog with the specified user on the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the accounts belongs to."),"Platform",                                                                                                                                         _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("TargetAccountId", _HELP("The target account identifier for the account to open the dialog with.")),
		InputPortConfig<int>("DialogType", 0, _HELP("Type of dialog to open."),  _UICONFIG("enum_int:UserInfo=0,FriendAdd=1,FriendRemove=2,FriendRequestAccept=3,FriendRequestIgnore=4,Chat=5,JoinTrade=6,Stats=7,Achievements=8")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the action was successful, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Opens the specified dialog with the specified user on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OpenDialogWithTargetUser::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_OpenDialog))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_TargetAccountId));

			EUserTargetedDialog dialogType;
			switch (GetPortInt(pActInfo, eIn_DialogType))
			{
			case 0:
				dialogType = EUserTargetedDialog::UserInfo;
				break;
			case 1:
				dialogType = EUserTargetedDialog::FriendAdd;
				break;
			case 2:
				dialogType = EUserTargetedDialog::FriendRemove;
				break;
			case 3:
				dialogType = EUserTargetedDialog::FriendRequestAccept;
				break;
			case 4:
				dialogType = EUserTargetedDialog::FriendRequestIgnore;
				break;
			case 5:
				dialogType = EUserTargetedDialog::Chat;
				break;
			case 6:
				dialogType = EUserTargetedDialog::JoinTrade;
				break;
			case 7:
				dialogType = EUserTargetedDialog::Stats;
				break;
			case 8:
				dialogType = EUserTargetedDialog::Achievements;
				break;
			}

			ActivateOutput(pActInfo, eOut_Success, pService->OpenDialogWithTargetUser(dialogType, accountId));
		}
		else
		{
			ActivateOutput(pActInfo, eOut_Success, false);
		}
	}
}

void CFlowNode_OpenPurchaseOverlay::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Open", _HELP("Open the Purchase Overlay.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("ProductId", _HELP("The product identifier that we are opening the dialog for.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the Purchase Overlay was opened.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Tries to open the purchase overlay for the specified product.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OpenPurchaseOverlay::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Open))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			ApplicationIdentifier productId;
			pService->SetIdentifierFromString(productId, GetPortString(pActInfo, eIn_ProductId));
			ActivateOutput(pActInfo, eOut_Success, pService->OpenPurchaseOverlay(productId));
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Overlay:CanOpenPurchase", CFlowNode_CanOpenPurchaseOverlay);
REGISTER_FLOW_NODE("GamePlatform:Overlay:OpenBrowser", CFlowNode_OpenBrowser);
REGISTER_FLOW_NODE("GamePlatform:Overlay:OpenDialog", CFlowNode_OpenDialog);
REGISTER_FLOW_NODE("GamePlatform:Overlay:OpenDialogWithTarget", CFlowNode_OpenDialogWithTargetUser);
REGISTER_FLOW_NODE("GamePlatform:Overlay:OpenPurchaseOverlay", CFlowNode_OpenPurchaseOverlay);

} // Cry::GamePlatform
