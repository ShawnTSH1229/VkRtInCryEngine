// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

class COnAuthTokenReceived : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnAuthTokenReceived;

	COnAuthTokenReceived()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_success(false)
		, m_token()
	{}

	COnAuthTokenReceived(EGamePlatform platform, bool success, const string& token)
		: CBaseEventContext(platform, EventType)
		, m_success(success)
		, m_token(token)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnAuthTokenReceived>& typeDesc)
	{
		typeDesc.SetGUID("{C6924555-864C-4DF5-9E63-3A98C3976514}"_cry_guid);
		typeDesc.SetLabel("Networking::OnAuthTokenReceived");
		typeDesc.AddMember(&COnAuthTokenReceived::m_success, 'succ', "success", "Success", nullptr, false);
		typeDesc.AddMember(&COnAuthTokenReceived::m_token, 'tkn', "token", "Token", nullptr, "");
	}

	bool IsSuccessful() const
	{
		return m_success;
	}

	const Schematyc::CSharedString& GetToken() const
	{
		return m_token;
	}

private:

	bool                     m_success;
	Schematyc::CSharedString m_token;

};

} // Cry::GamePlatform