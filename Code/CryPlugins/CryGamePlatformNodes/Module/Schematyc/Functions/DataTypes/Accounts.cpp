// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <Types/DynArray.h>
#include <Types/Service.h>
#include <Schematyc/GamePlatformEnv.h>
#include <Schematyc/Utils.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

YASLI_ENUM_BEGIN(EPersonaChangeFlags, "PersonaChangeFlags")
YASLI_ENUM_VALUE(EPersonaChangeFlags::Name, "Name")
YASLI_ENUM_VALUE(EPersonaChangeFlags::Status, "Status");
YASLI_ENUM_VALUE(EPersonaChangeFlags::CameOnline, "Came Online");
YASLI_ENUM_VALUE(EPersonaChangeFlags::WentOffline, "Went Offline");
YASLI_ENUM_VALUE(EPersonaChangeFlags::GamePlayed, "Game Played");
YASLI_ENUM_VALUE(EPersonaChangeFlags::GameServer, "Game Server");
YASLI_ENUM_VALUE(EPersonaChangeFlags::ChangeAvatar, "Change Avatar");
YASLI_ENUM_VALUE(EPersonaChangeFlags::JoinedSource, "Joined Source");
YASLI_ENUM_VALUE(EPersonaChangeFlags::LeftSource, "Left Source");
YASLI_ENUM_VALUE(EPersonaChangeFlags::RelationshipChanged, "Relationship Changed");
YASLI_ENUM_VALUE(EPersonaChangeFlags::BlockedStateChanged, "Blocked State Changed");
YASLI_ENUM_VALUE(EPersonaChangeFlags::MutedStateChanged, "Muted State Changed");
YASLI_ENUM_VALUE(EPersonaChangeFlags::PrivacyStateChanged, "Privacy State Changed");
YASLI_ENUM_VALUE(EPersonaChangeFlags::NameFirstSet, "First Name Set");
YASLI_ENUM_VALUE(EPersonaChangeFlags::FacebookInfo, "Facebook Info");
YASLI_ENUM_VALUE(EPersonaChangeFlags::Nickname, "Nickname");
YASLI_ENUM_VALUE(EPersonaChangeFlags::SteamLevel, "Steam Level");
YASLI_ENUM_VALUE(EPersonaChangeFlags::RichPresence, "Rich Presence");
YASLI_ENUM_END()

YASLI_ENUM_BEGIN(EUserInformationFlags, "UserInformationFlags")
YASLI_ENUM_VALUE(eUIF_Avatar, "Avatar");
YASLI_ENUM_VALUE(eUIF_Name, "Name");
YASLI_ENUM_END()

namespace _AccountIdentifier
{

AccountIdentifier Create(EGamePlatform platform, const Schematyc::CSharedString& id)
{
	AccountIdentifier accountIdentifier;

	if (IService* pService = Helper::GetService(platform))
	{
		pService->SetIdentifierFromString(accountIdentifier, id.c_str());
	}

	return accountIdentifier;
}

EGamePlatform GetPlatform(const AccountIdentifier& accountId)
{
	return Helper::GetServiceEnum(accountId.Service());
}

Schematyc::CSharedString ToString(const AccountIdentifier& accountId)
{
	stack_string result;
	accountId.GetAsString(result);
	return Schematyc::CSharedString(result.c_str());
}

bool IsEqual(const AccountIdentifier& a, const AccountIdentifier& b)
{
	return a == b;
}

bool NotEqual(const AccountIdentifier& a, const AccountIdentifier& b)
{
	return !IsEqual(a,b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(AccountIdentifier));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{FCB13C20-1073-4B9F-B0D4-A271573A9792}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'id', "Account");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInput(2, 'accs', "IdString");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{9B6C21B8-F62A-49F2-90D6-3281855F6046}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{7023BDB4-3AB4-4D8D-BE2E-6BFDC568FA21}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{45B7486E-903A-4C92-9F2D-5DC1FCF8B70B}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{BE64FF3F-B14D-46A8-A5B8-F03333C97AB4}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<AccountIdentifier>, "{4FDA8AEE-061E-47BB-B252-A938EE77FEB6}"_cry_guid, "ToArray");
		pFunction->BindInput(1, 'accs', "DynArray[Account Identifier]");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'arr', "Array[Account Identifier]");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::_AccountIdentifier

