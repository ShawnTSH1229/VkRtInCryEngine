// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/DynArray.h>
#include <Types/Service.h>

namespace Cry::GamePlatform
{

class COnLicensesChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLicensesChanged;

	COnLicensesChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_newLicenses()
	{}

	COnLicensesChanged(EGamePlatform platform, const DynArray<InventoryItemIdentifier>& newLicenses)
		: CBaseEventContext(platform, EventType)
		, m_newLicenses(newLicenses)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLicensesChanged>& typeDesc)
	{
		typeDesc.SetGUID("{C760B618-555F-4881-A517-0F6657A4B087}"_cry_guid);
		typeDesc.SetLabel("Catalog::OnLicensesChanged");
		typeDesc.AddMember(&COnLicensesChanged::m_newLicenses, 'lics', "licenses", "Licenses", nullptr, DynArray<InventoryItemIdentifier>());
	}

	const DynArray<InventoryItemIdentifier>& GetLicenses() const
	{
		return m_newLicenses;
	}

private:

	DynArray<InventoryItemIdentifier> m_newLicenses;

};

} // Cry::GamePlatform