// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnPersonaStateChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPersonaStateChanged;

	COnPersonaStateChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
		, m_changeFlags()
	{}

	COnPersonaStateChanged(EGamePlatform platform, const AccountIdentifier& accountId, CEnumFlags<EPersonaChangeFlags> changeFlags)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
		, m_changeFlags(changeFlags)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPersonaStateChanged>& typeDesc)
	{
		typeDesc.SetGUID("{6854F376-290E-4F36-B788-C051D58057E5}"_cry_guid);
		typeDesc.SetLabel("Accounts::OnPersonaStateChanged");
		typeDesc.AddMember(&COnPersonaStateChanged::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnPersonaStateChanged::m_changeFlags, 'cflg', "changeflags", "Change Flags", nullptr, CEnumFlags<EPersonaChangeFlags>());
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	const CEnumFlags<EPersonaChangeFlags>& GetChangeFlags() const
	{
		return m_changeFlags;
	}

private:

	AccountIdentifier               m_accountId;
	CEnumFlags<EPersonaChangeFlags> m_changeFlags;

};

} // Cry::GamePlatform