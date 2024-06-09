// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryCore/CryVariant.h>
#include <CrySchematyc/Reflection/TypeDesc.h>

namespace Cry::GamePlatform
{

enum class EStatDataType
{
	None,

	Integer,
	Float
};

inline void ReflectType(Schematyc::CTypeDesc<EStatDataType>& typeDesc)
{
	typeDesc.SetGUID("{0091640C-32D8-41F4-B280-D841A6FCA369}"_cry_guid);
	typeDesc.SetLabel("StatisticDataType");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(EStatDataType::None);
	typeDesc.AddConstant(EStatDataType::None, "None", "None");
	typeDesc.AddConstant(EStatDataType::Float, "Float", "Float");
	typeDesc.AddConstant(EStatDataType::Integer, "Integer", "Integer");
}

class CStatisticValue
{

public:

	CStatisticValue()
		: m_value(0.f)
	{
	}

	CStatisticValue(float value)
		: m_value(value)
	{
	}

	CStatisticValue(int32 value)
		: m_value(value)
	{
	}

	bool GetValue(float& value) const
	{
		if (stl::holds_alternative<float>(m_value))
		{
			value = stl::get<float>(m_value);
			return true;
		}
		
		return false;
	}

	bool GetValue(int32& value) const
	{
		if (stl::holds_alternative<int32>(m_value))
		{
			value = stl::get<int32>(m_value);
			return true;
		}

		return false;
	}

	EStatDataType GetType() const
	{
		if (stl::holds_alternative<float>(m_value))
			return EStatDataType::Float;
		else if (stl::holds_alternative<int32>(m_value))
			return EStatDataType::Integer;

		return EStatDataType::None;
	}

	void operator=(float value) { m_value = value; }
	void operator=(int32 value) { m_value = value; }
	bool operator==(const CStatisticValue& other) const { return m_value == other.m_value; }

	static void ReflectType(Schematyc::CTypeDesc<CStatisticValue>& typeDesc)
	{
		typeDesc.SetGUID("{CF286E62-91E3-4F06-9658-7DD205EF7ED9}"_cry_guid);
		typeDesc.SetLabel("Statistic Value");
	}

private:

	CryVariant<float, int32> m_value;

};

} // Cry::GamePlatform