// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "DynamicRegistry.h"

namespace Cry::Scaleform
{

CDynamicTypeDescRegistry::~CDynamicTypeDescRegistry()
{
	m_registry.clear();
}

CDynamicTypeDescRegistry& CDynamicTypeDescRegistry::GetInstance()
{
	static CDynamicTypeDescRegistry registry;
	return registry;
}

void CDynamicTypeDescRegistry::RegisterTypeDesc(std::weak_ptr<Schematyc::CClassDesc> pTypeDesc)
{
	m_registry.emplace(pTypeDesc);
}

void CDynamicTypeDescRegistry::UnregisterTypeDesc(const CryGUID& typeGuid)
{
	auto it = std::find_if(m_registry.begin(), m_registry.end(), [&typeGuid](const std::shared_ptr<Schematyc::CClassDesc>& pTypeDesc) { return pTypeDesc->GetGUID() == typeGuid; });
	if (it != m_registry.end())
		m_registry.erase(it);
}

bool CDynamicTypeDescRegistry::IsTypeRegistered(const CryGUID& typeGuid) const
{
	auto it = std::find_if(m_registry.begin(), m_registry.end(), [&typeGuid](const std::shared_ptr<Schematyc::CClassDesc>& pTypeDesc) { return pTypeDesc->GetGUID() == typeGuid; });
	return it != m_registry.end();
}

const std::shared_ptr<Schematyc::CClassDesc>& CDynamicTypeDescRegistry::GetTypeDesc(const CryGUID& typeGuid) const
{
	auto itType = std::find_if(m_registry.begin(), m_registry.end(), [&typeGuid](const std::shared_ptr<Schematyc::CClassDesc>& pTypeDesc) { return pTypeDesc->GetGUID() == typeGuid; });
	if (itType != m_registry.end())
	{
		return *itType;
	}
	else
	{
		static std::shared_ptr<Schematyc::CClassDesc> nullDesc;
		return nullDesc;
	}
}

void CDynamicTypeDescRegistry::Reset()
{
	m_registry.clear();
}

} // Cry::Scaleform