// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "AccountNodes.h"

#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

void CFlowNode_CheckIsFriend::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Checks if the supplied account is on the local account's friends list.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check on."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier for the account to query.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the check itself was successful, False otherwise.")),
		OutputPortConfig<bool>("IsFriends", _HELP("True if the account is a friend, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if the supplied account is on the local account's friends list.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CheckIsFriend::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));
			const bool isFriend = pService->IsFriendWith(accountId);

			ActivateOutput(pActInfo, eOut_Success, true);
			ActivateOutput(pActInfo, eOut_IsFriend, isFriend);
		}
		else
		{
			ActivateOutput(pActInfo, eOut_Success, false);
		}
	}
}

void CFlowNode_GetAccountInfo::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Acquires account details pertaining to the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier for the account to query.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the account was found, False otherwise.")),
		OutputPortConfig<string>("Nickname", _HELP("The nickname associated with this account.")),
		OutputPortConfig<bool>("IsLocal", _HELP("Whether this is the account logged in on this machine.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets details about the specified account on the platform selected.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetAccountInfo::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

			if (IAccount* pAccount = pService->GetAccountById(accountId))
			{
				ActivateOutput(pActInfo, eOut_Success, true);
				ActivateOutput(pActInfo, eOut_Nickname, string(pAccount->GetNickname()));
				ActivateOutput(pActInfo, eOut_IsLocal, pAccount->IsLocal());
			}
			else
			{
				const string accountString = GetPortString(pActInfo, eIn_AccountId);
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find the account '%s'.", accountString.c_str());
				ActivateOutput(pActInfo, eOut_Success, false);
			}
		}
	}
}

void CFlowNode_GetAvatar::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Acquires account details pertaining to the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),                   "Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier for the account to query.")),
		InputPortConfig<int>("Size",         2, _HELP("The size of the avatar to retrieve. Default is Large."),  "Size",     _UICONFIG("enum_int:Small=0,Medium=1,Large=2")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the avatar was found, False otherwise.")),
		OutputPortConfig<int>("Avatar TexId", _HELP("The texture id of the avatar image.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the avatar texture id of size for the account on the platform selected.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetAvatar::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));
			if (IAccount* pAccount = pService->GetAccountById(accountId))
			{
				const EAvatarSize avatarSize = UIConfigToSize(GetPortInt(pActInfo, eIn_Size));
				_smart_ptr<ITexture> pTexture = pAccount->GetAvatar(avatarSize);

				if (pTexture)
				{
					ActivateOutput(pActInfo, eOut_Success, true);
					ActivateOutput(pActInfo, eOut_Avatar, pTexture->GetTextureID());
				}
				else
				{
					const string accountString = GetPortString(pActInfo, eIn_AccountId);
					const string sizeString = AvatarSizeToString(avatarSize);
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not retrieve '%s' avatar texture for account '%s'.", sizeString.c_str(), accountString.c_str());
					ActivateOutput(pActInfo, eOut_Success, false);
				}
			}
			else
			{
				const string accountString = GetPortString(pActInfo, eIn_AccountId);
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Account '%s' does not exist.", accountString.c_str());
				ActivateOutput(pActInfo, eOut_Success, false);
			}
		}
		else
		{
			ActivateOutput(pActInfo, eOut_Success, false);
		}
	}
}

EAvatarSize CFlowNode_GetAvatar::UIConfigToSize(int uicValue) const
{
	switch (uicValue)
	{
	case 0:
		return EAvatarSize::Small;
	case 1:
		return EAvatarSize::Medium;
	default:
		return EAvatarSize::Large;
	}
}

const char* CFlowNode_GetAvatar::AvatarSizeToString(EAvatarSize eSize) const
{
	switch (eSize)
	{
	case EAvatarSize::Small:
		return "Small";
	case EAvatarSize::Medium:
		return "Medium";
	case EAvatarSize::Large:
		return "Large";
	default:
		return "Error";
	}
}

