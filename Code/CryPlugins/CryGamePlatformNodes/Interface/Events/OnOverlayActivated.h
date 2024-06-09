// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"

namespace Cry::GamePlatform
{

class COnOverlayActivated : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnOverlayActivated;

	COnOverlayActivated()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_active(false)
	{}

	COnOverlayActivated(EGamePlatform platform, bool active)
		: CBaseEventContext(platform, EventType)
		, m_active(active)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnOverlayActivated>& typeDesc)
	{
		typeDesc.SetGUID("{6E6DB53C-9119-43E9-A4CF-41F1D1E59A4C}"_cry_guid);
		typeDesc.SetLabel("Service::OnOverlayActivated");
		typeDesc.AddMember(&COnOverlayActivated::m_active, 'actv', "active", "Active", nullptr, false);
	}

	bool IsActive() const
	{
		return m_active;
	}

private:

	bool m_active;

};

} // Cry::GamePlatform