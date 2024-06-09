// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/DynArray.h>
#include <Types/Service.h>

namespace Cry::GamePlatform
{

class COnCatalogReceived : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnCatalogReceived;

	COnCatalogReceived()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_success(false)
		, m_items()
		, m_remainingItemsInCatalog(false)
	{}

	COnCatalogReceived(EGamePlatform platform, bool success, const DynArray<IService::SStoreItem> items, bool remainingItemsInCatalog)
		: CBaseEventContext(platform, EventType)
		, m_success(success)
		, m_items(items)
		, m_remainingItemsInCatalog(remainingItemsInCatalog)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnCatalogReceived>& typeDesc)
	{
		typeDesc.SetGUID("{DCA899EF-4AA3-44CB-9641-96ABDBC92E0D}"_cry_guid);
		typeDesc.SetLabel("Catalog::OnCatalogReceived");
		typeDesc.AddMember(&COnCatalogReceived::m_success, 'succ', "success", "Success", nullptr, false);
		typeDesc.AddMember(&COnCatalogReceived::m_items, 'itms', "items", "Items", nullptr, DynArray<IService::SStoreItem>());
		typeDesc.AddMember(&COnCatalogReceived::m_remainingItemsInCatalog, 'riic', "hasremaining", "Has Remaining", nullptr, false);
	}

	bool IsSuccessful() const
	{
		return m_success;
	}

	const DynArray<IService::SStoreItem>& GetItems() const
	{
		return m_items;
	}

	bool HasRemaining() const
	{
		return m_remainingItemsInCatalog;
	}

private:
	bool                           m_success;
	DynArray<IService::SStoreItem> m_items;
	bool                           m_remainingItemsInCatalog;
};

} // Cry::GamePlatform