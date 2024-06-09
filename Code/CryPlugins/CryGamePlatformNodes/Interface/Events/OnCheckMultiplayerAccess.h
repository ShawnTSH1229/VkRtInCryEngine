// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnCheckMultiplayerAccess : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnCheckMultiplayerAccess;

	COnCheckMultiplayerAccess()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_bHasMultiplayerAccess(false)
	{}

	COnCheckMultiplayerAccess(EGamePlatform platform, bool hasMultiplayerAccess)
		: CBaseEventContext(platform, EventType)
		, m_bHasMultiplayerAccess(hasMultiplayerAccess)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnCheckMultiplayerAccess>& typeDesc)
	{
		typeDesc.SetGUID("{4D09C0BD-1434-4A53-80C6-34D5A72AF9DB}"_cry_guid);
		typeDesc.SetLabel("Networking::OnCheckMultiplayerAccess");
		typeDesc.AddMember(&COnCheckMultiplayerAccess::m_bHasMultiplayerAccess, 'hmp', "hasaccess", "HasAccess", nullptr, false);
	}

	bool HasAccess() const
	{
		return m_bHasMultiplayerAccess;
	}

private:

	bool m_bHasMultiplayerAccess;

};

} // Cry::GamePlatform