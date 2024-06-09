// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnAccountAdded : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnAccountAdded;

	COnAccountAdded()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
	{}

	COnAccountAdded(EGamePlatform platform, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnAccountAdded>& typeDesc)
	{
		typeDesc.SetGUID("{2190480D-E7F3-4A3D-9D33-C66196673830}"_cry_guid);
		typeDesc.SetLabel("Accounts::OnAccountAdded");
		typeDesc.AddMember(&COnAccountAdded::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

private:

	AccountIdentifier m_accountId;

};

} // Cry::GamePlatform