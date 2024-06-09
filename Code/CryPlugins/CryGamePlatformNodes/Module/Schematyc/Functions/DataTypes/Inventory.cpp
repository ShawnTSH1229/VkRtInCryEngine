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

namespace InventoryItem
{

Schematyc::CSharedString ToString(const IService::SInventoryItem& inventoryItem)
{
	string id;
	inventoryItem.id.GetAsString(id);
	return id.c_str();
}

void Expand(const IService::SInventoryItem& inventoryItem, InventoryItemIdentifier& itemId, ApplicationIdentifier& appId, uint32& quantity)
{
	itemId = inventoryItem.id;
	appId = inventoryItem.productId;
	quantity = inventoryItem.quantity;
}

bool IsEqual(const IService::SInventoryItem& a, const IService::SInventoryItem& b)
{
	return a.id == b.id;
}

bool NotEqual(const IService::SInventoryItem& a, const IService::SInventoryItem& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::SInventoryItem));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{A296E518-7846-4CC7-B02A-A1BE2B211CBE}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'iitm', "InventoryItem");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Expand, "{8224B1DC-EF5F-4156-875C-3BBDF135B2A9}"_cry_guid, "Expand");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindInput(1, 'iitm', "InventoryItem");
		pFunction->BindOutput(2, 'iid', "InventoryItemId");
		pFunction->BindOutput(3, 'aid', "ApplicationId");
		pFunction->BindOutput(4, 'qty', "Quantity");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{6BAEAD78-DADA-412A-AD4A-01F45E79B136}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sita', "A");
		pFunction->BindInput(2, 'sitb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{777ED2D1-F11A-469D-9C1B-69F8A9529D21}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sita', "A");
		pFunction->BindInput(2, 'sitb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<IService::SInventoryItem>, "{AFB7B6D3-2350-488D-9734-51207CFB6FDE}"_cry_guid, "ToArray");
		pFunction->BindInput(1, 'itms', "DynArray[Inventory Item]");
		pFunction->BindInputArrayType<IService::SInventoryItem>(2, 'siar', "Array[Inventory Item]");
		scope.Register(pFunction);
	}
}

} // InventoryItem

namespace _InventoryItemIdentifier
{

InventoryItemIdentifier Create(EGamePlatform platform, const Schematyc::CSharedString& id)
{
	InventoryItemIdentifier inventoryItemId;

	if (IService* pService = Helper::GetService(platform))
	{
		pService->SetIdentifierFromString(inventoryItemId, id.c_str());
	}

	return inventoryItemId;
}

EGamePlatform GetPlatform(const InventoryItemIdentifier& inventoryItemId)
{
	return Helper::GetServiceEnum(inventoryItemId.Service());
}

Schematyc::CSharedString ToString(const InventoryItemIdentifier& inventoryItemId)
{
	stack_string result;
	inventoryItemId.GetAsString(result);
	return Schematyc::CSharedString(result.c_str());
}

bool IsEqual(const InventoryItemIdentifier& a, const InventoryItemIdentifier& b)
{
	return a == b;
}

bool NotEqual(const InventoryItemIdentifier& a, const InventoryItemIdentifier& b)
{
	return !IsEqual(a,b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(InventoryItemIdentifier));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{F7A6A140-51A4-47F9-A290-3254FC79A933}"_cry_guid, "Create");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'iid', "InventoryItemId");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInput(2, 'apps', "Application Id");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPlatform, "{87EC0109-140F-47E9-8803-986BA280C278}"_cry_guid, "GetPlatform");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'plat', "Platform");
		pFunction->BindInput(1, 'iid', "InventoryItemId");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{11C7F670-4096-4374-B29E-03C13EBEA2B6}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'iid', "InventoryItemId");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{7D337892-A448-45C4-89B4-9AA40E208749}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'iida', "A");
		pFunction->BindInput(2, 'iidb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{5AB179E3-39C5-460E-B46B-A26BD3FE4690}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'iida', "A");
		pFunction->BindInput(2, 'iidb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<InventoryItemIdentifier>, "{48A5EC19-D0F7-4721-B7B4-5929ED30F39F}"_cry_guid, "ToArray");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindInput(1, 'itms', "DynArray[Inventory Item Identifier]");
		pFunction->BindInputArrayType<InventoryItemIdentifier>(2, 'iiia', "Array[Inventory Item Identifier]");
		scope.Register(pFunction);
	}
}

} // _InventoryItemIdentifier
} // Cry::GamePlatform