void CFlowNode_GetBlockedAccounts::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Retrieve", _HELP("Query the platform for the list of blocked accounts.")),
		InputPortConfig_Void("GetNext", _HELP("Get the next account that was retrieved (if available).")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<int>("Remaining", _HELP("Remaining blocked accounts you can get from the list using GetNext.")),
		OutputPortConfig<string>("AccountId", _HELP("The account identifier of the blocked user.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Allows iteration of the list of blocked accounts relative to the local account.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetBlockedAccounts::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{

	if (event == eFE_Activate)
	{
		if (IsPortActive(pActInfo, eIn_Retrieve))
		{
			// Empty the friends queue
			m_blockedUsers = {}; 

			EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (IService* pService = Helper::GetService(platform))
			{
				const auto blockedUsers = pService->GetBlockedAccounts();
				for (const auto pAccount : blockedUsers)
				{
					m_blockedUsers.emplace(pAccount->GetIdentifier());
				}
			}
		}
		else if (IsPortActive(pActInfo, eIn_GetNext))
		{
			if (m_blockedUsers.size())
			{
				const auto& front = m_blockedUsers.front();

				string accountId;
				front.GetAsString(accountId);

				ActivateOutput(pActInfo, eOut_AccountId, accountId);

				m_blockedUsers.pop();
			}
		}

		ActivateOutput(pActInfo, eOut_Remaining, static_cast<int>(m_blockedUsers.size()));
	}
}

void CFlowNode_GetFriends::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Retrieve", _HELP("Query the platform for the list of friends.")),
		InputPortConfig_Void("GetNext", _HELP("Get the next account that was retrieved (if available).")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<int>("Remaining", _HELP("Remaining friends you can get from the list using GetNext.")),
		OutputPortConfig<string>("AccountId", _HELP("The account identifier of the friend.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Allows iteration of the list of friends connected to the account on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetFriends::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{

		if (IsPortActive(pActInfo, eIn_Retrieve))
		{
			// Empty the friends queue
			m_friends = {};

			EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (IService* pService = Helper::GetService(platform))
			{
				const auto friends = pService->GetFriendAccounts();
				for (const auto pAccount : friends)
				{
					m_friends.emplace(pAccount->GetIdentifier());
				}
			}
		}
		else if (IsPortActive(pActInfo, eIn_GetNext))
		{
			if (m_friends.size())
			{
				const auto& front = m_friends.front();

				string accountId;
				front.GetAsString(accountId);

				ActivateOutput(pActInfo, eOut_AccountId, accountId);

				m_friends.pop();
			}
		}

		ActivateOutput(pActInfo, eOut_Remaining, static_cast<int>(m_friends.size()));
	}
}

void CFlowNode_GetLocalAccount::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the local account for the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the account was found. False otherwise.")),
		OutputPortConfig<string>("AccountId", _HELP("The account identifier of the local account.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the identifier of the local account on the platform selected.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetLocalAccount::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		bool result = false;
		if (IService* pService = Helper::GetService(platform))
		{
			if (IAccount* pAccount = pService->GetLocalAccount())
			{
				string accountId;
				if (pAccount->GetIdentifier().GetAsString(accountId))
				{
					result = true;
					ActivateOutput(pActInfo, eOut_AccountId, accountId);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get the account identifier as string for account the local account for the platform '%s'.", Helper::GetServiceName(platform));
				}
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find the local account on the platform '%s'.", Helper::GetServiceName(platform));
			}
		}

		ActivateOutput(pActInfo, eOut_Success, result);
	}
}

void CFlowNode_GetMutedAccounts::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Retrieve", _HELP("Query the platform for the list of muted accounts.")),
		InputPortConfig_Void("GetNext", _HELP("Get the next account that was retrieved (if available).")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<int>("Remaining", _HELP("Remaining muted accounts you can get from the list using GetNext.")),
		OutputPortConfig<string>("AccountId", _HELP("The account identifier of the muted user.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Allows iteration of the list of muted accounts relative to the local account.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetMutedAccounts::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{

		if (IsPortActive(pActInfo, eIn_Retrieve))
		{
			// Empty the friends queue
			m_mutedUsers = {};

			EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (IService* pService = Helper::GetService(platform))
			{
				const auto mutedUsers = pService->GetMutedAccounts();
				for (const auto pAccount : mutedUsers)
				{
					m_mutedUsers.emplace(pAccount->GetIdentifier());
				}
			}
		}
		else if (IsPortActive(pActInfo, eIn_GetNext))
		{
			if (m_mutedUsers.size())
			{
				const auto& front = m_mutedUsers.front();

				string accountId;
				front.GetAsString(accountId);

				ActivateOutput(pActInfo, eOut_AccountId, accountId);

				m_mutedUsers.pop();
			}
		}

		ActivateOutput(pActInfo, eOut_Remaining, static_cast<int>(m_mutedUsers.size()));
	}
}

void CFlowNode_GetPermission::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Check the slected permission for the account on the platform specified.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform",   _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("Permission", 0, _HELP("The permission to check."),  "Permission", _UICONFIG("enum_int:Communication=0,Multiplayer=1,ViewProfiles=2,WebBrowser=3,UserGeneratedContent=4,MultiplayerSubscription=5,CrossNetworkPlay=6")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig_Void("Granted", _HELP("Fires if this was the result of the permission check.")),
		OutputPortConfig_Void("Denied", _HELP("Fires if this was the result of the permission check.")),
		OutputPortConfig_Void("Pending", _HELP("Fires if this was the result of the permission check.")),
		OutputPortConfig_Void("NotApplicable", _HELP("Fires if this was the result of the permission check.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks the status of the permission for the local account on the platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetPermission::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			EPermission checkPermission = EPermission::Communication;

			switch (GetPortInt(pActInfo, eIn_PermissionType))
			{
			default:
				break;
			case 1:
				checkPermission = EPermission::Multiplayer;
				break;
			case 2:
				checkPermission = EPermission::ViewProfiles;
				break;
			case 3:
				checkPermission = EPermission::WebBrowser;
				break;
			case 4:
				checkPermission = EPermission::UserGeneratedContent;
				break;
			case 5:
				checkPermission = EPermission::MultiplayerSubscription;
				break;
			case 6:
				checkPermission = EPermission::CrossNetworkPlay;
				break;
			}

			switch (pService->GetPermission(checkPermission))
			{
			case EPermissionResult::Granted:
				ActivateOutput(pActInfo, eOut_Granted, true);
				break;
			case EPermissionResult::Denied:
				ActivateOutput(pActInfo, eOut_Denied, true);
				break;
			case EPermissionResult::Pending:
				ActivateOutput(pActInfo, eOut_Pending, true);
				break;
			case EPermissionResult::NotApplicable:
				ActivateOutput(pActInfo, eOut_NotApplicable, true);
				break;
			}
		}
	}
}

void CFlowNode_GetPrivacyPermission::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Check for the selected permission.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the accounts belongs to."),  "Platform",   _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("TargetAccountId", _HELP("The target account identifier to check against.")),
		InputPortConfig<int>("Permission", 0, _HELP("The permission to query for."),  "Permission", _UICONFIG("enum_int:CanVoiceChat=0,CanTextChat=1,CanJoinOrInvite=2")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig_Void("Granted", _HELP("Fires if this was the result of the permission check.")),
		OutputPortConfig_Void("Denied", _HELP("Fires if this was the result of the permission check.")),
		OutputPortConfig_Void("Pending", _HELP("Fires if this was the result of the permission check.")),
		OutputPortConfig_Void("NotApplicable", _HELP("Fires if this was the result of the permission check.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if the requested permission is granted from the local account to the target account.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetPrivacyPermission::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_TargetAccountId));

			EPrivacyPermission checkPermission = EPrivacyPermission::VoiceCommunication;

			switch (GetPortInt(pActInfo, eIn_PermissionType))
			{
			default:
				break;
			case 1:
				checkPermission = EPrivacyPermission::TextCommunication;
				break;
			case 2:
				checkPermission = EPrivacyPermission::PlayMultiplayer;
				break;
			}

			switch (pService->GetPrivacyPermission(accountId, checkPermission))
			{
			case EPermissionResult::Granted:
				ActivateOutput(pActInfo, eOut_Granted, true);
				break;
			case EPermissionResult::Denied:
				ActivateOutput(pActInfo, eOut_Denied, true);
				break;
			case EPermissionResult::Pending:
				ActivateOutput(pActInfo, eOut_Pending, true);
				break;
			case EPermissionResult::NotApplicable:
				ActivateOutput(pActInfo, eOut_NotApplicable, true);
				break;
			}
		}
		else
		{
			ActivateOutput(pActInfo, eOut_NotApplicable, true);
		}
	}
}

void CFlowNode_RefreshBlockedAccounts::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Refresh", _HELP("Refresh the block list of the local account.")),
		InputPortConfig<int>("Platform", 0, _HELP("The to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Refreshes the internal cache of the block list. "
		"Listen for OnPersonaStateChange event for changes.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RefreshBlockedAccounts::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Refresh))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			pService->RefreshBlockedAccounts();
		}
	}
}

void CFlowNode_RefreshMutedAccounts::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Refresh", _HELP("Refresh the muted user list of the local account.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Refreshes the internal cache of the muted users. "
		"Listen for OnPersonaStateChange event for changes (PlatformListener Node).");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RefreshMutedAccounts::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Refresh))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			pService->RefreshBlockedAccounts();
		}
	}
}

