// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/IPlatformAccount.h>

namespace Cry::GamePlatform
{

class COnPlatformServiceError : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnPlatformServiceError;

	COnPlatformServiceError()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_accountId()
		, m_errorCode(0)
	{}

	COnPlatformServiceError(EGamePlatform platform, const AccountIdentifier& accountId, int32 errorCode)
		: CBaseEventContext(platform, EventType)
		, m_accountId(accountId)
		, m_errorCode(errorCode)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnPlatformServiceError>& typeDesc)
	{
		typeDesc.SetGUID("{DCB1D337-6879-4ABB-A1A2-09F8D3937C49}"_cry_guid);
		typeDesc.SetLabel("Service::OnPlatformServiceError");
		typeDesc.AddMember(&COnPlatformServiceError::m_accountId, 'aid', "account", "Account", nullptr, AccountIdentifier());
		typeDesc.AddMember(&COnPlatformServiceError::m_errorCode, 'errc', "errorcode", "Error Code", nullptr, 0);
	}

	const AccountIdentifier& GetAccountId() const
	{
		return m_accountId;
	}

	int32 GetErrorCode() const
	{
		return m_errorCode;
	}

private:

	AccountIdentifier m_accountId;
	int32             m_errorCode;

};

} // Cry::GamePlatform