// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/PlatformTypes.h>

namespace Cry::GamePlatform
{

class COnInventoryItemConsumed : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnInventoryItemConsumed;

	COnInventoryItemConsumed()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_success(false)
		, m_identifier()
		, m_quantity(0)
	{}

	COnInventoryItemConsumed(EGamePlatform platform, bool success, const InventoryItemIdentifier& identifier, uint32 quantity)
		: CBaseEventContext(platform, EventType)
		, m_success(success)
		, m_identifier(identifier)
		, m_quantity(quantity)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnInventoryItemConsumed>& typeDesc)
	{
		typeDesc.SetGUID("{E14CCCED-736D-4D94-A259-691B90040E55}"_cry_guid);
		typeDesc.SetLabel("Inventory::OnInventoryItemConsumed");
		typeDesc.AddMember(&COnInventoryItemConsumed::m_success, 'succ', "success", "Success", nullptr, false);
		typeDesc.AddMember(&COnInventoryItemConsumed::m_identifier, 'item', "inventoryitem", "Inventory Item", nullptr, InventoryItemIdentifier());
		typeDesc.AddMember(&COnInventoryItemConsumed::m_quantity, 'qty', "quantity", "Quantity", nullptr, 0u);
	}

	bool IsSuccessful() const
	{
		return m_success;
	}

	const InventoryItemIdentifier& GetIdentifier() const
	{
		return m_identifier;
	}

	uint32 GetQuantity() const
	{
		return m_quantity;
	}

private:

	bool                    m_success;
	InventoryItemIdentifier m_identifier;
	uint32                  m_quantity;

};

} // Cry::GamePlatform