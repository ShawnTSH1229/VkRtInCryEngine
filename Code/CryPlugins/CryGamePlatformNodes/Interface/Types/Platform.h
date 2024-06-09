// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/FixedString.h>
#include <CryGamePlatform/Interface/PlatformTypes.h>

namespace Cry::GamePlatform
{

enum class EGamePlatform
{
	None,
	Main,

	Steam,
	Playstation,
	Xbox,
	Discord
};

inline void ReflectType(Schematyc::CTypeDesc<EGamePlatform>& typeDesc)
{
	typeDesc.SetGUID("{92B0E12F-D4D5-4681-B432-FFFDA97FDB22}"_cry_guid);
	typeDesc.SetLabel("GamePlatform");
	typeDesc.SetDescription("The game platform");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EGamePlatform::None);
	typeDesc.AddConstant(EGamePlatform::None, "none", "None");
	typeDesc.AddConstant(EGamePlatform::Main, "main", "Main");
	typeDesc.AddConstant(EGamePlatform::Discord, "discord", "Discord");
	typeDesc.AddConstant(EGamePlatform::Steam, "steam", "Steam");
	typeDesc.AddConstant(EGamePlatform::Playstation, "playstation", "Playstation");
	typeDesc.AddConstant(EGamePlatform::Xbox, "xbox", "Xbox");
}

inline void ReflectType(Schematyc::CTypeDesc<AccountIdentifier>& typeDesc)
{
	typeDesc.SetGUID("{88D475BC-545F-4642-8FBF-D2C7396C2607}"_cry_guid);
	typeDesc.SetLabel("AccountIdentifier");
	typeDesc.SetDescription("An identifier for a platform-specific account");
}

inline void ApplicationIdentifierToString(Schematyc::IString& result, const ApplicationIdentifier& value)
{
	string out;
	value.GetAsString(out);
	result.assign(out.c_str());
}

inline void ReflectType(Schematyc::CTypeDesc<ApplicationIdentifier>& typeDesc)
{
	typeDesc.SetGUID("{1699E3C5-CD13-470D-8B50-C5CCF82B8339}"_cry_guid);
	typeDesc.SetLabel("ApplicationIdentifier");
	typeDesc.SetDescription("An identifier for a platform-specific application (product)");
	typeDesc.SetToStringOperator<&ApplicationIdentifierToString>();
}

inline void ReflectType(Schematyc::CTypeDesc<InventoryItemIdentifier>& typeDesc)
{
	typeDesc.SetGUID("{296E9D9C-2668-4141-9015-C4D6FA5A78A7}"_cry_guid);
	typeDesc.SetLabel("InventoryItemIdentifier");
	typeDesc.SetDescription("An identifier for a platform-specific inventory item");
}

inline void ReflectType(Schematyc::CTypeDesc<TransactionIdentifier>& typeDesc)
{
	typeDesc.SetGUID("{7B08AD60-DC4B-4279-9723-CD98050D3118}"_cry_guid);
	typeDesc.SetLabel("TransactionIdentifier");
	typeDesc.SetDescription("An identifier for a platform-specific transaction");
}

inline void ReflectType(Schematyc::CTypeDesc<SRichPresence::ETimer>& typeDesc)
{
	typeDesc.SetGUID("{97C4F057-77AB-47EB-89F2-5B284F957F67}"_cry_guid);
	typeDesc.SetLabel("RichPresenceTimerType");
	typeDesc.SetDescription("Duration type for rich presence timer");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(SRichPresence::ETimer::Elapsed);
	typeDesc.AddConstant(SRichPresence::ETimer::None, "none", "None");
	typeDesc.AddConstant(SRichPresence::ETimer::Elapsed, "elapsed", "Elapsed");
	typeDesc.AddConstant(SRichPresence::ETimer::Remaining, "remaining", "Remaining");
}

inline void ReflectType(Schematyc::CTypeDesc<SRichPresence>& typeDesc)
{
	typeDesc.SetGUID("{86275EE6-AEAB-425F-9DB5-73B7BCF0C563}"_cry_guid);
	typeDesc.SetLabel("RichPresence");
	typeDesc.SetDescription("Rich presence data for a platform specific account");
	typeDesc.SetDefaultValue(SRichPresence());
	typeDesc.AddMember(&SRichPresence::activity, 'actv', "activity", "Activity", "", "");
	typeDesc.AddMember(&SRichPresence::headline, 'head', "headline", "Headline", "", "");
	typeDesc.AddMember(&SRichPresence::countdownTimer, 'time', "countdowntimer", "Countdown Timer", "", SRichPresence::ETimer::None);
	typeDesc.AddMember(&SRichPresence::seconds, 'sec', "seconds", "Seconds", "", 0);
	typeDesc.AddMember(&SRichPresence::partySize, 'psz', "partysize", "Party Size", "", 0);
	typeDesc.AddMember(&SRichPresence::partyMax, 'pmax', "partymax", "Party Max", "", 0);

}

