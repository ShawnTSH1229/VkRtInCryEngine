// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <Types/Lobby.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

namespace _LobbyIdentifier
{

LobbyIdentifier Create(EGamePlatform platform, const Schematyc::CSharedString& id)
{
	LobbyIdentifier lobbyIdentifier;

	if (IService* pService = Helper::GetService(platform))
	{
		pService->SetIdentifierFromString(lobbyIdentifier, id.c_str());
	}

	return lobbyIdentifier;
}

EGamePlatform GetPlatform(const LobbyIdentifier& lobbyId)
{
	return Helper::GetServiceEnum(lobbyId.Service());
}

Schematyc::CSharedString ToString(const LobbyIdentifier& lobbyId)
{
	stack_string result;
	lobbyId.GetAsString(result);
	return Schematyc::CSharedString(result.c_str());
}

bool IsEqual(const LobbyIdentifier& a, const LobbyIdentifier& b)
{
	return a == b;
}

bool NotEqual(const LobbyIdentifier& a, const LobbyIdentifier& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(LobbyIdentifier));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{723AD32B-891E-43F3-85EE-BEF4DE762AC5}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'loby', "Lobby");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInput(2, 'lbid', "Lobby Id");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{DFC6F790-3D23-417C-B1CC-0BDC2ECC9DFA}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'loby', "Lobby");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{0F83F7B7-074A-4023-BB46-C2A0DD08E0E3}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'loby', "Lobby");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{FAE62AC7-F11B-453A-8E9B-F8C44B969FB8}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'lbya', "A");
		pFunction->BindInput(2, 'lbyb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{BB2B5988-F3C1-423F-B6DB-570867F0C3AB}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'lbya', "A");
		pFunction->BindInput(2, 'lbyb', "B");
		scope.Register(pFunction);
	}
}

} // _LobbyIdentifier

namespace LobbyQueryComparator
{

Schematyc::CSharedString ToString(IUserLobby::EComparison comparator)
{
	switch (comparator)
	{
	case IUserLobby::EComparison::EqualToOrLess: return "EqualToOrLess";
	case IUserLobby::EComparison::LessThan: return "LessThan";
	case IUserLobby::EComparison::Equal: return "Equal";
	case IUserLobby::EComparison::GreaterThan: return "GreaterThan";
	case IUserLobby::EComparison::EqualToOrGreaterThan: return "EqualToOrGreaterThan";
	case IUserLobby::EComparison::NotEqual: return "NotEqual";
	}

	return "Invalid";
}

IUserLobby::EComparison FromString(const Schematyc::CSharedString& comparator)
{
	if (stricmp(comparator.c_str(), "EqualToOrLess") == 0)
	{
		return IUserLobby::EComparison::EqualToOrLess;
	}
	if (stricmp(comparator.c_str(), "LessThan") == 0)
	{
		return IUserLobby::EComparison::LessThan;
	}
	if (stricmp(comparator.c_str(), "Equal") == 0)
	{
		return IUserLobby::EComparison::Equal;
	}
	if (stricmp(comparator.c_str(), "GreaterThan") == 0)
	{
		return IUserLobby::EComparison::GreaterThan;
	}
	if (stricmp(comparator.c_str(), "EqualToOrGreaterThan") == 0)
	{
		return IUserLobby::EComparison::EqualToOrGreaterThan;
	}
	
	return IUserLobby::EComparison::NotEqual;
}

bool IsEqual(IUserLobby::EComparison a, IUserLobby::EComparison b)
{
	return a == b;
}

bool NotEqual(IUserLobby::EComparison a, IUserLobby::EComparison b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IUserLobby::EComparison));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{7E70E67E-1F3F-4E2F-9AC4-5F17C17C769B}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'styp', "ScoreType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{B63DED58-0F3B-4E01-9EB3-3911B8C083F1}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'styp', "ScoreType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{A75D2B94-3C34-4801-85EE-FE85B6B81708}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{6B1154B9-3E37-43AA-9841-461B5CCBA1D4}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
}

} // LobbyQueryComparator

namespace LobbyVisibility
{

Schematyc::CSharedString ToString(IUserLobby::EVisibility visibility)
{
	switch (visibility)
	{
	case IUserLobby::EVisibility::Private: return "Private";
	case IUserLobby::EVisibility::FriendsOnly: return "FriendsOnly";
	case IUserLobby::EVisibility::Public: return "Public";
	case IUserLobby::EVisibility::Invisible: return "Invisible";
	}

	return "Invalid";
}

IUserLobby::EVisibility FromString(const Schematyc::CSharedString& visibility)
{
	if (stricmp(visibility.c_str(), "Private") == 0)
	{
		return IUserLobby::EVisibility::Private;
	}
	if (stricmp(visibility.c_str(), "FriendsOnly") == 0)
	{
		return IUserLobby::EVisibility::FriendsOnly;
	}
	if (stricmp(visibility.c_str(), "Public") == 0)
	{
		return IUserLobby::EVisibility::Public;
	}
	
	return IUserLobby::EVisibility::Invisible;
}

bool IsEqual(IUserLobby::EVisibility a, IUserLobby::EVisibility b)
{
	return a == b;
}

bool NotEqual(IUserLobby::EVisibility a, IUserLobby::EVisibility b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IUserLobby::EVisibility));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{2FA497AA-DA82-4899-B118-6F2668ACEF2E}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'styp', "ScoreType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{4AB89F5D-E32C-429E-BBE7-CF654FB97B2B}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'styp', "ScoreType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{70DFB1CF-B325-405E-9C51-1AE4E1E289AA}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{71BA2719-3E50-4C25-AA66-C2E0DFF534E1}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
}

} // LobbyVisibility
} // Cry::GamePlatform