namespace AvatarSize
{

Schematyc::CSharedString ToString(EAvatarSize size)
{
	switch (size)
	{
	case EAvatarSize::Large: return "Large";
	case EAvatarSize::Medium: return "Medium";
	case EAvatarSize::Small: return "Small";
	}

	return "Invalid";
}

EAvatarSize FromString(const Schematyc::CSharedString& size)
{
	if (stricmp(size.c_str(), "Large") == 0)
	{
		return EAvatarSize::Large;
	}

	if (stricmp(size.c_str(), "Medium") == 0)
	{
		return EAvatarSize::Medium;
	}
	
	return EAvatarSize::Small;
}

bool IsEqual(EAvatarSize a, EAvatarSize b)
{
	return a == b;
}

bool NotEqual(EAvatarSize a, EAvatarSize b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(EAvatarSize));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{423CF5E0-925F-4844-9DD3-328889EF118F}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'asz', "AvatarSize");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{02F14636-09C3-4F84-82CF-E91D7597C30E}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'asz', "AvatarSize");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{5373CFDF-3216-460C-895B-7ADB8D47CE83}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{6914A064-FB6E-4539-9A9E-B986F2F4BDCF}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
}

} // AvatarSize

namespace Permission
{

Schematyc::CSharedString ToString(const IService::EPermission& permission)
{
	switch (permission)
	{
	case IService::EPermission::Communication: return "Communication";
	case IService::EPermission::Multiplayer: return "Multiplayer";
	case IService::EPermission::ViewProfiles: return "ViewProfiles";
	case IService::EPermission::WebBrowser: return "WebBrowser";
	case IService::EPermission::UserGeneratedContent: return "UserGeneratedContent";
	case IService::EPermission::MultiplayerSubscription: return "MultiplayerSubscription";
	case IService::EPermission::CrossNetworkPlay: return "CrossNetworkPlay";
	}

	return "Invalid";
}

IService::EPermission FromString(const Schematyc::CSharedString& permission)
{
	if (stricmp(permission.c_str(), "Communication") == 0)
	{
		return IService::EPermission::Communication;
	}

	if (stricmp(permission.c_str(), "Multiplayer") == 0)
	{
		return IService::EPermission::Multiplayer;
	}

	if (stricmp(permission.c_str(), "ViewProfiles") == 0)
	{
		return IService::EPermission::ViewProfiles;
	}

	if (stricmp(permission.c_str(), "WebBrowser") == 0)
	{
		return IService::EPermission::WebBrowser;
	}

	if (stricmp(permission.c_str(), "UserGeneratedContent") == 0)
	{
		return IService::EPermission::UserGeneratedContent;
	}

	if (stricmp(permission.c_str(), "MultiplayerSubscription") == 0)
	{
		return IService::EPermission::MultiplayerSubscription;
	}

	CRY_ASSERT(stricmp(permission.c_str(), "CrossNetworkPlay") == 0, "Unexpected value in FromString.");

	return IService::EPermission::CrossNetworkPlay;
}

bool IsEqual(const IService::EPermission& a, const IService::EPermission& b)
{
	return a == b;
}

bool NotEqual(const IService::EPermission& a, const IService::EPermission& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::EPermission));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{56C78127-4ADA-4355-AE27-13A81827C0CC}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'perm', "Permission");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{DCD5AE0E-6052-4D56-BDCC-292DDA3369C4}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'perm', "Permission");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{81038651-21BE-426B-B465-509361916651}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pa', "A");
		pFunction->BindInput(2, 'pb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{58563B4C-B7C3-4CC3-BA62-6B2813605DD5}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pa', "A");
		pFunction->BindInput(2, 'pb', "B");
		scope.Register(pFunction);
	}
}

} // Permission

