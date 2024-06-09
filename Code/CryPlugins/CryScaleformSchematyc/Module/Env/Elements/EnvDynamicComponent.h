// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

/*	EnvDynamicComponent

	This is a modified copy of the original EnvComponent implementation. It allows a user
	to define a dynamic type description (Varying guids, names etc)	at run-time.

	This is required as the default implementation is statically defined, including the
	type description itself through template magic.
*/
#pragma once

#include "../Utils/Utils.h"
#include <CrySchematyc/Env/Elements/EnvComponent.h>

namespace Cry::Scaleform
{

template <class TComponent>
class CEnvDynamicComponent : public Schematyc::CEnvComponent<TComponent>
{

public:

	CEnvDynamicComponent(CTypeNameWithString&& typeName, CEntityComponentClassDesc& desc, const Schematyc::SSourceFileInfo& sourceFileInfo)
		: Schematyc::CEnvComponent<TComponent>(sourceFileInfo)
		, m_typeName(std::move(typeName))
		, m_desc(desc)
	{
		CEnvDynamicComponent<TComponent>::CEnvElementBase::SetGUID(desc.GetGUID());
		CEnvDynamicComponent<TComponent>::CEnvElementBase::SetName(desc.GetLabel());
		CEnvDynamicComponent<TComponent>::CEnvElementBase::SetDescription(desc.GetDescription());
	}

	virtual ~CEnvDynamicComponent() override = default;
	
	virtual const CEntityComponentClassDesc& GetDesc() const override
	{
		return m_desc;
	}

protected:

	const CTypeNameWithString m_typeName;
	CEntityComponentClassDesc& m_desc;

};

namespace EnvDynamicComponent
{

constexpr uint64 GUIDHigher = 0x44594E434F4D5054;

template <class TComponent>
inline std::shared_ptr<CEnvDynamicComponent<TComponent>> MakeShared(CTypeNameWithString&& typeName, CEntityComponentClassDesc& desc, const Schematyc::SSourceFileInfo& sourceFileInfo)
{
	return std::make_shared<CEnvDynamicComponent<TComponent>>(std::move(typeName), desc, sourceFileInfo);
}

inline string TypeString(const char * name)
{
	string temp;
	temp.Format("DynamicComponent::%s", name);
	return temp;
}

} // EnvDynamicComponent

} // Cry::Scaleform