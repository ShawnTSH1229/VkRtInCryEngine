// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "../EnvTypes.h"
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::Scaleform
{

// CTypeName with storage for the source string
class CTypeNameWithString : public Schematyc::CTypeName
{

public:

	inline CTypeNameWithString(string&& typeString)
		: Schematyc::CTypeName(typeString.c_str())
		, m_typeString(std::move(typeString))
	{}

	inline const CryGUID ToGUID(uint64 higher) const
	{
		return CryGUID::Construct(higher, GetHash());
	}

private:

	const string m_typeString;

};

inline EDynamicType GetDynTypeByGUID(const CryGUID& guid)
{
	if (guid == Schematyc::GetTypeGUID<bool>())
	{
		return EDynamicType::Boolean;
	}
	else if (guid == Schematyc::GetTypeGUID<float>())
	{
		return EDynamicType::Float;
	}
	else if (guid == Schematyc::GetTypeGUID<int32>())
	{
		return EDynamicType::Integer;
	}
	else if (guid == Schematyc::GetTypeGUID<Schematyc::CSharedString>())
	{
		return EDynamicType::String;
	}

	return EDynamicType::Any;
}

inline CryGUID GetGUIDFromDynType(const EDynamicType& type)
{
	switch (type)
	{
	case EDynamicType::Boolean:
		return Schematyc::GetTypeGUID<bool>();

	case EDynamicType::Float:
		return Schematyc::GetTypeGUID<float>();

	case EDynamicType::Integer:
		return Schematyc::GetTypeGUID<int32>();

	case EDynamicType::String:
		return Schematyc::GetTypeGUID<Schematyc::CSharedString>();
	}

	return CryGUID::Null();
}

inline Schematyc::CAnyValuePtr GetDefaultValueByType(const EDynamicType& type)
{
	switch (type)
	{
	case EDynamicType::Boolean:
		return Schematyc::CAnyValue::MakeShared(false);

	case EDynamicType::Float:
		return Schematyc::CAnyValue::MakeShared<float>(0.f);

	case EDynamicType::Integer:
		return Schematyc::CAnyValue::MakeShared<int32>(0);

	case EDynamicType::String:
		return Schematyc::CAnyValue::MakeShared(Schematyc::CSharedString(""));
	}

	return Schematyc::CAnyValuePtr();
}

} // Cry::Scaleform