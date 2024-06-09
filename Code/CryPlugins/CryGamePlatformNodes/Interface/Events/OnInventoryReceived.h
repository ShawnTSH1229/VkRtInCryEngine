// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/DynArray.h>
#include <Types/Service.h>

namespace Cry::GamePlatform
{

class COnInventoryReceived : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnInventoryReceived;

	COnInventoryReceived()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_success(false)
		, m_items()
	{}

	COnInventoryReceived(EGamePlatform platform, bool success, const DynArray<IService::SInventoryItem>& items)
		: CBaseEventContext(platform, EventType)
		, m_success(success)
		, m_items(items)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnInventoryReceived>& typeDesc)
	{
		typeDesc.SetGUID("{9BFEC8BF-4A0C-4B8F-9281-90D31FD877B8}"_cry_guid);
		typeDesc.SetLabel("Inventory::OnInventoryReceived");
		typeDesc.AddMember(&COnInventoryReceived::m_success, 'succ', "success", "Success", nullptr, false);
		typeDesc.AddMember(&COnInventoryReceived::m_items, 'itms', "items", "Inventory Items", nullptr, DynArray<IService::SInventoryItem>());
	}

	bool IsSuccessful() const
	{
		return m_success;
	}

	const DynArray<IService::SInventoryItem>& GetItems() const
	{
		return m_items;
	}

private:

	bool m_success;
	DynArray<IService::SInventoryItem> m_items;

};

} // Cry::GamePlatform