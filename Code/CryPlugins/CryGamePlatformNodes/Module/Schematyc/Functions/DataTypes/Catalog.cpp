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

namespace CatalogSortOrder
{

Schematyc::CSharedString ToString(const IService::ECatalogSortOrder& sortOrder)
{
	switch (sortOrder)
	{
	case IService::ECatalogSortOrder::AllPurchaseCountDaily: return "AllPurchaseCountDaily";
	case IService::ECatalogSortOrder::DigitalReleaseDate: return "DigitalReleaseDate";
	case IService::ECatalogSortOrder::PaidPurchaseCountAllTime: return "PaidPurchaseCountAllTime";
	case IService::ECatalogSortOrder::PaidPurchaseCountDaily: return "PaidPurchaseCountDaily";
	case IService::ECatalogSortOrder::PhysicalReleaseDate: return "PhysicalReleaseDate";
	case IService::ECatalogSortOrder::UserRating: return "UserRating";
	}

	return "Invalid";
}

IService::ECatalogSortOrder FromString(const Schematyc::CSharedString& sortOrder)
{
	if (stricmp(sortOrder.c_str(), "AllPurchaseCountDaily") == 0)
	{
		return IService::ECatalogSortOrder::AllPurchaseCountDaily;
	}

	if (stricmp(sortOrder.c_str(), "DigitalReleaseDate") == 0)
	{
		return IService::ECatalogSortOrder::DigitalReleaseDate;
	}

	if (stricmp(sortOrder.c_str(), "PaidPurchaseCountAllTime") == 0)
	{
		return IService::ECatalogSortOrder::PaidPurchaseCountAllTime;
	}

	if (stricmp(sortOrder.c_str(), "PaidPurchaseCountDaily") == 0)
	{
		return IService::ECatalogSortOrder::PaidPurchaseCountDaily;
	}

	if (stricmp(sortOrder.c_str(), "PhysicalReleaseDate") == 0)
	{
		return IService::ECatalogSortOrder::PhysicalReleaseDate;
	}

	CRY_ASSERT(stricmp(sortOrder.c_str(), "UserRating") == 0, "Unexpected value in FromString.");
	
	return IService::ECatalogSortOrder::UserRating;
}

bool IsEqual(const IService::ECatalogSortOrder& a, const IService::ECatalogSortOrder& b)
{
	return a == b;
}

bool NotEqual(const IService::ECatalogSortOrder& a, const IService::ECatalogSortOrder& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::ECatalogSortOrder));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{C7BA97BF-FD92-401A-B1F1-09AA50ADF305}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sodr', "SortOrder");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{1AA42461-FE12-4DDD-8B19-065ED5EA372D}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'sodr', "SortOrder");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{D12D12D0-BA98-4012-89E0-6BB67434401C}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "A");
		pFunction->BindInput(2, 'sdrb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{FC6CF8D3-646C-445A-A302-C331D50BE860}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sdra', "A");
		pFunction->BindInput(2, 'sdrb', "B");
		scope.Register(pFunction);
	}
}

} // CatalogSortOrder

namespace ProductType
{

Schematyc::CSharedString ToString(const IService::EProductType& productType)
{
	switch (productType)
	{
	case IService::EProductType::Consumable: return "Consumable";
	case IService::EProductType::DLC: return "DLC";
	}

	return "Invalid";
}

IService::EProductType FromString(const Schematyc::CSharedString& productType)
{
	if (stricmp(productType.c_str(), "Consumable") == 0)
	{
		return IService::EProductType::Consumable;
	}

	CRY_ASSERT(stricmp(productType.c_str(), "DLC") == 0, "Unexpected value in FromString.");
	
	return IService::EProductType::DLC;
}

bool IsEqual(const IService::EProductType& a, const IService::EProductType& b)
{
	return a == b;
}

bool NotEqual(const IService::EProductType& a, const IService::EProductType& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::EProductType));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{67F6B6A9-D71F-4007-986E-1E425C015C76}"_cry_guid, "ToString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'ptyp', "ProductType");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{BFCBCB6D-0397-4096-BE56-FE73A69921F1}"_cry_guid, "FromString");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'ptyp', "ProductType");
		pFunction->BindInput(1, 'iput', "Input");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{9791778A-1F01-4CB3-B73B-DDC2C0354EBE}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pta', "A");
		pFunction->BindInput(2, 'ptb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{5158165C-1F97-43F3-AF17-4B1FC55269D0}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'pta', "A");
		pFunction->BindInput(2, 'ptb', "B");
		scope.Register(pFunction);
	}
}

} // ProductType

namespace StoreItem
{

	Schematyc::CSharedString ToString(const IService::SStoreItem& storeItem)
	{
		string id;
		storeItem.id.GetAsString(id);
		return id.c_str();
	}

	void Expand(const IService::SStoreItem& storeItem, ApplicationIdentifier& appId, Schematyc::CSharedString& name, IService::EProductType& productType)
	{
		appId = storeItem.id;
		name = storeItem.name;
		productType = storeItem.type;
	}

	bool IsEqual(const IService::SStoreItem& a, const IService::SStoreItem& b)
	{
		return a == b;
	}

	bool NotEqual(const IService::SStoreItem& a, const IService::SStoreItem& b)
	{
		return !IsEqual(a, b);
	}