void CFlowNode_RefreshPermissions::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Refresh", _HELP("Refresh the permissions of the local account.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Refreshes the internal cache of the platform permissions. "
		"Listen for OnPersonaStateChange event for changes (PlatformListener Node).");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RefreshPermissions::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Refresh))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			pService->RefreshPermissions();
		}
	}
}

void CFlowNode_RefreshPrivacyPermissions::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Refresh", _HELP("Refresh the permissions of the local account.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("SystemContainerId", -1, _HELP("The Id of the System:Container holding the list of target account Ids. " "Use -1 to use accounts added to the local session instead."), "SystemContainerId"),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP(
		"Refreshes the internal cache of the privacy permissions between the local and target player(s) specified platform. "
		"If no target accounts are specified, will use accounts added to the local session as targets. "
		"Listen for OnPersonaStateChange event for changes."
	);
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RefreshPrivacyPermissions::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		if (IsPortActive(pActInfo, eIn_Refresh))
		{
			const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (IService* pService = Helper::GetService(platform))
			{
				const int containerId = crymath::clamp(GetPortInt(pActInfo, eIn_SystemContainerId), -1, std::numeric_limits<int>::max());
				if (containerId > -1)
				{
					if (IFlowSystemContainerPtr pContainer = gEnv->pFlowSystem->GetContainer(containerId))
					{
						DynArray<AccountIdentifier> accounts;
						for (int i = 0; i < pContainer->GetItemCount(); i++)
						{
							string elem;
							if (!pContainer->GetItem(i).GetValueWithConversion<string>(elem))
							{
								const string platformString = Helper::GetServiceName(platform);
								CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Container '%s:%d' type mis-match.", platformString.c_str(), containerId);
								break;
							}

							AccountIdentifier accountId;
							pService->SetIdentifierFromString(accountId, elem);
							accounts.emplace_back(accountId);
						}

						pService->RefreshPrivacyPermissions(&accounts);
					}
					else
					{
						const string platformString = Helper::GetServiceName(platform);
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Container '%s:%d' does not exist.", platformString.c_str(), containerId);
					}
				}
				else
				{
					pService->RefreshPrivacyPermissions(nullptr);
				}
			}
		}
	}
}

