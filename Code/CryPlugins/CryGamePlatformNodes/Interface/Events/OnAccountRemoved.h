// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnAccountRemoved : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnAccountRemoved;

	COnAccountRemoved()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
	{}

	COnAccountRemoved(EGamePlatform platform, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnAccountRemoved>& typeDesc)
	{
		typeDesc.SetGUID("{49AA8130-0941-4358-91E0-CA63722FEBCF}"_cry_guid);
		typeDesc.SetLabel("Accounts::OnAccountRemoved");
		typeDesc.AddMember(&COnAccountRemoved::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

private:

	AccountIdentifier m_accountId;

};

} // Cry::GamePlatform