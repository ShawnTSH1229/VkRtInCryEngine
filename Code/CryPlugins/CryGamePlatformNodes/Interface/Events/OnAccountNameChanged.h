// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnAccountNameChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnAccountNameChanged;

	COnAccountNameChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
		, m_name()
	{}

	COnAccountNameChanged(EGamePlatform platform, const AccountIdentifier& accountId, const char* szName)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
		, m_name(szName)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnAccountNameChanged>& typeDesc)
	{
		typeDesc.SetGUID("{B755CF42-ED41-45AB-B381-AB9F38CEF17E}"_cry_guid);
		typeDesc.SetLabel("Accounts::OnAccountNameChanged");
		typeDesc.AddMember(&COnAccountNameChanged::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnAccountNameChanged::m_name, 'name', "accountname", "Account Name", nullptr, "");
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	const Schematyc::CSharedString& GetAccountName() const
	{
		return m_name;
	}

private:

	AccountIdentifier        m_accountId;
	Schematyc::CSharedString m_name;

};

} // Cry::GamePlatform