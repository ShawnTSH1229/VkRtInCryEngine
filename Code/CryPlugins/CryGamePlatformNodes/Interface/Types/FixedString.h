// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySerialization/CryStrings.h>

namespace Schematyc::Helpers
{

template<template<size_t> class CryFixedStringT, size_t SIZE>
struct SIsString<CryFixedStringT<SIZE>>
{
	static const bool value = true;
};

namespace FixedString
{

template<size_t SIZE>
inline bool IsEqual(const CryFixedStringT<SIZE>& left, const CryFixedStringT<SIZE>& right)
{
	return left == right;
}

template<size_t SIZE>
inline uint32 LengthOf(const CryFixedStringT<SIZE>& s)
{
	return static_cast<uint32>(s.length());
}

template<size_t SIZE>
inline void ToString(const CryFixedStringT<SIZE>& in, Schematyc::CSharedString& out)
{
	out = in.c_str();
}

template<size_t SIZE>
inline void FromString(const Schematyc::CSharedString& in, CryFixedStringT<SIZE>& out)
{
	CRY_ASSERT(in.length() < SIZE, "String size is larger than the fixed string expected capacity. String will be truncated.");
	out.assign(in.c_str(), in.c_str() + min<uint64>(SIZE - 1, in.length()));
}

} // FixedString

} // Schematyc::Helpers

template<size_t SIZE>
inline void ReflectType(Schematyc::CTypeDesc<CryFixedStringT<SIZE>>& typeDesc)
{
	static CryGUID guid = "{B4979AD9-D5C5-4757-B18C-000000000000}"_cry_guid;
	guid.lopart += SIZE;

	static string label;
	label.Format("FixedString[%d]", SIZE - 1);

	typeDesc.SetGUID(guid);
	typeDesc.SetLabel(label);
	typeDesc.SetDefaultValue(CryFixedStringT<SIZE>(""));
}