namespace PermissionResult
{

Schematyc::CSharedString ToString(const IService::EPermissionResult& permissionResult)
{
	switch (permissionResult)
	{
	case IService::EPermissionResult::NotApplicable: return "NotApplicable";
	case IService::EPermissionResult::Pending: return "Pending";
	case IService::EPermissionResult::Granted: return "Granted";
	case IService::EPermissionResult::Denied: return "Denied";
	}

	return "Invalid";
}

IService::EPermissionResult FromString(const Schematyc::CSharedString& permissionResult)
{
	if (stricmp(permissionResult.c_str(), "Pending") == 0)
	{
		return IService::EPermissionResult::Pending;
	}

	if (stricmp(permissionResult.c_str(), "Granted") == 0)
	{
		return IService::EPermissionResult::Granted;
	}

	if (stricmp(permissionResult.c_str(), "Denied") == 0)
	{
		return IService::EPermissionResult::Denied;
	}
	
	return IService::EPermissionResult::NotApplicable;
}

bool IsEqual(const IService::EPermissionResult& a, const IService::EPermissionResult& b)
{
	return a == b;
}

bool NotEqual(const IService::EPermissionResult& a, const IService::EPermissionResult& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::EPermissionResult));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{4F0083DE-600C-48FE-9893-0A42955C39EB}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pres', "PermissionResult");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{8C2E250B-D482-47D4-9DF5-DD310B96504A}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'pres', "PermissionResult");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{A83FC4CC-2674-493A-AED5-06238F969B14}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pra', "A");
		pFunction->BindInput(2, 'prb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{8848F45C-3AD5-4001-ADA0-BAA486BF1C48}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pra', "A");
		pFunction->BindInput(2, 'prb', "B");
		scope.Register(pFunction);
	}
}

} // PermissionResult

namespace PersonaChangeFlags
{

CEnumFlags<EPersonaChangeFlags> AddFlags(const CEnumFlags<EPersonaChangeFlags>& flags, const CEnumFlags<EPersonaChangeFlags>& addFlags)
{
	CEnumFlags<EPersonaChangeFlags> result{ flags };
	result.Add(addFlags);
	return result;
}

CEnumFlags<EPersonaChangeFlags> RemoveFlags(const CEnumFlags<EPersonaChangeFlags>& flags, const CEnumFlags<EPersonaChangeFlags>& removeFlags)
{
	CEnumFlags<EPersonaChangeFlags> result{ flags };
	result.Remove(removeFlags);
	return result;
}

bool HasAnyFlags(const CEnumFlags<EPersonaChangeFlags>& flags, const CEnumFlags<EPersonaChangeFlags>& checkFlags)
{
	return flags.CheckAny(checkFlags);
}

bool HasAllFlags(const CEnumFlags<EPersonaChangeFlags>& flags, const CEnumFlags<EPersonaChangeFlags>& checkFlags)
{
	return flags.CheckAll(checkFlags);
}

Schematyc::CSharedString ToString(const CEnumFlags<EPersonaChangeFlags>& userInformationFlags)
{
	uint32 value = userInformationFlags.UnderlyingValue();
	string result;
	result.Format("%ul", value);
	return result;
}

CEnumFlags<EPersonaChangeFlags> FromString(const Schematyc::CSharedString& userDialog)
{
	uint32 flags = 0;
	sscanf_s(userDialog.c_str(), "%u", &flags);
	return CEnumFlags<EPersonaChangeFlags>(static_cast<EPersonaChangeFlags>(flags));
}

bool IsEqual(const CEnumFlags<EPersonaChangeFlags>& a, const CEnumFlags<EPersonaChangeFlags>& b)
{
	return a == b;
}

bool NotEqual(const CEnumFlags<EPersonaChangeFlags>& a, const CEnumFlags<EPersonaChangeFlags>& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CEnumFlags<EPersonaChangeFlags>));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&AddFlags, "{4EF5C24A-B464-462F-96C3-7E7DDF2F7D90}"_cry_guid, "AddFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "AddFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RemoveFlags, "{C616D700-79AD-4631-8813-AC8CF3A20470}"_cry_guid, "RemoveFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "RemoveFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&HasAnyFlags, "{AB76C10E-5220-46E1-889D-633E222AEFE1}"_cry_guid, "HasAnyFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "CheckFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&HasAllFlags, "{7D595A78-3849-4A5B-B3F2-098BECE0FFA5}"_cry_guid, "HasAllFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "CheckFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{4FF15715-F880-4EC2-BF2D-385788FEF6B3}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uif', "UserInfoFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{87752D6C-3923-456E-8078-63396306DADA}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'uif', "UserInfoFlags");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{BF55A1DD-61EE-47C3-814B-8E3548FE36C3}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "A");
		pFunction->BindInput(2, 'uifb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{70DF2414-778A-417E-A129-02FF62D25424}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "A");
		pFunction->BindInput(2, 'uifb', "B");
		scope.Register(pFunction);
	}
}

} // PersonaChangeFlags

