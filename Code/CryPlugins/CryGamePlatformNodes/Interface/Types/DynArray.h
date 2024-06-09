// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryCore/Containers/CryArray.h>
#include <CrySchematyc/Reflection/TypeDesc.h>

// Reflect 'DynArray' type.

namespace Schematyc
{

SCHEMATYC_DECLARE_ARRAY_TYPE(DynArray)

}

namespace DynArrayOperators
{
template<typename ELEMENT_TYPE>
inline static uint32 Size(const DynArray<ELEMENT_TYPE>& arr)
{
	return static_cast<uint32>(arr.size());
}

template<typename ELEMENT_TYPE>
inline static void PushBack(DynArray<ELEMENT_TYPE>& arr, const ELEMENT_TYPE& value)
{
	arr.push_back(value);
}

template<typename ELEMENT_TYPE>
inline static ELEMENT_TYPE& At(DynArray<ELEMENT_TYPE>& arr, uint32 pos)
{
	return arr.at(pos);
}

template<typename ELEMENT_TYPE>
inline static const ELEMENT_TYPE& At(const DynArray<ELEMENT_TYPE>& arr, uint32 pos)
{
	return arr.at(pos);
}
}

template<typename ELEMENT_TYPE>
inline void ReflectType(Schematyc::CTypeDesc<DynArray<ELEMENT_TYPE>>& desc)
{
	static const size_t hash = Schematyc::GetTypeName<DynArray<ELEMENT_TYPE>>().GetHash();

	static string label;
	label.Format("Array[%s]", Schematyc::GetTypeDesc<ELEMENT_TYPE>().GetLabel());

	desc.SetLabel(label);
	desc.SetGUID(CryGUID::Construct('dyna', hash));
	desc.template SetArraySizeOperator<&DynArrayOperators::Size<ELEMENT_TYPE>>();
	desc.template SetArrayAtOperator<&DynArrayOperators::At<ELEMENT_TYPE>>();
	desc.template SetArrayAtConstOperator<&DynArrayOperators::At<ELEMENT_TYPE>>();
	desc.template SetArrayPushBackOperator<&DynArrayOperators::PushBack<ELEMENT_TYPE>>();
}

template<typename T>
inline bool operator==(const DynArray<T>& left, const DynArray<T>& right)
{
	if (left.size() != right.size())
	{
		return false;
	}

	for (int i = 0; i < left.size(); i++)
	{
		if (!(left[i] == right[i]))
		{
			return false;
		}
	}

	return true;
}