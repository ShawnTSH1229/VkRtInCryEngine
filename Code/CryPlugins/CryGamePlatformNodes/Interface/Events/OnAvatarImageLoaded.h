// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnAvatarImageLoaded : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnAvatarImageLoaded;

	COnAvatarImageLoaded()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
	{}

	COnAvatarImageLoaded(EGamePlatform platform, const AccountIdentifier& accountId)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnAvatarImageLoaded>& typeDesc)
	{
		typeDesc.SetGUID("{BDF45820-0992-4E71-BAAB-FE51E9C09CFC}"_cry_guid);
		typeDesc.SetLabel("Accounts::OnAvatarImageLoaded");
		typeDesc.AddMember(&COnAvatarImageLoaded::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

private:

	AccountIdentifier m_accountId;

};

} // Cry::GamePlatform