namespace PrivacyPermission
{

Schematyc::CSharedString ToString(const IService::EPrivacyPermission& privacyPermission)
{
	switch (privacyPermission)
	{
	case IService::EPrivacyPermission::VoiceCommunication: return "VoiceCommunication";
	case IService::EPrivacyPermission::TextCommunication: return "TextCommunication";
	case IService::EPrivacyPermission::PlayMultiplayer: return "PlayMultiplayer";
	}

	return "Invalid";
}

IService::EPrivacyPermission FromString(const Schematyc::CSharedString& privacyPermission)
{
	if (stricmp(privacyPermission.c_str(), "VoiceCommunication") == 0)
	{
		return IService::EPrivacyPermission::VoiceCommunication;
	}

	if (stricmp(privacyPermission.c_str(), "TextCommunication") == 0)
	{
		return IService::EPrivacyPermission::TextCommunication;
	}

	return IService::EPrivacyPermission::PlayMultiplayer;
}

bool IsEqual(const IService::EPrivacyPermission& a, const IService::EPrivacyPermission& b)
{
	return a == b;
}

bool NotEqual(const IService::EPrivacyPermission& a, const IService::EPrivacyPermission& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::EPrivacyPermission));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{F914BF03-0C14-4407-ADCE-9709A835FABE}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pp', "PrivacyPermission");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{D9043432-D207-4456-A123-72A45B9913D1}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'pp', "PrivacyPermission");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{EF1EE94A-8416-478C-B891-E54A5308B9A8}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ppa', "A");
		pFunction->BindInput(2, 'ppb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{B8E019B3-8588-4649-9E24-C3A318B0C7CC}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ppa', "A");
		pFunction->BindInput(2, 'ppb', "B");
		scope.Register(pFunction);
	}
}

} // PrivacyPermission

namespace Relationship
{

Schematyc::CSharedString ToString(const IService::ERelationship& relationship)
{
	switch (relationship)
	{
	case IService::ERelationship::Default: return "Default";
	case IService::ERelationship::Friend: return "Friend";
	}

	return "Invalid";
}

IService::ERelationship FromString(const Schematyc::CSharedString& privacyPermission)
{
	if (stricmp(privacyPermission.c_str(), "Default") == 0)
	{
		return IService::ERelationship::Default;
	}
	
	return IService::ERelationship::Friend;
}

bool IsEqual(const IService::ERelationship& a, const IService::ERelationship& b)
{
	return a == b;
}

bool NotEqual(const IService::ERelationship& a, const IService::ERelationship& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::ERelationship));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{157004B4-3AD8-4715-841A-9EBF741B9FC4}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rel', "Relationship");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{87E6DAF3-2AB8-408E-8A72-0055FCBDFBA3}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rel', "Relationship");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{992DB9F9-4F68-42DE-8E3A-65C63E833B30}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rela', "A");
		pFunction->BindInput(2, 'relb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{58555859-9830-4567-92E1-44534431795F}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rela', "A");
		pFunction->BindInput(2, 'relb', "B");
		scope.Register(pFunction);
	}
}

} // Relationship