inline void ReflectType(Schematyc::CTypeDesc<EDialog>& typeDesc)
{
	typeDesc.SetGUID("{EFD225DF-5B33-44BA-BFE2-8E77C1F637EB}"_cry_guid);
	typeDesc.SetLabel("DialogType");
	typeDesc.SetDescription("Type of in-game overlay dialog");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EDialog::Achievements);
	typeDesc.AddConstant(EDialog::Achievements, "achievements", "Achievements");
	typeDesc.AddConstant(EDialog::Community, "community", "Community");
	typeDesc.AddConstant(EDialog::Friends, "friends", "Friends");
	typeDesc.AddConstant(EDialog::Group, "group", "Group");
	typeDesc.AddConstant(EDialog::Players, "players", "Players");
	typeDesc.AddConstant(EDialog::Settings, "settings", "Settings");
	typeDesc.AddConstant(EDialog::Stats, "stats", "Stats");
}

inline void ReflectType(Schematyc::CTypeDesc<EConnectionStatus>& typeDesc)
{
	typeDesc.SetGUID("{0416174F-9E92-4845-958F-A36C518E7198}"_cry_guid);
	typeDesc.SetLabel("ConnectionStatus");
	typeDesc.SetDescription("Represents the current connection to the platform's services");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EConnectionStatus::Connected);
	typeDesc.AddConstant(EConnectionStatus::Connected, "connected", "Connected");
	typeDesc.AddConstant(EConnectionStatus::Connecting, "connecting", "Connecting");
	typeDesc.AddConstant(EConnectionStatus::Disconnected, "disconnected", "Disconnected");
	typeDesc.AddConstant(EConnectionStatus::ObtainingIP, "obtainingip", "ObtainingIP");
}

inline void ReflectType(Schematyc::CTypeDesc<EFriendRelationship>& typeDesc)
{
	typeDesc.SetGUID("{0B298F7F-B234-425B-8B56-7AE2F95B9964}"_cry_guid);
	typeDesc.SetLabel("FriendRelationship");
	typeDesc.SetDescription("Represents the relationship with the local account vs another account");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EFriendRelationship::None);
	typeDesc.AddConstant(EFriendRelationship::None, "none", "None");
	typeDesc.AddConstant(EFriendRelationship::Friend, "friend", "Friend");
	typeDesc.AddConstant(EFriendRelationship::RequestSent, "requestsent", "Request Sent");
	typeDesc.AddConstant(EFriendRelationship::RequestReceived, "requestreceived", "Request Received");
	typeDesc.AddConstant(EFriendRelationship::Blocked, "blocked", "Blocked");
}

inline void ReflectType(Schematyc::CTypeDesc<EAvatarSize>& typeDesc)
{
	typeDesc.SetGUID("{371888CA-16BE-458D-B15F-5047CD7DC90B}"_cry_guid);
	typeDesc.SetLabel("AvatarSize");
	typeDesc.SetDescription("Represents the size of an avatar");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EAvatarSize::Small);
	typeDesc.AddConstant(EAvatarSize::Small, "small", "Small");
	typeDesc.AddConstant(EAvatarSize::Medium, "medium", "Medium");
	typeDesc.AddConstant(EAvatarSize::Large, "large", "Large");
}

inline void ReflectType(Schematyc::CTypeDesc<EUserTargetedDialog>& typeDesc)
{
	typeDesc.SetGUID("{2F205FB6-E8F0-468C-A6F4-C7A8EF7356CD}"_cry_guid);
	typeDesc.SetLabel("UserTargetedDialog");
	typeDesc.SetDescription("Type of in-game overlay dialog with a target user");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EUserTargetedDialog::UserInfo);
	typeDesc.AddConstant(EUserTargetedDialog::UserInfo, "userinfo", "User Info");
	typeDesc.AddConstant(EUserTargetedDialog::FriendAdd, "friendadd", "Add Friend");
	typeDesc.AddConstant(EUserTargetedDialog::FriendRemove, "friendremove", "Remove Friend");
	typeDesc.AddConstant(EUserTargetedDialog::FriendRequestAccept, "friendrequestaccept", "Accept Friend Request");
	typeDesc.AddConstant(EUserTargetedDialog::FriendRequestIgnore, "friendrequestignore", "Ignore Friend Request");
	typeDesc.AddConstant(EUserTargetedDialog::Chat, "chat", "Chat");
	typeDesc.AddConstant(EUserTargetedDialog::JoinTrade, "jointrade", "Join Trade");
	typeDesc.AddConstant(EUserTargetedDialog::Stats, "stats", "Stats");
	typeDesc.AddConstant(EUserTargetedDialog::Achievements, "achievements", "Achievements");
}

inline void ReflectType(Schematyc::CTypeDesc<CEnumFlags<EUserInformationFlags>>& typeDesc)
{
	typeDesc.SetGUID("{82F404C1-B9D5-49EB-A849-AFE98B7FDA0A}"_cry_guid);
	typeDesc.SetLabel("UserInformationFlags");
	typeDesc.SetDescription("Flags to be passed to platform service for querying/downloading information about users.");
	typeDesc.SetDefaultValue(eUIF_Name);
}

inline void ReflectType(Schematyc::CTypeDesc<CEnumFlags<EPersonaChangeFlags>>& typeDesc)
{
	typeDesc.SetGUID("{2EB939E2-F889-4201-932F-5F71EFA38BC7}"_cry_guid);
	typeDesc.SetLabel("PersonaChangeFlags");
	typeDesc.SetDescription("Flags used to determine what persona data has changed.");
	typeDesc.SetDefaultValue(EPersonaChangeFlags::Name);
}

} // Cry::GamePlatform