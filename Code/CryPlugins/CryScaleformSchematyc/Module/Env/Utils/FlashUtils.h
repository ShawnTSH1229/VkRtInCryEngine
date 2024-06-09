// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "../EnvTypes.h"
#include "../FlashTypes.h"

namespace Cry::Scaleform
{
	
inline EFlashReturnType GetReturnTypeByGUID(const CryGUID& guid)
{
	if (guid == Schematyc::GetTypeGUID<float>())
	{
		return EFlashReturnType::Number;
	}
	else if (guid == Schematyc::GetTypeGUID<bool>())
	{
		return EFlashReturnType::Boolean;
	}
	else if (guid == Schematyc::GetTypeGUID<Schematyc::CSharedString>())
	{
		return EFlashReturnType::String;
	}

	return EFlashReturnType::Any;
}

inline EDynamicType GetDynTypeFromFlash(const SUIParameterDesc::EUIParameterType& paramType)
{
	switch (paramType)
	{
	case SUIParameterDesc::eUIPT_Bool:
		return EDynamicType::Boolean;

	case SUIParameterDesc::eUIPT_Float:
		return EDynamicType::Float;

	case SUIParameterDesc::eUIPT_Int:
		return EDynamicType::Integer;

	case SUIParameterDesc::eUIPT_String:
		return EDynamicType::String;

	default:
		return EDynamicType::Any;
	}
}

inline Schematyc::CAnyValuePtr ConvertDataToAny(const TUIData& data)
{
	if (data.GetType() == EUIDataTypes::eUIDT_Bool)
		return Schematyc::CAnyValue::MakeShared(*data.GetPtr<bool>());

	else if (data.GetType() == EUIDataTypes::eUIDT_Float)
		return Schematyc::CAnyValue::MakeShared(*data.GetPtr<float>());

	else if (data.GetType() == EUIDataTypes::eUIDT_String)
		return Schematyc::CAnyValue::MakeShared(Schematyc::CSharedString(data.GetPtr<string>()->c_str()));

	return Schematyc::CAnyValuePtr();
}

inline uint32 FindUnusedInstanceId(IUIElement* pElement)
{
	if (pElement == nullptr)
		return 0;

	std::vector<uint32> takenIds;
	takenIds.reserve(pElement->GetInstances()->GetCount());

	IUIElementIteratorPtr iter = pElement->GetInstances();
	while (IUIElement* pInstance = iter->Next())
	{
		takenIds.emplace_back(pInstance->GetInstanceID());
	}
		
	std::sort(takenIds.begin(), takenIds.end());
		
	uint32 newId = 0;
	bool found = false;
	for (auto& takenId : takenIds)
	{
		if (newId != takenId)
		{
			found = true;
			break;
		}
		++newId;
	}

	return newId;
}

inline bool InstanceExists(IUIElement* pElement, const uint32& instanceId)
{
	IUIElementIteratorPtr iter = pElement->GetInstances();
	while (IUIElement* pInstance = iter->Next())
	{
		if (pInstance->GetInstanceID() == instanceId)
			return true;
	}

	return false;
}

template <typename TSource, typename TTarget>
inline void WriteFlashParam(const TSource& source, void* target)
{
	new (target) TTarget(source);
}

template <typename TSource, typename TTarget>
inline void WriteFlashParam(const TUIData& source, void* target)
{
	TSource val;
	source.GetValueWithConversion(val);
	return WriteFlashParam<TSource, TTarget>(val, target);
}

template <typename TSourceAndTarget>
inline void WriteFlashParam(const TUIData& source, void* target)
{
	return WriteFlashParam<TSourceAndTarget, TSourceAndTarget>(source, target);
}

} // namespace Cry::Scaleform

#ifndef _RELEASE
	#define VERIFY_INSTANCE_EXISTS(element, id)																																		 \
		if (!InstanceExists(element, id))																													 \
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Trying to execute on a non-existent Element Instance. This can result in creating new instances unintentionally.")
#else
	#define VERIFY_INSTANCE_EXISTS(element, id)
#endif