	void RegisterElements(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

		auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::SStoreItem));
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{A81A2249-7DA8-4940-932E-81FB44DF3E54}"_cry_guid, "ToString");
			pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
			pFunction->BindOutput(0, 'rslt', "Result");
			pFunction->BindInput(1, 'sitm', "StoreItem");
			scope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Expand, "{AF8F30E6-71A1-43D5-9451-3BAD05D1A7C7}"_cry_guid, "Expand");
			pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
			pFunction->BindInput(1, 'sitm', "StoreItem");
			pFunction->BindOutput(2, 'aid', "ApplicationId");
			pFunction->BindOutput(3, 'name', "Name");
			pFunction->BindOutput(4, 'ptyp', "ProductType");
			scope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{28F6F6EC-3A59-4213-97DC-687CD2D0E485}"_cry_guid, "IsEqual");
			pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
			pFunction->BindOutput(0, 'rslt', "Result");
			pFunction->BindInput(1, 'sita', "A");
			pFunction->BindInput(2, 'sitb', "B");
			scope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{83365441-5F4C-4579-83D2-8DA12140A911}"_cry_guid, "NotEqual");
			pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
			pFunction->BindOutput(0, 'rslt', "Result");
			pFunction->BindInput(1, 'sita', "A");
			pFunction->BindInput(2, 'sitb', "B");
			scope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<IService::SStoreItem>, "{FF36E688-1D60-4635-934C-C0F902288650}"_cry_guid, "ToArray");
			pFunction->BindInput(1, 'itms', "DynArray[Store Item]");
			pFunction->BindInputArrayType<IService::SStoreItem>(2, 'siar', "Array[Store Item]");
			scope.Register(pFunction);
		}
	}

} // StoreItem

namespace StoreItemDetails
{

Schematyc::CSharedString ToString(const IService::SStoreItemDetails& storeItemDetails)
{
	string id;
	storeItemDetails.id.GetAsString(id);
	return id.c_str();
}

void Expand(const IService::SStoreItemDetails& storeItemDetails, 
	ApplicationIdentifier& appId, 
	Schematyc::CSharedString& name, 
	Schematyc::CSharedString& description, 
	bool& isPurchasable, 
	Schematyc::CSharedString& currencyCode, 
	double& price, 
	double& listPrice,
	Schematyc::CSharedString& displayPrice,
	Schematyc::CSharedString& displayListPrice,
	uint32& consumableQuantity
)
{
	appId = storeItemDetails.id;
	name = storeItemDetails.name;
	description = storeItemDetails.description;
	isPurchasable = storeItemDetails.isPurchasable;
	currencyCode = storeItemDetails.currencyCode;
	price = storeItemDetails.price;
	listPrice = storeItemDetails.listPrice;
	displayPrice = storeItemDetails.displayPrice;
	displayListPrice = storeItemDetails.displayListPrice;
	consumableQuantity = storeItemDetails.consumableQuantity;
}

bool IsEqual(const IService::SStoreItemDetails& a, const IService::SStoreItemDetails& b)
{
	return a == b;
}

bool NotEqual(const IService::SStoreItemDetails& a, const IService::SStoreItemDetails& b)
{
	return !IsEqual(a, b);
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope typeScope = registrar.Scope(g_gamePlatformDataTypesGUID);

	auto scope = typeScope.Register(SCHEMATYC_MAKE_ENV_DATA_TYPE(IService::SStoreItemDetails));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{BCA83620-939D-4C1D-BC47-7F1B21E57F84}"_cry_guid, "ToString");
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sitm', "StoreItem");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Expand, "{99093287-96AA-4499-B4BA-C121511F605E}"_cry_guid, "Expand");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindInput(1, 'sitm', "StoreItem");
		pFunction->BindOutput(2, 'aid', "ApplicationId");
		pFunction->BindOutput(3, 'name', "Name");
		pFunction->BindOutput(4, 'desc', "Description");
		pFunction->BindOutput(5, 'isp', "IsPurchasable");
		pFunction->BindOutput(6, 'cc', "CurrencyCode");
		pFunction->BindOutput(7, 'pri', "Price");
		pFunction->BindOutput(8, 'lpri', "ListPrice");
		pFunction->BindOutput(9, 'dpr', "DisplayPrice");
		pFunction->BindOutput(10, 'dlpr', "DisplayListPrice");
		pFunction->BindOutput(11, 'cq', "ConsumableQuantity");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEqual, "{28043784-26FF-4C58-8CCD-A3A66E4891E6}"_cry_guid, "IsEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sita', "A");
		pFunction->BindInput(2, 'sitb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{AB44E7DC-435F-44D7-90F1-6FDB8AA6453B}"_cry_guid, "NotEqual");
		pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
		pFunction->BindOutput(0, 'rslt', "Result");
		pFunction->BindInput(1, 'sita', "A");
		pFunction->BindInput(2, 'sitb', "B");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DynArrayToAnyArray<IService::SStoreItemDetails>, "{871A421E-377C-4312-BC40-A7BB4C9784D6}"_cry_guid, "ToArray");
		pFunction->BindInput(1, 'itms', "DynArray[Store Item Detail]");
		pFunction->BindInputArrayType<IService::SStoreItemDetails>(2, 'sida', "Array[Store Item Detail]");
		scope.Register(pFunction);
	}
}

} // StoreItemDetails
} // Cry::GamePlatform
