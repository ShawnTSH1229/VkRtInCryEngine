// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/RemoteStorage.h>

namespace Cry::GamePlatform
{

class COnContentCreated : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnContentCreated;

	COnContentCreated()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_userGeneratedContent()
		, m_bRequireLegalAgreementAccept(false)
	{}

	COnContentCreated(EGamePlatform platform, const CUserGeneratedContent& userGeneratedContent, bool bRequireLegalAgreementAccept)
		: CBaseEventContext(platform, EventType)
		, m_userGeneratedContent(userGeneratedContent)
		, m_bRequireLegalAgreementAccept(bRequireLegalAgreementAccept)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnContentCreated>& typeDesc)
	{
		typeDesc.SetGUID("{563C0333-C878-472E-97DC-806BC392060A}"_cry_guid);
		typeDesc.SetLabel("RemoteStorage::OnContentCreated");
		typeDesc.AddMember(&COnContentCreated::m_userGeneratedContent, 'ugc', "usergeneratedcontent", "UGC Content", nullptr, CUserGeneratedContent());
		typeDesc.AddMember(&COnContentCreated::m_bRequireLegalAgreementAccept, 'rla', "sharedfile", "Requires Legal Agreement", nullptr, false);
	}

	const CUserGeneratedContent& GetUserGeneratedContent() const
	{
		return m_userGeneratedContent;
	}

	bool IsLegalRequired() const
	{
		return m_bRequireLegalAgreementAccept;
	}

private:

	CUserGeneratedContent m_userGeneratedContent;
	bool                  m_bRequireLegalAgreementAccept;

};

} // Cry::GamePlatform