namespace RichPresence
{

SRichPresence Create(const CryFixedStringT<128>& activity, const CryFixedStringT<128>& headline, SRichPresence::ETimer countdownTimer, int64 seconds, uint32 partySize, uint32 partyMax)
{
	SRichPresence richPresence;

	richPresence.activity = activity;
	richPresence.headline = headline;
	richPresence.countdownTimer = countdownTimer;
	richPresence.seconds = seconds;
	richPresence.partySize = partySize;
	richPresence.partyMax = partyMax;

	return richPresence;
}

void Expand(const SRichPresence& richPresence, CryFixedStringT<128>& activity, CryFixedStringT<128>& headline, SRichPresence::ETimer& countdownTimer, int64& seconds, uint32& partySize, uint32& partyMax)
{
	activity = richPresence.activity;
	headline = richPresence.headline;
	countdownTimer = richPresence.countdownTimer;
	seconds = richPresence.seconds;
	partySize = richPresence.partySize;
	partyMax = richPresence.partyMax;
}

Schematyc::CSharedString ToString(const SRichPresence& richPresence)
{
	return richPresence.activity.c_str();
}

SRichPresence FromString(const Schematyc::CSharedString& activity)
{
	SRichPresence richPresence;
	richPresence.activity = activity.c_str();
	return richPresence;
}

bool IsEqual(const SRichPresence& a, const SRichPresence& b)
{
	return a == b;
}

bool NotEqual(const SRichPresence& a, const SRichPresence& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(SRichPresence));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{C270BB2D-739B-4D41-922A-1B6FF366796F}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rp', "RichPresence");
		pFunction->BindInput(1, 'act', "Activity");
		pFunction->BindInput(2, 'hl', "Headline");
		pFunction->BindInput(3, 'tt', "TimerType");
		pFunction->BindInput(4, 'sec', "Seconds");
		pFunction->BindInput(5, 'ps', "PartySize");
		pFunction->BindInput(6, 'pm', "PartyMax");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Expand, "{7DE7CAC3-E5E0-456E-9768-DB0717A7B920}"_cry_guid, "Expand");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindInput(1, 'rp', "RichPresence");
		pFunction->BindOutput(2, 'act', "Activity");
		pFunction->BindOutput(3, 'hl', "Headline");
		pFunction->BindOutput(4, 'tt', "TimerType");
		pFunction->BindOutput(5, 'sec', "Seconds");
		pFunction->BindOutput(6, 'ps', "PartySize");
		pFunction->BindOutput(7, 'pm', "PartyMax");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{0B86773A-2D08-4943-840F-0F5E581CFA1E}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rptt', "RichPresence");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{1141F3C9-3685-4909-92FA-2AD8844B6495}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rptt', "RichPresence");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{D03748E6-9180-4D52-83FA-536D1B226DCF}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rpa', "A");
		pFunction->BindInput(2, 'rpb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{9E9A3F4C-1A6B-47D2-B0C3-140F8266D87B}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rpa', "A");
		pFunction->BindInput(2, 'rpb', "B");
		scope.Register(pFunction);
	}
}

} // RichPresence

