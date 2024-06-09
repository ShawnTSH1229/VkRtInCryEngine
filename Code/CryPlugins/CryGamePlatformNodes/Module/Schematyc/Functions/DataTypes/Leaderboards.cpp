// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <Types/Leaderboard.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

namespace _LeaderboardIdentifier
{

LeaderboardIdentifier Create(EGamePlatform platform, const Schematyc::CSharedString& id)
{
		LeaderboardIdentifier leaderboardIdentifier;

	if (IService* pService = Helper::GetService(platform))
	{
		pService->SetIdentifierFromString(leaderboardIdentifier, id.c_str());
	}

	return leaderboardIdentifier;
}

EGamePlatform GetPlatform(const LeaderboardIdentifier& leaderboardId)
{
	return Helper::GetServiceEnum(leaderboardId.Service());
}

Schematyc::CSharedString ToString(const LeaderboardIdentifier& leaderboardId)
{
	stack_string result;
	leaderboardId.GetAsString(result);
	return Schematyc::CSharedString(result.c_str());
}

bool IsEqual(const LeaderboardIdentifier& a, const LeaderboardIdentifier& b)
{
	return a == b;
}

bool NotEqual(const LeaderboardIdentifier& a, const LeaderboardIdentifier& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(LeaderboardIdentifier));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{1FBEB99D-D1F1-4926-A049-351A28EBBC2A}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ldrb', "Leaderboard");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInput(2, 'lbid', "Leaderboard Id");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{B6AB13A3-F678-4565-A662-2849B41F0674}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'ldrb', "Leaderboard");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{45BFD120-887B-4AB9-B1CA-C3EAEEFAA690}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ldrb', "Leaderboard");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{1EB1D3E2-9D74-457F-8436-83FF8928341A}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ldba', "A");
		pFunction->BindInput(2, 'ldbb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{CDD6608F-623D-488A-8768-A2DBFB8F4251}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ldba', "A");
		pFunction->BindInput(2, 'ldbb', "B");
		scope.Register(pFunction);
	}
}

} // _LeaderboardIdentifier

namespace LeaderboardRequest
{

Schematyc::CSharedString ToString(ILeaderboards::ERequest request)
{
	switch (request)
	{
	case ILeaderboards::ERequest::Friends: return "Friends";
	case ILeaderboards::ERequest::Global: return "Global";
	case ILeaderboards::ERequest::GlobalRelativeToUser: return "GlobalRelativeToUser";
	case ILeaderboards::ERequest::Users: return "Users";
	}

	return "Invalid";
}

ILeaderboards::ERequest FromString(const Schematyc::CSharedString& request)
{
	if (stricmp(request.c_str(), "Friends") == 0)
	{
		return ILeaderboards::ERequest::Friends;
	}

	if (stricmp(request.c_str(), "Global") == 0)
	{
		return ILeaderboards::ERequest::Global;
	}

	if (stricmp(request.c_str(), "GlobalRelativeToUser") == 0)
	{
		return ILeaderboards::ERequest::GlobalRelativeToUser;
	}
	
	return ILeaderboards::ERequest::Users;
}

bool IsEqual(ILeaderboards::ERequest a, ILeaderboards::ERequest b)
{
	return a == b;
}

bool NotEqual(ILeaderboards::ERequest a, ILeaderboards::ERequest b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(ILeaderboards::ERequest));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{FB40BBC7-7E4E-45CF-A220-63673716372A}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'rtyp', "RequestType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{B5118504-48AB-4E4D-B325-4FE497890ED3}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rtyp', "RequestType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{E9CCD543-A9C9-466A-AEC0-0C3E5A177BAE}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{8A168A01-E71E-4633-9DA9-630886C2CEB5}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
}

} // LeaderboardRequest

namespace LeaderboardScoreType
{

Schematyc::CSharedString ToString(ILeaderboards::EScoreType scoreType)
{
	switch (scoreType)
	{
	case ILeaderboards::EScoreType::Time: return "Time";
	case ILeaderboards::EScoreType::Score: return "Score";
	}

	return "Unknown";
}

ILeaderboards::EScoreType FromString(const Schematyc::CSharedString& scoreType)
{
	if (stricmp(scoreType.c_str(), "Time") == 0)
	{
		return ILeaderboards::EScoreType::Time;
	}

	if (stricmp(scoreType.c_str(), "Score") == 0)
	{
		return ILeaderboards::EScoreType::Score;
	}
	
	return ILeaderboards::EScoreType::Unknown;
}

bool IsEqual(ILeaderboards::EScoreType a, ILeaderboards::EScoreType b)
{
	return a == b;
}

bool NotEqual(ILeaderboards::EScoreType a, ILeaderboards::EScoreType b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(ILeaderboards::EScoreType));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{2C1607F7-9AF0-4433-9E9E-510DC7F2CA37}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'styp', "ScoreType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{B03A741F-E4C3-404C-B4E5-D255C70CFB46}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'styp', "ScoreType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{E6849262-ACF1-461E-8D3C-A1E518774420}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{AE548BE2-6F5A-45B6-AE9F-E979BF88F184}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'acca', "A");
		pFunction->BindInput(2, 'accb', "B");
		scope.Register(pFunction);
	}
}

} // LeaderboardScoreType
} // Cry::GamePlatform
