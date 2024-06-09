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

namespace Dialog
{

Schematyc::CSharedString ToString(EDialog dialog)
{
	switch (dialog)
	{
	case EDialog::Friends: return "Friends";
	case EDialog::Community: return "Community";
	case EDialog::Players: return "Players";
	case EDialog::Settings: return "Settings";
	case EDialog::Group: return "Group";
	case EDialog::Achievements: return "Achievements";
	case EDialog::Stats: return "Stats";
	}

	return "None";
}

EDialog FromString(const Schematyc::CSharedString& dialog)
{
	if (stricmp(dialog.c_str(), "Friends") == 0)
	{
		return EDialog::Friends;
	}

	if (stricmp(dialog.c_str(), "Community") == 0)
	{
		return EDialog::Community;
	}

	if (stricmp(dialog.c_str(), "Players") == 0)
	{
		return EDialog::Players;
	}

	if (stricmp(dialog.c_str(), "Settings") == 0)
	{
		return EDialog::Settings;
	}

	if (stricmp(dialog.c_str(), "Group") == 0)
	{
		return EDialog::Group;
	}

	if (stricmp(dialog.c_str(), "Achievements") == 0)
	{
		return EDialog::Achievements;
	}

	CRY_ASSERT(stricmp(dialog.c_str(), "Stats") == 0, "Unexpected value in FromString.");
	
	return EDialog::Stats;
}

bool IsEqual(EDialog a, EDialog b)
{
	return a == b;
}

bool NotEqual(EDialog a, EDialog b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(EDialog));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{0E95AFD4-5E23-46F9-9906-2D216A8ED426}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'dlg', "Dialog");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{EAEAFD5E-34E3-4801-8B6E-C27EA558F660}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'dlg', "Dialog");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{B48037F7-B1A6-4C1E-AD44-C5698062B6A6}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'dlga', "A");
		pFunction->BindInput(2, 'dlgb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{20BB1468-55E4-402B-B64A-064CD1501F7B}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'dlga', "A");
		pFunction->BindInput(2, 'dlgb', "B");
		scope.Register(pFunction);
	}
}

} // Dialog

namespace UserDialog
{

Schematyc::CSharedString ToString(EUserTargetedDialog userDialog)
{
	switch (userDialog)
	{
	case EUserTargetedDialog::UserInfo: return "UserInfo";
	case EUserTargetedDialog::FriendAdd: return "FriendAdd";
	case EUserTargetedDialog::FriendRemove: return "FriendRemove";
	case EUserTargetedDialog::FriendRequestAccept: return "FriendRequestAccept";
	case EUserTargetedDialog::FriendRequestIgnore: return "FriendRequestIgnore";
	case EUserTargetedDialog::Chat: return "Chat";
	case EUserTargetedDialog::JoinTrade: return "JoinTrade";
	case EUserTargetedDialog::Stats: return "Stats";
	case EUserTargetedDialog::Achievements: return "Achievements";
	}

	return "Invalid";
}

EUserTargetedDialog FromString(const Schematyc::CSharedString& userDialog)
{
	if (stricmp(userDialog.c_str(), "UserInfo") == 0)
	{
		return EUserTargetedDialog::UserInfo;
	}

	if (stricmp(userDialog.c_str(), "FriendAdd") == 0)
	{
		return EUserTargetedDialog::FriendAdd;
	}

	if (stricmp(userDialog.c_str(), "FriendRemove") == 0)
	{
		return EUserTargetedDialog::FriendRemove;
	}

	if (stricmp(userDialog.c_str(), "FriendRequestAccept") == 0)
	{
		return EUserTargetedDialog::FriendRequestAccept;
	}

	if (stricmp(userDialog.c_str(), "FriendRequestIgnore") == 0)
	{
		return EUserTargetedDialog::FriendRequestIgnore;
	}

	if (stricmp(userDialog.c_str(), "Chat") == 0)
	{
		return EUserTargetedDialog::Chat;
	}

	if (stricmp(userDialog.c_str(), "JoinTrade") == 0)
	{
		return EUserTargetedDialog::JoinTrade;
	}

	if (stricmp(userDialog.c_str(), "Stats") == 0)
	{
		return EUserTargetedDialog::Stats;
	}

	CRY_ASSERT(stricmp(userDialog.c_str(), "Achievements") == 0, "Unexpected value in FromString ('%s').", userDialog.c_str());
	
	return EUserTargetedDialog::Achievements;
}

bool IsEqual(EUserTargetedDialog a, EUserTargetedDialog b)
{
	return a == b;
}

bool NotEqual(EUserTargetedDialog a, EUserTargetedDialog b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(EUserTargetedDialog));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{265D2844-1F1B-49F0-8419-FDACE26D6120}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'udlg', "UserDialog");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{5F5A857B-DFF5-4826-B13E-3B7AFE28A2F9}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'udlg', "UserDialog");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{8D1D4D7E-16E9-41A3-9AB7-DBFC19779E99}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'udga', "A");
		pFunction->BindInput(2, 'udgb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{81C4DCCB-CD3F-40A8-8A52-66B212D28E1B}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'udga', "A");
		pFunction->BindInput(2, 'udgb', "B");
		scope.Register(pFunction);
	}
}

} // UserDialog
} // Cry::GamePlatform
