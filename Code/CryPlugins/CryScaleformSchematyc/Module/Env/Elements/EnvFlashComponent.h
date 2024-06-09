// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

/*	EnvFlashComponent

	The same as EnvDynamicComponent, except the constructor and 
	destructor is specialized to make use of registering the 
	component with the Flash Registry.
*/
#pragma once
#include "EnvDynamicComponent.h"

namespace Cry::Scaleform
{

// This facilitates the reloading of the flash env package at runtime.
// Since we are using dynamic class descriptions we need to re-use these
// descriptions even after the env component was destroyed. We have no 
// way of knowing if any of these descriptions are still in use in user 
// entities, so we keep all unique GUIDs here so Schematyc can still 
// reference them.
class CDynDescFactory
{

	CDynDescFactory() = default;

public:

	~CDynDescFactory() = default;

	static CDynDescFactory& GetFactory()
	{
		static CDynDescFactory pFactory;
		return pFactory;
	}

	CEntityComponentClassDesc* GetOrCreateDesc(const CryGUID& guid)
	{
		auto desc = m_descriptions.find(guid);
		if (desc == m_descriptions.end())
		{
			auto pairb = m_descriptions.emplace(guid, std::make_unique<CEntityComponentClassDesc>());
			if (pairb.second)
			{
				return &*(*(pairb.first)).second;
			}
		}
		else
		{
			return (*desc).second.get();
		}

		return nullptr;
	}

private:

	std::map<CryGUID, std::unique_ptr<CEntityComponentClassDesc>> m_descriptions;

};

template <class TComponent>
class CEnvFlashComponent : public CEnvDynamicComponent<TComponent>
{

public:

	inline CEnvFlashComponent(CTypeNameWithString&& typeName, CEntityComponentClassDesc& desc, const Schematyc::SSourceFileInfo& sourceFileInfo) :
		CEnvDynamicComponent<TComponent>(std::move(typeName), desc, sourceFileInfo)
	{
		auto& pComponentRegistry = CFlashRegistry::GetInstance();
		pComponentRegistry.RegisterComponent(desc.GetGUID());
	}

	virtual ~CEnvFlashComponent() override
	{
		auto& pComponentRegistry = CFlashRegistry::GetInstance();
		pComponentRegistry.UnregisterComponent(CEnvDynamicComponent<TComponent>::m_desc.GetGUID());
	}

	virtual const CEntityComponentClassDesc& GetDesc() const override
	{
		return CEnvDynamicComponent<TComponent>::m_desc;
	}

};

namespace EnvFlashComponent
{

constexpr uint64 GUIDHigher = 0x464C41434F4D5054;

template <class TComponent>
inline std::shared_ptr<CEnvFlashComponent<TComponent>> MakeShared(CTypeNameWithString&& typeName, CEntityComponentClassDesc& desc, const Schematyc::SSourceFileInfo& sourceFileInfo)
{
	return std::make_shared<CEnvFlashComponent<TComponent>>(std::move(typeName), desc, sourceFileInfo);
}

inline string TypeString(const char * name)
{
	string temp;
	temp.Format("FlashComponent::%s", name);
	return temp;
}

} // EnvFlashComponent

} // Cry::Scaleform