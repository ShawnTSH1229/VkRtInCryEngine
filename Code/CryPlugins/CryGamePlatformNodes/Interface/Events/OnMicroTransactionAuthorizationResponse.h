// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CryGamePlatform/Interface/PlatformTypes.h>

namespace Cry::GamePlatform
{

class COnMicroTransactionAuthorizationResponse : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnMicroTransactionAuthorizationResponse;

	COnMicroTransactionAuthorizationResponse()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_orderId()
		, m_authorized(false)
	{}

	COnMicroTransactionAuthorizationResponse(EGamePlatform platform, const TransactionIdentifier& orderId, bool authorized)
		: CBaseEventContext(platform, EventType)
		, m_orderId(orderId)
		, m_authorized(authorized)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnMicroTransactionAuthorizationResponse>& typeDesc)
	{
		typeDesc.SetGUID("{7221900C-F196-48BE-8942-3A15885EF245}"_cry_guid);
		typeDesc.SetLabel("Catalog::OnMicroTransactionAuthorization");
		typeDesc.AddMember(&COnMicroTransactionAuthorizationResponse::m_orderId, 'oid', "orderid", "Order Id", nullptr, TransactionIdentifier());
		typeDesc.AddMember(&COnMicroTransactionAuthorizationResponse::m_authorized, 'auth', "authorized", "Authorized", nullptr, false);
	}

	const TransactionIdentifier& GetOrderId() const
	{
		return m_orderId;
	}

	bool IsAuthorized() const
	{
		return m_authorized;
	}

private:

	TransactionIdentifier m_orderId;
	bool                  m_authorized;

};

} // Cry::GamePlatform