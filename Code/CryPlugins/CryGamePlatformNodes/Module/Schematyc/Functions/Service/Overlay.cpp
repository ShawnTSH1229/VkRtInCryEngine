// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <GamePlatformHelper.h>
#include <Types/DynArray.h>
#include <Schematyc/Utils.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>

#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform::Overlay
{

bool OpenDialogWithTargetUser(EUserTargetedDialog dialog, const AccountIdentifier& otherAccountId)
{
	if (auto pService = Helper::GetService(otherAccountId.Service()))
	{
		return pService->OpenDialogWithTargetUser(dialog, otherAccountId);
	}

	return false;
}

bool OpenDialog(EDialog dialog)
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->OpenDialog(dialog);
	}

	return false;
}

bool OpenBrowser(const Schematyc::CSharedString& address)
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->OpenBrowser(address.c_str());
	}

	return false;
}

bool CanOpenPurchaseOverlay()
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->CanOpenPurchaseOverlay();
	}

	return false;
}

bool OpenPurchaseOverlay(const ApplicationIdentifier& productId)
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->OpenPurchaseOverlay(productId);
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{973B43D6-2F63-4B3A-9159-6357A2B4288B}"_cry_guid, "Overlay"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&OpenDialogWithTargetUser, "{96F9BF72-9808-4924-9A30-7E731D108463}"_cry_guid, "OpenDialogWithTargetUser");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'dlg', "UserDialog");
		pFunction->BindInput(2, 'acc', "TargetAccountId");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&OpenDialog, "{0789F539-5872-4DCF-B217-DF40843650B8}"_cry_guid, "OpenDialog");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'dlg', "Dialog");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&OpenBrowser, "{5DF470F3-856A-42CF-9394-D4966A26E2F8}"_cry_guid, "OpenBrowser");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'addr', "WebAddress");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CanOpenPurchaseOverlay, "{A8D9F380-758F-48C9-BEFB-69AD5E90AEC4}"_cry_guid, "CanOpenPurchaseOverlay");
		pFunction->BindOutput(0, 'succ', "CanOpen");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&OpenPurchaseOverlay, "{97F1261A-E309-4D83-BE9E-597750F58F64}"_cry_guid, "OpenPurchaseOverlay");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pid', "ProductAppId");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Overlay
