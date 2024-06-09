// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/Service.h>

namespace Cry::GamePlatform
{

class COnUserPermissionChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnUserPermissionChanged;

	COnUserPermissionChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
		, m_permission(IService::EPermission::Communication)
		, m_hasPermission(false)
	{}

	COnUserPermissionChanged(EGamePlatform platform, const AccountIdentifier& accountId, IService::EPermission permission, bool hasPermission)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
		, m_permission(permission)
		, m_hasPermission(hasPermission)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnUserPermissionChanged>& typeDesc)
	{
		typeDesc.SetGUID("{D77111FF-DF3F-4332-9BB9-20658B7D672F}"_cry_guid);
		typeDesc.SetLabel("Accounts::OnUserPermissionChanged");
		typeDesc.AddMember(&COnUserPermissionChanged::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnUserPermissionChanged::m_permission, 'perm', "permission", "Permission", nullptr, IService::EPermission::Communication);
		typeDesc.AddMember(&COnUserPermissionChanged::m_hasPermission, 'hprm', "haspermission", "Has Permission", nullptr, false);
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	IService::EPermission GetPermission() const
	{
		return m_permission;
	}

	bool HasPermission() const
	{
		return m_hasPermission;
	}

private:

	AccountIdentifier     m_accountId;
	IService::EPermission m_permission;
	bool                  m_hasPermission;

};

} // Cry::GamePlatform