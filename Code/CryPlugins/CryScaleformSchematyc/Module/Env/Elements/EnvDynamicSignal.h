// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

/*	EnvDynamicSignal

	This is a modified copy of the original EnvSignal implementation. It allows a user
	to define a dynamic type description for a signal (Varying outputs)	at run-time.

	This is required as the default implementation is statically defined, including the
	type description itself through template magic.

	Users of this signal, when sending require the generated type description for the
	dynamic signal they want to fire. This is because the user will need to create a 
	data buffer for the outputs of the signal that the system will then transmit.
	/See: Components\Flash\DynamicComponent.cpp: CFlashUIComponent::OnUIEvent
*/
#pragma once

#include "../EnvTypes.h"
#include "../Utils/Utils.h"
#include <CrySchematyc/Env/EnvElementBase.h>
#include <CrySchematyc/Env/Elements/IEnvSignal.h>

namespace Cry::Scaleform
{

// The only difference to the original CEnvSignal class is that we store the type description data rather than a reference to a static variable.
class CEnvDynamicSignal : public Schematyc::CEnvElementBase<Schematyc::IEnvSignal>
{

public:

	inline CEnvDynamicSignal(CTypeNameWithString&& typeName, const std::shared_ptr<Schematyc::CClassDesc>& desc, const Schematyc::SSourceFileInfo& sourceFileInfo)
		: CEnvElementBase(sourceFileInfo)
		, m_typeName(std::move(typeName))
		, m_desc(desc)
	{
		CEnvElementBase::SetGUID(desc->GetGUID());
		CEnvElementBase::SetName(desc->GetLabel());
		CEnvElementBase::SetDescription(desc->GetDescription());
	}

	// IEnvElement
	virtual bool IsValidScope(IEnvElement& scope) const override
	{
		switch (scope.GetType())
		{
		case Schematyc::EEnvElementType::Root:
		case Schematyc::EEnvElementType::Module:
		case Schematyc::EEnvElementType::Action:
		case Schematyc::EEnvElementType::Component:
		{
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	// ~IEnvElement

	// IEnvSignal
	virtual const Schematyc::CClassDesc& GetDesc() const override
	{
		return *m_desc;
	}
	// ~IEnvSignal

private:

	const CTypeNameWithString m_typeName;
	const std::shared_ptr<Schematyc::CClassDesc> m_desc;

};

namespace EnvDynamicSignal
{

constexpr uint64 GUIDHigher = 0x44594E5349474E4C;

inline std::shared_ptr<CEnvDynamicSignal> MakeShared(CTypeNameWithString&& typeName, const std::shared_ptr<Schematyc::CClassDesc>& typeDesc, const Schematyc::SSourceFileInfo& sourceFileInfo)
{
	Schematyc::SAnyValueAllocator<CEnvDynamicSignal> allocator(*typeDesc);
	return std::allocate_shared<CEnvDynamicSignal, Schematyc::SAnyValueAllocator<CEnvDynamicSignal>>(allocator, std::move(typeName), typeDesc, sourceFileInfo);
}

inline string TypeString(const char * name)
{
	string temp;
	temp.Format("DynamicSignal::%s", name);
	return temp;
}

} // EnvDynamicSignal

} // Cry::Scaleform