void CFlowNode_RequestUserInformation::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Request", _HELP("Acquires account details pertaining to the specified platform.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("AccountId", _HELP("The account identifier for the account to query.")),
		InputPortConfig<bool>("Avatar", _HELP("Whether to request the avatar of the account along with the name of the user.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("NeedToWait", _HELP("True if you need to wait for a OnPersonaChange event. False if the requested data is already available.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Requests user information from remote server (Name, Avatar) for the specified account on the specified platform. Listen for OnPersonaUpdate events if necessary.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RequestUserInformation::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Request))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			AccountIdentifier accountId;
			pService->SetIdentifierFromString(accountId, GetPortString(pActInfo, eIn_AccountId));

			UserInformationMask request = EUserInformationFlags::eUIF_Name;

			if (GetPortBool(pActInfo, eIn_Avatar))
			{
				request |= EUserInformationFlags::eUIF_Avatar;
			}

			const bool success = pService->RequestUserInformation(accountId, request);

			ActivateOutput(pActInfo, eOut_NeedWait, success);
		}
	}
}

void CFlowNode_RichPresence::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the rich presence info.")),
		InputPortConfig_Void("Set", _HELP("Sets the rich presence info.")),
		InputPortConfig_Void("Clear", _HELP("Clears the rich presence info.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to get or set the rich presence info for."),"Platform",  _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Activity", _HELP("The activity string.")),
		InputPortConfig<string>("Headline", _HELP("The headline string.")),
		InputPortConfig<int>("TimerType",   -1, _HELP("The type of the timer, elapsed or remaining."),  "TimerType", _UICONFIG("enum_int:None=-1,Elapsed=0,Remaining=1")),
		InputPortConfig<int>("Seconds", _HELP("The seconds used by the timer for remaining or elapsed counters.")),
		InputPortConfig<int>("PartySize", _HELP("The size of the players group/party.")),
		InputPortConfig<int>("PartyMax", _HELP("The maximum size of the players group/party.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<string>("Activity", _HELP("The activity string.")),
		OutputPortConfig<string>("Headline", _HELP("The headline string.")),
		OutputPortConfig<int>("TimerType", _HELP("The type of the timer, elapsed or remaining.")),
		OutputPortConfig<int>("Seconds", _HELP("The seconds used by the timer for remaining or elapsed counters.")),
		OutputPortConfig<int>("PartySize", _HELP("The size of the players group/party.")),
		OutputPortConfig<int>("PartyMax", _HELP("The maximum size of the players group/party.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets or sets the rich presence data for the local user on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_RichPresence::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IAccount* pAccount = GetAccount(platform))
		{
			if (IsPortActive(pActInfo, eIn_Clear))
			{
				pAccount->ClearPresence();
			}
			else
			{
				SRichPresence presence;

				if (IsPortActive(pActInfo, eIn_Get))
				{
					pAccount->GetPresence(presence);
					OutputPresence(pActInfo, presence);
				}
				else if (IsPortActive(pActInfo, eIn_Set))
				{
					presence.activity = GetPortString(pActInfo, eIn_Activity).c_str();
					presence.headline = GetPortString(pActInfo, eIn_Headline).c_str();
					presence.countdownTimer = static_cast<SRichPresence::ETimer>(GetPortInt(pActInfo, eIn_TimerType));
					presence.seconds = GetPortInt(pActInfo, eIn_Seconds);
					presence.partySize = GetPortInt(pActInfo, eIn_PartySize);
					presence.partyMax = GetPortInt(pActInfo, eIn_PartyMax);

					pAccount->SetPresence(presence);
					OutputPresence(pActInfo, presence);
				}
			}
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get account from platform.");
		}
	}
}

IAccount* CFlowNode_RichPresence::GetAccount(EGamePlatform platform) const
{
	if (IService* pService = Helper::GetService(platform))
	{
		if (IAccount* pAccount = pService->GetLocalAccount())
		{
			return pAccount;
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find the local account on the platform '%s'.", pService->GetName());
		}
	}

	return nullptr;
}

void CFlowNode_RichPresence::OutputPresence(SActivationInfo* pActInfo, const SRichPresence& presence) const
{
	ActivateOutput(pActInfo, eOut_Activity, string(presence.activity));
	ActivateOutput(pActInfo, eOut_Headline, string(presence.headline));
	ActivateOutput(pActInfo, eOut_TimerType, static_cast<int>(presence.countdownTimer));
	ActivateOutput(pActInfo, eOut_Seconds, static_cast<int>(presence.seconds));
	ActivateOutput(pActInfo, eOut_PartySize, presence.partySize);
	ActivateOutput(pActInfo, eOut_PartyMax, presence.partyMax);
}

void CFlowNode_SetStatus::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Set", _HELP("Sets the status info.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to get or set the status info for."),"Platform",  _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Status", _HELP("The status string.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig_Void("OnSet", _HELP("Triggered when the status is set.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets or sets the status string for the local user on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SetStatus::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Set))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		if (IAccount* pAccount = GetAccount(platform))
		{
			pAccount->SetStatus(GetPortString(pActInfo, eIn_Status).c_str());
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get account from platform.");
		}

		ActivateOutput(pActInfo, eOut_OnSet, true);
	}
}

