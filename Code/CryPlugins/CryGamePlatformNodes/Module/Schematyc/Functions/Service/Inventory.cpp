// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <GamePlatformHelper.h>
#include <Types/Service.h>
#include <Schematyc/Utils.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>

#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform::Inventory
{

bool QueryInventory()
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->QueryInventory();
	}

	return false;
}

bool ConsumeInventoryItem(const InventoryItemIdentifier& id, uint32 quantity)
{
	if (auto pService = Helper::GetService(id.Service()))
	{
		return pService->ConsumeInventoryItem(id, quantity);
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{BF155D57-5546-496F-BD0A-A1C8FA290A77}"_cry_guid, "Inventory"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ConsumeInventoryItem, "{05B33487-F71A-4A51-8CC1-4BE2108ECDA5}"_cry_guid, "ConsumeInventoryItem");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'iitm', "Inventory Item");
		pFunction->BindInput(2, 'qty', "Quantity");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&QueryInventory, "{9BA111F8-C523-4CD2-B64B-DA8E96214902}"_cry_guid, "QueryInventory");
		pFunction->BindOutput(0, 'succ', "Success");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Inventory
