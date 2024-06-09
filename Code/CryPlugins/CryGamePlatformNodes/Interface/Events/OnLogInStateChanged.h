// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnLogInStateChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLogInStateChanged;

	COnLogInStateChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
		, m_isLoggedIn(false)
	{}

	COnLogInStateChanged(EGamePlatform platform, const AccountIdentifier& accountId, bool isLoggedIn)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
		, m_isLoggedIn(isLoggedIn)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLogInStateChanged>& typeDesc)
	{
		typeDesc.SetGUID("{2068B8D6-DEDA-4831-978F-538E54015938}"_cry_guid);
		typeDesc.SetLabel("Networking::OnLogInStateChanged");
		typeDesc.AddMember(&COnLogInStateChanged::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnLogInStateChanged::m_isLoggedIn, 'islg', "isloggedin", "Is Logged-In", nullptr, false);
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	bool IsLoggedIn() const
	{
		return m_isLoggedIn;
	}

private:

	AccountIdentifier m_accountId;
	bool              m_isLoggedIn;

};

} // Cry::GamePlatform