namespace RichPresenceTimerType
{

Schematyc::CSharedString ToString(SRichPresence::ETimer timerType)
{
	switch (timerType)
	{
	case SRichPresence::ETimer::Elapsed: return "Elapsed";
	case SRichPresence::ETimer::Remaining: return "Remaining";
	}

	return "None";
}

SRichPresence::ETimer FromString(const Schematyc::CSharedString& timerType)
{
	if (stricmp(timerType.c_str(), "Elapsed") == 0)
	{
		return SRichPresence::ETimer::Elapsed;
	}

	if (stricmp(timerType.c_str(), "Remaining") == 0)
	{
		return SRichPresence::ETimer::Remaining;
	}
	
	return SRichPresence::ETimer::None;
}

bool IsEqual(SRichPresence::ETimer a, SRichPresence::ETimer b)
{
	return a == b;
}

bool NotEqual(SRichPresence::ETimer a, SRichPresence::ETimer b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(SRichPresence::ETimer));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{F662946B-C41B-468E-9975-55A615C7E377}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rptt', "TimerType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{ECBDD2A2-6C74-4F06-A40E-10DBA93CE85D}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rptt', "TimerType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{608D5E8E-10F2-4389-801E-9A3BBE8CC6CC}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rpta', "A");
		pFunction->BindInput(2, 'rptb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{188A78C8-7B69-4DFF-9153-2999C44EEF4A}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rpta', "A");
		pFunction->BindInput(2, 'rptb', "B");
		scope.Register(pFunction);
	}
}

} // RichPresenceTimerType

namespace UserInformationFlags
{

CEnumFlags<EUserInformationFlags> AddFlags(const CEnumFlags<EUserInformationFlags>& flags, const CEnumFlags<EUserInformationFlags>& addFlags)
{
	CEnumFlags<EUserInformationFlags> result{ flags };
	result.Add(addFlags);
	return result;
}

CEnumFlags<EUserInformationFlags> RemoveFlags(const CEnumFlags<EUserInformationFlags>& flags, const CEnumFlags<EUserInformationFlags>& removeFlags)
{
	CEnumFlags<EUserInformationFlags> result{ flags };
	result.Remove(removeFlags);
	return result;
}

bool HasAnyFlags(const CEnumFlags<EUserInformationFlags>& flags, const CEnumFlags<EUserInformationFlags>& checkFlags)
{
	return flags.CheckAny(checkFlags);
}

bool HasAllFlags(const CEnumFlags<EUserInformationFlags>& flags, const CEnumFlags<EUserInformationFlags>& checkFlags)
{
	return flags.CheckAll(checkFlags);
}

Schematyc::CSharedString ToString(const CEnumFlags<EUserInformationFlags>& userInformationFlags)
{
	uint32 value = userInformationFlags.UnderlyingValue();
	string result;
	result.Format("%ul", value);
	return result;
}

CEnumFlags<EUserInformationFlags> FromString(const Schematyc::CSharedString& userDialog)
{
	uint32 flags = 0;
	sscanf_s(userDialog.c_str(), "%u", &flags);
	return CEnumFlags<EUserInformationFlags>(static_cast<EUserInformationFlags>(flags));
}

bool IsEqual(const CEnumFlags<EUserInformationFlags>& a, const CEnumFlags<EUserInformationFlags>& b)
{
	return a == b;
}

bool NotEqual(const CEnumFlags<EUserInformationFlags>& a, const CEnumFlags<EUserInformationFlags>& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CEnumFlags<EUserInformationFlags>));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&AddFlags, "{39D3F5C6-A820-4141-923B-029E40596AE2}"_cry_guid, "AddFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "AddFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RemoveFlags, "{DF2B82C4-12CD-4905-88D1-B820DEB72EC7}"_cry_guid, "RemoveFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "RemoveFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&HasAnyFlags, "{FC24EE89-D6BF-42A7-BC83-78FCCEC21CCE}"_cry_guid, "HasAnyFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "CheckFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&HasAllFlags, "{F604575D-22C3-432B-833B-311E17097375}"_cry_guid, "HasAllFlags");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "Flags");
		pFunction->BindInput(2, 'uifb', "CheckFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{6582204B-AC1E-49D6-B4A3-D46FAE4AF9FC}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uif', "UserInfoFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{6872AC35-59CB-4362-ABC8-42F7728618C2}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'uif', "UserInfoFlags");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{006D1FBA-2C1D-4F7A-AFD4-034318088B72}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "A");
		pFunction->BindInput(2, 'uifb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{9D09365A-6706-4E80-B706-03FFD9506FBB}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'uifa', "A");
		pFunction->BindInput(2, 'uifb', "B");
		scope.Register(pFunction);
	}
}

} // UserInformationFlags
} // Cry::GamePlatform
