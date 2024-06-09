// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform
{

class COnTextFiltered : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnTextFiltered;

	COnTextFiltered()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_filteredText("")
	{}

	COnTextFiltered(EGamePlatform platform, const char* szFilteredText)
		: CBaseEventContext(platform, EventType)
		, m_filteredText(szFilteredText)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnTextFiltered>& typeDesc)
	{
		typeDesc.SetGUID("{63B44939-2E3E-41F5-97D5-84A08B6F4C90}"_cry_guid);
		typeDesc.SetLabel("Networking::OnTextFiltered");
		typeDesc.AddMember(&COnTextFiltered::m_filteredText, 'ftxt', "filteredtext", "FilteredText", nullptr, "");
	}

	const Schematyc::CSharedString& GetFilteredText() const
	{
		return m_filteredText;
	}

private:

	Schematyc::CSharedString m_filteredText;

};

} // Cry::GamePlatform