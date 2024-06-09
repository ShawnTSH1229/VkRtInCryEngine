// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "AccountEventNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnAccountAdded::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier that was added.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have been added since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnAccountAdded::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);
}

void CFlowNode_OnAccountNameChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier that changed their name.")),
			OutputPortConfig<string>("Name", _HELP("The new name of the account.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have changed their name since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnAccountNameChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);

	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);
	ActivateOutput(pActInfo, eOut_Name, string(event.GetAccountName().c_str()));
}

void CFlowNode_OnAccountRemoved::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier that was removed.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have been removed since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnAccountRemoved::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);

	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);
}

void CFlowNode_OnAvatarImageLoaded::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier that loaded their avatar.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have loaded avatar data since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnAvatarImageLoaded::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);

	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);
}

void CFlowNode_OnPersonaStateChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier of the account whose persona data was updated.")),
			OutputPortConfig<bool>("Name", _HELP("True if the name of the user changed.")),
			OutputPortConfig<bool>("Status", _HELP("True if the status of the user changed.")),
			OutputPortConfig<bool>("CameOnline", _HELP("True if the user came online.")),
			OutputPortConfig<bool>("WentOffline", _HELP("True if the user went offline.")),
			OutputPortConfig<bool>("GamePlayed", _HELP("True if the user started playing a game.")),
			OutputPortConfig<bool>("GameServer", _HELP("True if the user joined a server.")),
			OutputPortConfig<bool>("ChangeAvatar", _HELP("True if the user changed their avatar.")),
			OutputPortConfig<bool>("JoinedSource", _HELP("True if the user joined source.")),
			OutputPortConfig<bool>("LeftSource", _HELP("True if the user left source.")),
			OutputPortConfig<bool>("RelationshipChanged", _HELP("True if the user added, removed or otherwise changed their friendship with the local account.")),
			OutputPortConfig<bool>("BlockedStateChanged", _HELP("True if the block state changed between the local account the specified account.")),
			OutputPortConfig<bool>("MutedStateChanged", _HELP("True if the muted state changed between the local account the specified account.")),
			OutputPortConfig<bool>("PrivacyStateChanged", _HELP("True if the privacy state changed between the local account the specified account.")),
			OutputPortConfig<bool>("NameFirstSet", _HELP("True if the user changed their first name on their profile.")),
			OutputPortConfig<bool>("Broadcast", _HELP("True if the user broadcast state changed.")),
			OutputPortConfig<bool>("Nickname", _HELP("True if the nickname of the user changed.")),
			OutputPortConfig<bool>("SteamLevel", _HELP("True if the Steam level of the user changed (Only relevant on the Steam platform).")),
			OutputPortConfig<bool>("RichPresence", _HELP("True if the rich presence data was updated on the account.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have changed their persona data since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnPersonaStateChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountIdString;
	event.GetAccountId().GetAsString(accountIdString);
	ActivateOutput(pActInfo, eOut_AccountId, accountIdString);

	ActivateOutput(pActInfo, eOut_Name, event.GetChangeFlags().Check(EPersonaChangeFlags::Name));
	ActivateOutput(pActInfo, eOut_Status, event.GetChangeFlags().Check(EPersonaChangeFlags::Status));
	ActivateOutput(pActInfo, eOut_CameOnline, event.GetChangeFlags().Check(EPersonaChangeFlags::CameOnline));
	ActivateOutput(pActInfo, eOut_WentOffline, event.GetChangeFlags().Check(EPersonaChangeFlags::WentOffline));
	ActivateOutput(pActInfo, eOut_GamePlayed, event.GetChangeFlags().Check(EPersonaChangeFlags::GamePlayed));
	ActivateOutput(pActInfo, eOut_GameServer, event.GetChangeFlags().Check(EPersonaChangeFlags::GameServer));
	ActivateOutput(pActInfo, eOut_ChangeAvatar, event.GetChangeFlags().Check(EPersonaChangeFlags::ChangeAvatar));
	ActivateOutput(pActInfo, eOut_JoinedSource, event.GetChangeFlags().Check(EPersonaChangeFlags::JoinedSource));
	ActivateOutput(pActInfo, eOut_LeftSource, event.GetChangeFlags().Check(EPersonaChangeFlags::LeftSource));
	ActivateOutput(pActInfo, eOut_RelationshipChanged, event.GetChangeFlags().Check(EPersonaChangeFlags::RelationshipChanged));
	ActivateOutput(pActInfo, eOut_BlockedStateChanged, event.GetChangeFlags().Check(EPersonaChangeFlags::BlockedStateChanged));
	ActivateOutput(pActInfo, eOut_MutedStateChanged, event.GetChangeFlags().Check(EPersonaChangeFlags::MutedStateChanged));
	ActivateOutput(pActInfo, eOut_PrivacyStateChanged, event.GetChangeFlags().Check(EPersonaChangeFlags::PrivacyStateChanged));
	ActivateOutput(pActInfo, eOut_NameFirstSet, event.GetChangeFlags().Check(EPersonaChangeFlags::NameFirstSet));
	ActivateOutput(pActInfo, eOut_FacebookInfo, event.GetChangeFlags().Check(EPersonaChangeFlags::FacebookInfo));
	ActivateOutput(pActInfo, eOut_Nickname, event.GetChangeFlags().Check(EPersonaChangeFlags::Nickname));
	ActivateOutput(pActInfo, eOut_SteamLevel, event.GetChangeFlags().Check(EPersonaChangeFlags::SteamLevel));
	ActivateOutput(pActInfo, eOut_RichPresence, event.GetChangeFlags().Check(EPersonaChangeFlags::RichPresence));
}

void CFlowNode_OnUserPermissionChanged::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("AccountId", _HELP("The account identifier that received the permission change.")),
			OutputPortConfig<bool>("HasPermission", _HELP("Whether the new permission was granted (true) or revoked (false).")),
			OutputPortConfig_Void("Communication", _HELP("Triggers if this permission was changed.")),
			OutputPortConfig_Void("Multiplayer", _HELP("Triggers if this permission was changed.")),
			OutputPortConfig_Void("ViewProfiles", _HELP("Triggers if this permission was changed.")),
			OutputPortConfig_Void("WebBrowser", _HELP("Triggers if this permission was changed.")),
			OutputPortConfig_Void("UserGeneratedContent", _HELP("Triggers if this permission was changed.")),
			OutputPortConfig_Void("MultiplayerSubscription", _HELP("Triggers if this permission was changed.")),
			OutputPortConfig_Void("CrossNetworkPlay", _HELP("Triggers if this permission was changed.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more accounts have been added since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnUserPermissionChanged::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string accountId;
	event.GetAccountId().GetAsString(accountId);

	ActivateOutput(pActInfo, eOut_AccountId, accountId);

	ActivateOutput(pActInfo, eOut_HasPermission, event.HasPermission());

	switch (event.GetPermission())
	{
	case IService::EPermission::Communication:
		ActivateOutput(pActInfo, eOut_PermCommunication, true);
		break;
	case IService::EPermission::Multiplayer:
		ActivateOutput(pActInfo, eOut_PermMultiplayer, true);
		break;
	case IService::EPermission::ViewProfiles:
		ActivateOutput(pActInfo, eOut_PermViewProfiles, true);
		break;
	case IService::EPermission::WebBrowser:
		ActivateOutput(pActInfo, eOut_PermWebBrowser, true);
		break;
	case IService::EPermission::UserGeneratedContent:
		ActivateOutput(pActInfo, eOut_PermUserGeneratedContent, true);
		break;
	case IService::EPermission::MultiplayerSubscription:
		ActivateOutput(pActInfo, eOut_PermMultiplayerSubscription, true);
		break;
	case IService::EPermission::CrossNetworkPlay:
		ActivateOutput(pActInfo, eOut_PermCrossNetworkPlay, true);
		break;
	}
}

REGISTER_FLOW_NODE("GamePlatform:Listener:Account:OnAccountAdded", CFlowNode_OnAccountAdded);
REGISTER_FLOW_NODE("GamePlatform:Listener:Account:OnAccountNameChanged", CFlowNode_OnAccountNameChanged);
REGISTER_FLOW_NODE("GamePlatform:Listener:Account:OnAccountRemoved", CFlowNode_OnAccountRemoved);
REGISTER_FLOW_NODE("GamePlatform:Listener:Account:OnAvatarImageLoaded", CFlowNode_OnAvatarImageLoaded);
REGISTER_FLOW_NODE("GamePlatform:Listener:Account:OnPersonaStateChanged", CFlowNode_OnPersonaStateChanged);
REGISTER_FLOW_NODE("GamePlatform:Listener:Account:OnUserPermissionChanged", CFlowNode_OnUserPermissionChanged);

} // Cry::GamePlatform
