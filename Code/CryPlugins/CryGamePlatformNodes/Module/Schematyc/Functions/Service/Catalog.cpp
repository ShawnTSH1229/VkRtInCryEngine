// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <Types/DynArray.h>
#include <Types/Service.h>
#include <Schematyc/GamePlatformEnv.h>
#include <Schematyc/Utils.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform::Catalog
{

bool BrowseCatalog(IService::ECatalogSortOrder sortOrder)
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->BrowseCatalog(sortOrder);
	}

	return false;
}

bool QueryCatalogItemDetailsByItem(const IService::SStoreItem& item)
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->QueryCatalogItemDetails(item);
	}

	return false;
}

bool QueryCatalogItemDetailsByApplicationId(const ApplicationIdentifier& applicationId)
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->QueryCatalogItemDetails(applicationId);
	}

	return false;
}

bool QueryCatalogItemDetailsByItems(Schematyc::CAnyArrayPtr items)
{
	if (auto pService = Helper::GetMainService())
	{
		if (items && items->GetSize())
		{
			DynArray<IService::SStoreItem> dyn;
			for (int i = 0; i < items->GetSize(); i++)
			{
				dyn.push_back(Schematyc::DynamicCast<IService::SStoreItem>((*items)[i]));
			}
			return pService->QueryCatalogItemDetails(dyn);
		}
	}

	return false;
}

bool QueryCatalogItemDetailsByApplicationIds(Schematyc::CAnyArrayPtr productIds)
{
	if (auto pService = Helper::GetMainService())
	{
		if (productIds && productIds->GetSize())
		{
			DynArray<ApplicationIdentifier> dyn;
			for (int i = 0; i < productIds->GetSize(); i++)
			{
				dyn.push_back(Schematyc::DynamicCast<ApplicationIdentifier>((*productIds)[i]));
			}
			return pService->QueryCatalogItemDetails(dyn);
		}
	}

	return false;
}

bool GetLicenses(EGamePlatform platform, Schematyc::CAnyArrayPtr pLicenses)
{
	if (auto pService = Helper::GetService(platform))
	{
		DynArrayToAnyArray(pService->GetLicenses(), pLicenses);
		return true;
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{29EEE03C-C545-496E-BDBF-978E197C5A7F}"_cry_guid, "Catalog"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&BrowseCatalog, "{CAAABBBA-38A8-4D9F-B82E-C9C213794BBF}"_cry_guid, "BrowseCatalog");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'so', "SortOrder");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&QueryCatalogItemDetailsByItem, "{DB26A23F-17C6-453C-BF7B-30C16244A2DD}"_cry_guid, "QueryCatalogItemDetailsByItem");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'itm', "Item");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&QueryCatalogItemDetailsByApplicationId, "{2F8BD117-56AE-416B-8EBA-A22128B6A758}"_cry_guid, "QueryCatalogItemDetailsByAppId");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pid', "Product");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&QueryCatalogItemDetailsByItems, "{CF153651-076F-47D7-8B2D-8E7D2EC77459}"_cry_guid, "QueryCatalogItemDetailsByItems");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInputArrayType<IService::SStoreItem>(1, 'itms', "Array[StoreItem]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&QueryCatalogItemDetailsByApplicationIds, "{C56E502E-BAA3-4007-9586-75229E9649D5}"_cry_guid, "QueryCatalogItemDetailsByApplicationIds");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInputArrayType<ApplicationIdentifier>(1, 'pids', "Array[ApplicationId]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetLicenses, "{68E5E86C-1093-47AC-A689-F2A91D4FD80D}"_cry_guid, "GetLicenses");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInputArrayType<ApplicationIdentifier>(2, 'lics', "Array[ApplicationId]");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Catalog