IAccount* CFlowNode_SetStatus::GetAccount(EGamePlatform platform) const
{
	if (IService* pService = Helper::GetService(platform))
	{
		if (IAccount* pAccount = pService->GetLocalAccount())
		{
			return pAccount;
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find the local account on the platform '%s'.", pService->GetName());
		}
	}

	return nullptr;
}

REGISTER_FLOW_NODE("GamePlatform:Account:CheckIsFriend", CFlowNode_CheckIsFriend);
REGISTER_FLOW_NODE("GamePlatform:Account:GetAccountInfo", CFlowNode_GetAccountInfo);
REGISTER_FLOW_NODE("GamePlatform:Account:GetAvatar", CFlowNode_GetAvatar);
REGISTER_FLOW_NODE("GamePlatform:Account:GetBlockedAccounts", CFlowNode_GetBlockedAccounts);
REGISTER_FLOW_NODE("GamePlatform:Account:GetFriends", CFlowNode_GetFriends);
REGISTER_FLOW_NODE("GamePlatform:Account:GetLocalAccount", CFlowNode_GetLocalAccount);
REGISTER_FLOW_NODE("GamePlatform:Service:GetMutedAccounts", CFlowNode_GetMutedAccounts);
REGISTER_FLOW_NODE("GamePlatform:Account:GetPermission", CFlowNode_GetPermission);
REGISTER_FLOW_NODE("GamePlatform:Account:GetPrivacyPermission", CFlowNode_GetPrivacyPermission);
REGISTER_FLOW_NODE("GamePlatform:Account:RefreshBlockedUsers", CFlowNode_RefreshBlockedAccounts);
REGISTER_FLOW_NODE("GamePlatform:Account:RefreshMutedUsers", CFlowNode_RefreshMutedAccounts);
REGISTER_FLOW_NODE("GamePlatform:Account:RefreshPermissions", CFlowNode_RefreshPermissions);
REGISTER_FLOW_NODE("GamePlatform:Account:RefreshPrivacyPermissions", CFlowNode_RefreshPrivacyPermissions);
REGISTER_FLOW_NODE("GamePlatform:Account:RequestUserInfo", CFlowNode_RequestUserInformation);
REGISTER_FLOW_NODE("GamePlatform:Account:RichPresence", CFlowNode_RichPresence);
REGISTER_FLOW_NODE("GamePlatform:Account:SetStatus", CFlowNode_SetStatus);

} // Cry::GamePlatform