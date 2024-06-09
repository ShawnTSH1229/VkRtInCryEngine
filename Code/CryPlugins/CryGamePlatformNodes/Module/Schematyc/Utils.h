// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/AnyArray.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

// TypeName wrapper used if the the type string needs storage
class CTypeNameWrapper : public Schematyc::CTypeName
{
public:
	inline CTypeNameWrapper(string&& typeString) :
		CTypeName(typeString.c_str()),
		m_typeString(std::move(typeString))
	{
	}

	inline const CryGUID ToGUID(uint64 higher) const
	{
		return CryGUID::Construct(higher, GetHash());
	}

private:
	const string m_typeString;
};

template<class TYPE>
inline void DynArrayToAnyArray(const DynArray<TYPE>& dynArray, Schematyc::CAnyArrayPtr pArray)
{
	if (pArray)
	{
		pArray->Clear();
		if (!dynArray.empty())
		{
			const auto& elemTypeDesc = Schematyc::GetTypeDesc<TYPE>();
			for (auto& elem : dynArray)
			{
				pArray->PushBack(Schematyc::CAnyConstRef(elemTypeDesc, &elem));
			}
		}
	}
}

template<typename T>
bool Equals(const T& lhs, const T& rhs)
{
	return lhs == rhs;
}

template<typename T>
bool IdentifierValueToString(const T& identifier, Schematyc::CSharedString& out)
{
	return identifier.GetAsString(out);
}

} // Cry::GamePlatform
