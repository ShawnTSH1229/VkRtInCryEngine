// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Types/RemoteStorage.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformUserGeneratedContent.h>

namespace Cry::GamePlatform
{

namespace _UserGeneratedContent
{

bool GetIdentifier(const CUserGeneratedContent& ugc, IUserGeneratedContent::Identifier& identifier)
{
	if (auto pUserGeneratedContent = ugc.GetPtr())
	{
		identifier = pUserGeneratedContent->GetIdentifier();
		return true;
	}
	return false;
}

bool IsEqual(const CUserGeneratedContent& a, const CUserGeneratedContent& b)
{
	return a == b;
}

bool NotEqual(const CUserGeneratedContent& a, const CUserGeneratedContent& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	Schematyc::CEnvRegistrationScope scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(CUserGeneratedContent));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetIdentifier, "{70656485-2C7D-4B2B-84E1-D2F4236F8C98}"_cry_guid, "GetIdentifier");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'ugc', "UserGeneratedContent");
		pFunction->BindOutput(2, 'id', "Identifier");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{BDC332E9-35FE-4D26-AB28-CB97B6E03957}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugca', "A");
		pFunction->BindInput(2, 'ugcb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{1CC1BADE-EA72-4DB5-9949-1245E7A33D32}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugca', "A");
		pFunction->BindInput(2, 'ugcb', "B");
		scope.Register(pFunction);
	}
}

} // _UserGeneratedContent

namespace UserGeneratedContentType
{

Schematyc::CSharedString ToString(const IUserGeneratedContent::EType& ugcType)
{
	switch (ugcType)
	{
	case IUserGeneratedContent::EType::Community: return "Community";
	case IUserGeneratedContent::EType::Microtransaction: return "Microtransaction";
	case IUserGeneratedContent::EType::Collection: return "Collection";
	case IUserGeneratedContent::EType::Art: return "Art";
	case IUserGeneratedContent::EType::Video: return "Video";
	case IUserGeneratedContent::EType::Screenshot: return "Screenshot";
	case IUserGeneratedContent::EType::Game: return "Game";
	case IUserGeneratedContent::EType::Software: return "Software";
	case IUserGeneratedContent::EType::Concept: return "Concept";
	case IUserGeneratedContent::EType::WebGuide: return "WebGuide";
	case IUserGeneratedContent::EType::IntegratedGuide: return "IntegratedGuide";
	case IUserGeneratedContent::EType::Merch: return "Merch";
	case IUserGeneratedContent::EType::ControllerBinding: return "ControllerBinding";
	case IUserGeneratedContent::EType::SteamworksAccessInvite: return "SteamworksAccessInvite";
	case IUserGeneratedContent::EType::SteamVideo: return "SteamVideo";
	}

	return "Invalid";
}

IUserGeneratedContent::EType FromString(const Schematyc::CSharedString& ugcType)
{
	if (stricmp(ugcType.c_str(), "Community") == 0)
	{
		return IUserGeneratedContent::EType::Community;
	}

	if (stricmp(ugcType.c_str(), "Microtransaction") == 0)
	{
		return IUserGeneratedContent::EType::Microtransaction;
	}

	if (stricmp(ugcType.c_str(), "Collection") == 0)
	{
		return IUserGeneratedContent::EType::Collection;
	}

	if (stricmp(ugcType.c_str(), "Art") == 0)
	{
		return IUserGeneratedContent::EType::Art;
	}

	if (stricmp(ugcType.c_str(), "Video") == 0)
	{
		return IUserGeneratedContent::EType::Video;
	}

	if (stricmp(ugcType.c_str(), "Screenshot") == 0)
	{
		return IUserGeneratedContent::EType::Screenshot;
	}

	if (stricmp(ugcType.c_str(), "Game") == 0)
	{
		return IUserGeneratedContent::EType::Game;
	}

	if (stricmp(ugcType.c_str(), "Software") == 0)
	{
		return IUserGeneratedContent::EType::Software;
	}

	if (stricmp(ugcType.c_str(), "Concept") == 0)
	{
		return IUserGeneratedContent::EType::Concept;
	}

	if (stricmp(ugcType.c_str(), "WebGuide") == 0)
	{
		return IUserGeneratedContent::EType::WebGuide;
	}

	if (stricmp(ugcType.c_str(), "IntegratedGuide") == 0)
	{
		return IUserGeneratedContent::EType::IntegratedGuide;
	}

	if (stricmp(ugcType.c_str(), "Merch") == 0)
	{
		return IUserGeneratedContent::EType::Merch;
	}

	if (stricmp(ugcType.c_str(), "ControllerBinding") == 0)
	{
		return IUserGeneratedContent::EType::ControllerBinding;
	}

	if (stricmp(ugcType.c_str(), "SteamworksAccessInvite") == 0)
	{
		return IUserGeneratedContent::EType::SteamworksAccessInvite;
	}

	CRY_ASSERT(stricmp(ugcType.c_str(), "SteamVideo") == 0, "Unexpected value in FromString.");
	
	return IUserGeneratedContent::EType::SteamVideo;
}

bool IsEqual(const IUserGeneratedContent::EType& a, const IUserGeneratedContent::EType& b)
{
	return a == b;
}

bool NotEqual(const IUserGeneratedContent::EType& a, const IUserGeneratedContent::EType& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IUserGeneratedContent::EType));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{D0063AF1-81C5-484F-8213-C22CF83DD432}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugct', "UGCType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{21A3287A-1FD4-436B-89B7-7CFEF565A352}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ugct', "UGCType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{0890B963-01EF-4E17-B5DB-25B0D2685DB0}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugca', "A");
		pFunction->BindInput(2, 'ugcb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{69C5AD5D-8EBE-4172-B602-761BB0698BF2}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugca', "A");
		pFunction->BindInput(2, 'ugcb', "B");
		scope.Register(pFunction);
	}
}

} // UserGeneratedContentType

