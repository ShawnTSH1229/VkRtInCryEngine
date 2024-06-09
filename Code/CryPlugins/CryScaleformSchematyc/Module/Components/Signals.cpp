// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "Signals.h"
#include <Env/DynamicRegistry.h>
#include <Env/Elements/EnvDynamicSignal.h>
#include <Env/Utils/FlashUtils.h>

namespace Cry::Scaleform
{

// Only purpose is to expose the AddMember method of CClassDesc so we can populate the CTypeDesc manually
class CUncheckedClassDesc : public Schematyc::CClassDesc
{

public:

	CUncheckedClassDesc() : Schematyc::CClassDesc() {};

	template <typename MEMBER_TYPE>
	Schematyc::CClassMemberDesc& AddMember(ptrdiff_t offset, uint32 id, const char* szName, const char* szLabel, const char* szDescription, const MEMBER_TYPE& defaultValue)
	{
		return Schematyc::CClassDesc::AddMember(Schematyc::GetTypeDesc<MEMBER_TYPE>(), offset, id, szName, szLabel, szDescription, stl::make_unique<Schematyc::Utils::CDefaultValue<MEMBER_TYPE>>(MEMBER_TYPE(defaultValue)));
	}

};

void RegisterElementEvents(Schematyc::CEnvRegistrationScope& scope, IUIElement* pElement, const CryGUID& componentGuid)
{
	// Here we collect all the events so we can reserve a buffer based on the number of events
	uint32 descIndex = 0;
	while (const SUIEventDesc* pEventDesc = pElement->GetEventDesc(descIndex++))
	{
		// Get a unique Type Name for this event and calculate a GUID from it
		string name;
		name.Format("%s::%s", pElement->GetName(), pEventDesc->sName);
		auto typeName = CTypeNameWithString(std::move(name));
		const auto guid = typeName.ToGUID(EnvDynamicSignal::GUIDHigher);
		
		auto typeDesc = std::make_shared<CUncheckedClassDesc>();
		size_t accumulatedSize = 0;
		
		// InstanceId Parameter
		typeDesc->AddMember(static_cast<ptrdiff_t>(accumulatedSize), accumulatedSize, "iid", "Instance Id", "The Id of the element instance this signal was generated from.", (uint32)0);
		accumulatedSize += sizeof(uint32);

		// Dynamic parameters
		for (const auto param : pEventDesc->InputParams.Params)
		{
			switch (GetDynTypeFromFlash(param.eType))
			{
			case EDynamicType::Boolean:
				typeDesc->AddMember(static_cast<ptrdiff_t>(accumulatedSize), accumulatedSize, param.sName, param.sDisplayName, param.sDesc, false);
				accumulatedSize += sizeof(bool);
				break;
			case EDynamicType::Integer:
				typeDesc->AddMember(static_cast<ptrdiff_t>(accumulatedSize), accumulatedSize, param.sName, param.sDisplayName, param.sDesc, 0);
				accumulatedSize += sizeof(int);
				break;
			case EDynamicType::Float:
				typeDesc->AddMember(static_cast<ptrdiff_t>(accumulatedSize), accumulatedSize, param.sName, param.sDisplayName, param.sDesc, 0.f);
				accumulatedSize += sizeof(float);
				break;

			default:
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Flash Event Parameter type is not a supported Dynamic Type. Event: '%s'. Type: '%d'", param.sName, param.eType);
				[[fallthrough]];
			case EDynamicType::String:
				typeDesc->AddMember<Schematyc::CSharedString>(static_cast<ptrdiff_t>(accumulatedSize), accumulatedSize, param.sName, param.sDisplayName, param.sDesc, "");
				accumulatedSize += sizeof(Schematyc::CSharedString);
				break;
			}
		}

		typeDesc->SetSize(accumulatedSize);
		typeDesc->SetGUID(guid);
		typeDesc->SetName(typeName);
		typeDesc->SetLabel(pEventDesc->sDisplayName);
		typeDesc->SetDescription(pEventDesc->sDesc);

		// Store the type description so we can find it when we receive events
		auto& dynTypeRegistry = CDynamicTypeDescRegistry::GetInstance();
		dynTypeRegistry.RegisterTypeDesc(typeDesc);

		auto pEvent = std::make_shared<CEnvDynamicSignal>(std::move(typeName), typeDesc, SCHEMATYC_SOURCE_FILE_INFO);
		scope.Register(pEvent);
	}
}

} // Cry::Scaleform