// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/DynArray.h>
#include <Types/Service.h>

namespace Cry::GamePlatform
{

class COnCatalogItemsReceived : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnCatalogItemsReceived;

	COnCatalogItemsReceived()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_success(false)
		, m_items()
	{}

	COnCatalogItemsReceived(EGamePlatform platform, bool success, const DynArray<IService::SStoreItemDetails>& items)
		: CBaseEventContext(platform, EventType)
		, m_success(success)
		, m_items(items)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnCatalogItemsReceived>& typeDesc)
	{
		typeDesc.SetGUID("{E89F65E7-5D55-40BB-8FD9-1B2C3EEB4B87}"_cry_guid);
		typeDesc.SetLabel("Catalog::OnCatalogItemsReceived");
		typeDesc.AddMember(&COnCatalogItemsReceived::m_success, 'succ', "success", "Success", "Catalog was successfuly retrieved", false);
		typeDesc.AddMember(&COnCatalogItemsReceived::m_items, 'itms', "items", "Items", "Array of items in the retrieved catalog", DynArray<IService::SStoreItemDetails>());
	}

	bool IsSuccessful() const
	{
		return m_success;
	}

	const DynArray<IService::SStoreItemDetails>& GetItems() const
	{
		return m_items;
	}

private:

	bool m_success;
	DynArray<IService::SStoreItemDetails> m_items;

};

} // Cry::GamePlatform