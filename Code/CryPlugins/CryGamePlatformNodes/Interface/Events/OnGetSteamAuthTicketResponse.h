// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"

namespace Cry::GamePlatform
{

class COnGetSteamAuthTicketResponse : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnGetSteamAuthTicketResponse;

	COnGetSteamAuthTicketResponse()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_success(false)
		, m_authTicket()
	{}

	COnGetSteamAuthTicketResponse(EGamePlatform platform, bool success, uint32 authTicket)
		: CBaseEventContext(platform, EventType)
		, m_success(success)
		, m_authTicket(authTicket)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnGetSteamAuthTicketResponse>& typeDesc)
	{
		typeDesc.SetGUID("{7D6A1FA6-D2F0-41D2-A319-75A2F2997B37}"_cry_guid);
		typeDesc.SetLabel("Networking::OnGetSteamAuthTicketResponse");
		typeDesc.AddMember(&COnGetSteamAuthTicketResponse::m_success, 'succ', "success", "Success", nullptr, false);
		typeDesc.AddMember(&COnGetSteamAuthTicketResponse::m_authTicket, 'atkt', "authticket", "Ticket", nullptr, 0u);
	}

	bool IsSuccessful() const
	{
		return m_success;
	}

	uint32 GetTicket() const
	{
		return m_authTicket;
	}

private:

	bool   m_success;
	uint32 m_authTicket;

};

} // Cry::GamePlatform