namespace UserGeneratedContentVisibility
{

Schematyc::CSharedString ToString(const IUserGeneratedContent::EVisibility& ugcVisibility)
{
	switch (ugcVisibility)
	{
	case IUserGeneratedContent::EVisibility::Public: return "Public";
	case IUserGeneratedContent::EVisibility::FriendsOnly: return "FriendsOnly";
	case IUserGeneratedContent::EVisibility::Private: return "Private";
	}

	return "Invalid";
}

IUserGeneratedContent::EVisibility FromString(const Schematyc::CSharedString& ugcVisibility)
{
	if (stricmp(ugcVisibility.c_str(), "Public") == 0)
	{
		return IUserGeneratedContent::EVisibility::Public;
	}

	if (stricmp(ugcVisibility.c_str(), "FriendsOnly") == 0)
	{
		return IUserGeneratedContent::EVisibility::FriendsOnly;
	}

	CRY_ASSERT(stricmp(ugcVisibility.c_str(), "Private") == 0, "Unexpected value in FromString.");
	
	return IUserGeneratedContent::EVisibility::Private;
}

bool IsEqual(const IUserGeneratedContent::EVisibility& a, const IUserGeneratedContent::EVisibility& b)
{
	return a == b;
}

bool NotEqual(const IUserGeneratedContent::EVisibility& a, const IUserGeneratedContent::EVisibility& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IUserGeneratedContent::EVisibility));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{1C49F1DC-B52D-4B28-A771-8398524827F4}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugcv', "UGCVisibility");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{DBB2DA66-005E-4D15-A080-FC8523FB2BDD}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ugcv', "UGCVisibility");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{4DEA92A1-D53D-4268-86DA-0B667A60C704}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugva', "A");
		pFunction->BindInput(2, 'ugvb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{B03F1CBC-8A36-4E49-BB95-FD5D1CEEFDD1}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ugva', "A");
		pFunction->BindInput(2, 'ugvb', "B");
		scope.Register(pFunction);
	}
}

} // UserGeneratedContentVisibility
} // Cry::GamePlatform
