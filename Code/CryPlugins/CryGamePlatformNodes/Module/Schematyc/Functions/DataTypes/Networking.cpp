// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <GamePlatformHelper.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

namespace ConnectionStatus
{

Schematyc::CSharedString ToString(EConnectionStatus connectionStatus)
{
	switch (connectionStatus)
	{
	case EConnectionStatus::Connected: return "Connected";
	case EConnectionStatus::Connecting: return "Connecting";
	case EConnectionStatus::Disconnected: return "Disconnected";
	case EConnectionStatus::ObtainingIP: return "ObtainingIP";
	}

	return "Invalid";
}

EConnectionStatus FromString(const Schematyc::CSharedString& connectionStatus)
{
	if (stricmp(connectionStatus.c_str(), "Connected") == 0)
	{
		return EConnectionStatus::Connected;
	}

	if (stricmp(connectionStatus.c_str(), "Connecting") == 0)
	{
		return EConnectionStatus::Connecting;
	}

	if (stricmp(connectionStatus.c_str(), "Disconnected") == 0)
	{
		return EConnectionStatus::Disconnected;
	}

	CRY_ASSERT(stricmp(connectionStatus.c_str(), "ObtainingIP") == 0, "Unexpected value in FromString.");
	
	return EConnectionStatus::ObtainingIP;
}

bool IsEqual(EConnectionStatus a, EConnectionStatus b)
{
	return a == b;
}

bool NotEqual(EConnectionStatus a, EConnectionStatus b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(EConnectionStatus));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{B36BA40D-4199-4C10-AFE7-99721FA0B202}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ptyp', "ConnectionStatus");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{115B59B8-300C-41B7-8AFC-4A94617D1AEE}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ptyp', "ConnectionStatus");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{05B45CFA-3C07-4F04-A138-BB88E62ADE2E}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pta', "A");
		pFunction->BindInput(2, 'ptb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{0B5A75B7-F6E9-4BB6-91C5-682417AD8757}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pta', "A");
		pFunction->BindInput(2, 'ptb', "B");
		scope.Register(pFunction);
	}
}

} // ConnectionStatus
} // Cry::GamePlatform
