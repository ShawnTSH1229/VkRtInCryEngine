// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include <CryMath/Random.h>

#include <CrySchematyc/CoreAPI.h>
#include <CryCore/StaticInstanceList.h>

namespace Schematyc
{
namespace Bool
{
bool Flip(bool x)
{
	return !x;
}

bool Or(bool a, bool b)
{
	return a || b;
}

bool And(bool a, bool b)
{
	return a && b;
}

bool XOR(bool a, bool b)
{
	return a != b;
}

bool Equal(bool a, bool b)
{
	return a == b;
}

bool NotEqual(bool a, bool b)
{
	return a != b;
}

int32 ToInt32(bool bValue)
{
	return bValue ? 1 : 0;
}

uint32 ToUInt32(bool bValue)
{
	return bValue ? 1 : 0;
}

int64 ToInt64(bool bValue)
{
	return bValue ? 1 : 0;
}

uint64 ToUInt64(bool bValue)
{
	return bValue ? 1 : 0;
}

bool FromString(const CSharedString& input, bool& bResult)
{
	if (input.length() > 0)
	{
		// False
		if (input.c_str()[0] == '0')
		{
			bResult = false;
			return true;
		}

		if (stricmp(input.c_str(), "false") == 0)
		{
			bResult = false;
			return true;
		}

		// True
		if (input.c_str()[0] == '1')
		{
			bResult = true;
			return true;
		}

		if (stricmp(input.c_str(), "true") == 0)
		{
			bResult = true;
			return true;
		}
	}

	return false;
}

void ToString(bool bValue, CSharedString& result)
{
	ToString(result, bValue);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<bool>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Flip, "6a1d0e15-c4ad-4c87-ab38-3739020dd708"_cry_guid, "Flip");
		pFunction->SetDescription("Flip boolean value");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Or, "ff6f2ed1-942b-4726-9f13-b9e711cd28df"_cry_guid, "Or");
		pFunction->SetDescription("Logical OR operation");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&And, "87d62e0c-d32f-4ea7-aa5c-22aeeb1c9cd5"_cry_guid, "And");
		pFunction->SetDescription("Logical AND operation");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&XOR, "14a312fc-0885-4ab7-8bef-f124588e4c45"_cry_guid, "XOR");
		pFunction->SetDescription("Logical XOR operation");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Equal, "{ACA31DD3-AFC3-4EB1-AF27-27C70D25B1A6}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks whether A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{C40AFA65-409B-4C69-922E-88F119DE18F4}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks whether A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt32, "F93DB38C-E4A1-4553-84D2-255A1E47AC8A"_cry_guid, "ToInt32");
		pFunction->SetDescription("Convert boolean to int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt32, "FE42F543-AA16-47E8-A1EA-D4374C01B4A3"_cry_guid, "ToUInt32");
		pFunction->SetDescription("Convert boolean to uint32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{4FBC6134-FCD8-4C5A-87FF-0504A60D0A22}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to boolean");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "9a6f082e-ba3c-4510-9248-8bf8235d5e03"_cry_guid, "ToString");
		pFunction->SetDescription("Convert boolean to string");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
}
} // Bool

namespace Generic
{
	template<typename T>
	T Add(T a, T b)
	{
		return a + b;
	}

	template<typename T>
	T Subtract(T a, T b)
	{
		return a - b;
	}

	template<typename T>
	T Multiply(T a, T b)
	{
		return a * b;
	}

	template<typename T>
	T Divide(T a, T b)
	{
		return b != 0 ? a / b : 0;
	}

	template<typename T>
	T Modulus(T a, T b)
	{
		return b != 0 ? a % b : 0;
	}

	template<typename T>
	T Random(T min, T max)
	{
		return min < max ? cry_random(min, max) : min;
	}

	template<typename T>
	T Clamp(T value, T min, T max)
	{
		return crymath::clamp(value, min, max);
	}

	template<typename T>
	T Abs(T x)
	{
		return abs(x);
	}

	template<typename T>
	T Negate(T x)
	{
		return -x;
	}

	template<typename T>
	int Sign(T x)
	{
		return static_cast<int>(sgn(x));
	}

	template<typename T>
	bool Equal(T a, T b)
	{
		return a == b;
	}

	template<typename T>
	bool NotEqual(T a, T b)
	{
		return a != b;
	}

	template<typename T>
	bool LessThan(T a, T b)
	{
		return a < b;
	}

	template<typename T>
	bool LessThanOrEqual(T a, T b)
	{
		return a <= b;
	}

	template<typename T>
	bool GreaterThan(T a, T b)
	{
		return a > b;
	}

	template<typename T>
	bool GreaterThanOrEqual(T a, T b)
	{
		return a >= b;
	}
}

namespace Int32
{

float ToFloat(int32 value)
{
	return static_cast<float>(value);
}

double ToDouble(int32 value)
{
	return static_cast<double>(value);
}

uint32 ToUInt32(int32 value)
{
	return static_cast<uint32>(value);
}

int64 ToInt64(int32 value)
{
	return value;
}

uint64 ToUInt64(int32 value)
{
	return value;
}

bool FromString(const CSharedString& input, int32& result)
{
	if (input.length() == 0)
		return false;

	char* pEnd;
	result = std::strtol(input.c_str(), &pEnd, 10);

	return pEnd != nullptr && pEnd[0] == '\0';
}

void ToString(int32 value, CSharedString& result)
{
	ToString(result, value);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<int32>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Add<int32>, "8aaf394a-5288-40df-b31e-47e0c9757a93"_cry_guid, "Add");
		pFunction->SetDescription("Add A to B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Subtract<int32>, "cf5e826b-f22d-4fde-8a0c-259ceea75416"_cry_guid, "Subtract");
		pFunction->SetDescription("Subtract B from A");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Multiply<int32>, "d03e2bfe-79fc-4321-bf30-13be640443bc"_cry_guid, "Multiply");
		pFunction->SetDescription("Multiply A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Divide<int32>, "450af09f-e7ea-4c6b-8cda-a913e53f4460"_cry_guid, "Divide");
		pFunction->SetDescription("Divide A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Modulus<int32>, "37a5e2ba-caba-431f-882a-3048a6b18904"_cry_guid, "Modulus");
		pFunction->SetDescription("Calculate remainder when A is divided by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Random<int32>, "67bc3c63-e6f2-42c2-85af-a30fc30d0b11"_cry_guid, "Random");
		pFunction->SetDescription("Generate random number");
		pFunction->BindInput(1, 'min', "Min", "Minimum value");
		pFunction->BindInput(2, 'max', "Max", "Maximum value", int32(100));
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Abs<int32>, "f2f63e0a-3adc-4659-9444-32546d73c550"_cry_guid, "Absolute");
		pFunction->SetDescription("Returns the absolute value");
		pFunction->BindInput(1, 'x', "Value"); // #SchematycTODO : Rename 'X'!
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Negate<int32>, "{C244F1BA-9B6C-46E2-BB5C-6AD1CD0CBCF5}"_cry_guid, "Negate");
		pFunction->SetDescription("Returns the negated value, for example -1 becomes 1, 1 becomes -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Sign<int32>, "{37661484-156F-430F-AA8B-5F3CFBC87BCC}"_cry_guid, "Sign");
		pFunction->SetDescription("Gets the sign of the entity, for example 10 returns 1, -10 returns -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Equal<int32>, "{8437F7B5-E324-446F-AEC7-D7C5DAFCDFFB}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks if A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::NotEqual<int32>, "{57974989-AB5F-4290-8670-37C2B8D4B12E}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks if A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThan<int32>, "{8104A615-0C6D-4236-A4C2-2F2D63F7CA70}"_cry_guid, "LessThan");
		pFunction->SetDescription("Checks if A is less than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThanOrEqual<int32>, "{C202CA58-821A-4A89-B3B7-CC869F3E9996}"_cry_guid, "LessThanOrEqual");
		pFunction->SetDescription("Checks if A is less than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThan<int32>, "{ED112D0B-3FDC-48DE-8D2F-A2240F4B0541}"_cry_guid, "GreaterThan");
		pFunction->SetDescription("Checks if A is greater than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThanOrEqual<int32>, "{76BAAADB-ECEC-4038-A2EF-AB9CA696FBA0}"_cry_guid, "GreaterThanOrEqual");
		pFunction->SetDescription("Checks if A is greater than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToFloat, "dc514c68-89ea-4c14-8ce3-9b45f950409a"_cry_guid, "ToFloat");
		pFunction->SetDescription("Convert int32 to float");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToDouble, "{74595828-1697-4C83-B9AA-AF3020773899}"_cry_guid, "ToDouble");
		pFunction->SetDescription("Convert int32 to double");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt32, "FE95372E-42BA-48C7-A051-7B91BDC85DE4"_cry_guid, "ToUInt32");
		pFunction->SetDescription("Convert int32 to uint32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt64, "{D2E1CBF5-118B-4905-A28B-B82EEB7F9E7B}"_cry_guid, "ToInt64");
		pFunction->SetDescription("Convert int32 to int64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt64, "{A86DF850-D807-4967-8200-56E3CB437EC2}"_cry_guid, "ToUInt64");
		pFunction->SetDescription("Convert int32 to uint64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{7B0EBD0A-67DB-46B0-B039-C98AC33B6D44}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to 32-bit integer");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "71b9bfa9-8578-42e3-8ad3-18ff4bb74cca"_cry_guid, "ToString");
		pFunction->SetDescription("Convert int32 to string");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "String"); // #SchematycTODO : Rename 'Result'!
		scope.Register(pFunction);
	}
}
} // Int32

namespace UInt32
{
float ToFloat(uint32 value)
{
	return static_cast<float>(value);
}

double ToDouble(uint32 value)
{
	return static_cast<double>(value);
}

int32 ToInt32(uint32 value)
{
	return static_cast<int32>(value);
}

int64 ToInt64(uint32 value)
{
	return value;
}

uint64 ToUInt64(uint32 value)
{
	return value;
}

bool FromString(const CSharedString& input, uint32& result)
{
	if (input.length() == 0)
		return false;

	char* pEnd;
	result = std::strtoul(input.c_str(), &pEnd, 10);

	return pEnd != nullptr && pEnd[0] == '\0';
}

void ToString(uint32 value, CSharedString& result)
{
	ToString(result, value);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<uint32>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Add<uint32>, "5dcc5f2e-573e-41e8-8b38-4aaf5a9c0854"_cry_guid, "Add");
		pFunction->SetDescription("Add A to B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Subtract<uint32>, "bd78264c-f111-4760-b9bc-7a65fe2b722e"_cry_guid, "Subtract");
		pFunction->SetDescription("Subtract B from A");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Multiply<uint32>, "53a75fa7-e113-4f24-99c0-a88a54ea1bf7"_cry_guid, "Multiply");
		pFunction->SetDescription("Multiply A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Divide<uint32>, "2258b2d2-387d-41f3-bf98-63f2458fc1d9"_cry_guid, "Divide");
		pFunction->SetDescription("Divide A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Modulus<uint32>, "770748ab-83c5-4da5-8bf8-c9006b75d8d7"_cry_guid, "Modulus");
		pFunction->SetDescription("Calculate remainder when A is divided by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Random<uint32>, "d06c1c65-a8be-4473-aae0-23e7f8e1ff98"_cry_guid, "Random");
		pFunction->SetDescription("Generate random number");
		pFunction->BindInput(1, 'min', "Min", "Minimum value");
		pFunction->BindInput(2, 'max', "Max", "Maximum value", int32(100));
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Equal<uint32>, "{A53F1140-6DE8-4D75-A803-40085CCD8622}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks if A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::NotEqual<uint32>, "{66B34C59-8E3A-4698-AFBD-2051D88D644A}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks if A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThan<uint32>, "{B7757CAD-8FDE-4FF6-AD07-8D9E66F9B34E}"_cry_guid, "LessThan");
		pFunction->SetDescription("Checks if A is less than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThanOrEqual<uint32>, "{FC372AA0-1FDF-4932-A170-CE6101F7A464}"_cry_guid, "LessThanOrEqual");
		pFunction->SetDescription("Checks if A is less than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThan<uint32>, "{A4EFE085-DD9F-4BF7-92E4-35E32359B509}"_cry_guid, "GreaterThan");
		pFunction->SetDescription("Checks if A is greater than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThanOrEqual<uint32>, "{86C707B1-0ABD-48B8-88D0-CF768EBA2277}"_cry_guid, "GreaterThanOrEqual");
		pFunction->SetDescription("Checks if A is greater than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToFloat, "89fc18a2-0fde-4d07-ada6-7d95ef613132"_cry_guid, "ToFloat");
		pFunction->SetDescription("Convert uint32 to float");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToDouble, "{714CB1EB-A28D-414D-A6C2-AFCF38D23B1B}"_cry_guid, "ToDouble");
		pFunction->SetDescription("Convert uint32 to double");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt32, "ee7dbbc9-d950-44a4-9fd4-0a3891414147"_cry_guid, "ToInt32");
		pFunction->SetDescription("Convert uint32 to int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt64, "{2FD7605C-568B-43AA-ABEF-EC516EE27723}"_cry_guid, "ToInt64");
		pFunction->SetDescription("Convert uint32 to int64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt64, "{A520DB64-3A51-4F86-A085-1AD4B277E156}"_cry_guid, "ToUInt64");
		pFunction->SetDescription("Convert uint32 to uint64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{8D084C5F-B324-4ED3-964D-1EB14A79FA30}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to 32-bit unsigned integer");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "38a094d6-a8b2-4a87-baee-8d456faf3739"_cry_guid, "ToString");
		pFunction->SetDescription("Convert uint32 to string");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "String"); // #SchematycTODO : Rename 'Result'!
		scope.Register(pFunction);
	}
}
} // UInt32

namespace Int64
{
float ToFloat(int64 value)
{
	return static_cast<float>(value);
}

double ToDouble(int64 value)
{
	return static_cast<double>(value);
}

uint64 ToUInt64(int64 value)
{
	return static_cast<uint64>(value);
}

int32 ToInt32(int64 value)
{
	return static_cast<int32>(value);
}

uint32 ToUInt32(int64 value)
{
	return static_cast<uint32>(value);
}

bool FromString(const CSharedString& input, int64& result)
{
	if (input.length() == 0)
		return false;

	char* pEnd;
	result = std::strtoll(input.c_str(), &pEnd, 10);

	return pEnd != nullptr && pEnd[0] == '\0';
}

void ToString(int64 value, CSharedString& result)
{
	ToString(result, value);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<int64>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Add<int64>, "{1E961921-5A0E-471C-A07B-24F57FE3BAF6}"_cry_guid, "Add");
		pFunction->SetDescription("Add A to B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Subtract<int64>, "{141D1906-2AAF-4726-8000-835CE4C95845}"_cry_guid, "Subtract");
		pFunction->SetDescription("Subtract B from A");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Multiply<int64>, "{F980EFE1-90E8-41BD-A334-B8387BF34BC5}"_cry_guid, "Multiply");
		pFunction->SetDescription("Multiply A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Divide<int64>, "{24AD9580-282A-4ADA-8731-05D8DA8C9DF5}"_cry_guid, "Divide");
		pFunction->SetDescription("Divide A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Modulus<int64>, "{99DEDB24-345D-4C5A-8D88-57C8D7202523}"_cry_guid, "Modulus");
		pFunction->SetDescription("Calculate remainder when A is divided by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Random<int64>, "{7667E1F7-163F-4673-8328-354CC2320CB2}"_cry_guid, "Random");
		pFunction->SetDescription("Generate random number");
		pFunction->BindInput(1, 'min', "Min", "Minimum value");
		pFunction->BindInput(2, 'max', "Max", "Maximum value", int64(100));
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Abs<int64>, "{225C8DE2-A228-4995-8655-3B150FEED7A2}"_cry_guid, "Absolute");
		pFunction->SetDescription("Returns the absolute value");
		pFunction->BindInput(1, 'x', "Value");     // #SchematycTODO : Rename 'X'!
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Negate<int64>, "{766DB654-3AF5-4614-918C-DDB4E5A4A238}"_cry_guid, "Negate");
		pFunction->SetDescription("Returns the negated value, for example -1 becomes 1, 1 becomes -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Sign<int64>, "{AFC89602-7DF3-454B-8C5F-A6816241D248}"_cry_guid, "Sign");
		pFunction->SetDescription("Gets the sign of the entity, for example 10 returns 1, -10 returns -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Equal<int64>, "{692D9D73-F4D9-4C52-AA75-012027864389}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks if A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::NotEqual<int64>, "{1CEFF5AC-A494-41B1-B88D-6E434B581F91}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks if A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThan<int64>, "{DDBA735D-690B-4E67-8F74-4E1E4DD9625A}"_cry_guid, "LessThan");
		pFunction->SetDescription("Checks if A is less than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThanOrEqual<int64>, "{DD5BA103-7B01-4A09-842A-88B46D4C4DA7}"_cry_guid, "LessThanOrEqual");
		pFunction->SetDescription("Checks if A is less than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThan<int64>, "{EC3BDE63-3A82-4212-825A-DD9376A5AECE}"_cry_guid, "GreaterThan");
		pFunction->SetDescription("Checks if A is greater than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThanOrEqual<int64>, "{465FAE03-4432-4292-A103-49D52E10E23A}"_cry_guid, "GreaterThanOrEqual");
		pFunction->SetDescription("Checks if A is greater than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToFloat, "{142B5471-84F1-442C-AEDE-EEB214BB3361}"_cry_guid, "ToFloat");
		pFunction->SetDescription("Convert int64 to float");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToDouble, "{7A5CC0EE-D002-4CA4-894E-1C8742E6B8A3}"_cry_guid, "ToDouble");
		pFunction->SetDescription("Convert int64 to double");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt64, "{F4161A9B-191A-4AF0-9E1E-A3C7BE0A3529}"_cry_guid, "ToUint64");
		pFunction->SetDescription("Convert int64 to uint64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt32, "{69E2A610-73EE-4F43-87F1-0883F1789A05}"_cry_guid, "ToInt32");
		pFunction->SetDescription("Convert int64 to int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt32, "{53EBA7C6-279C-479C-BDF7-83EF39D6BEA5}"_cry_guid, "ToUInt32");
		pFunction->SetDescription("Convert int64 to uint32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{ACCA4FC5-3572-422C-9008-A907C272C6CF}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to 64-bit integer");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{2C209197-12D8-4464-9C66-088EB2DBE832}"_cry_guid, "ToString");
		pFunction->SetDescription("Convert int64 to string");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "String");
		scope.Register(pFunction);
	}
}
} // Int64

namespace UInt64
{
float ToFloat(uint64 value)
{
	return static_cast<float>(value);
}

double ToDouble(uint64 value)
{
	return static_cast<double>(value);
}

int64 Toint64(uint64 value)
{
	return static_cast<int64>(value);
}

int32 ToInt32(uint64 value)
{
	return static_cast<int32>(value);
}

uint32 ToUInt32(uint64 value)
{
	return static_cast<uint32>(value);
}

bool FromString(const CSharedString& input, uint64& result)
{
	if (input.length() == 0)
		return false;

	char* pEnd;
	result = std::strtoull(input.c_str(), &pEnd, 10);

	return pEnd != nullptr && pEnd[0] == '\0';
}

void ToString(uint64 value, CSharedString& result)
{
	ToString(result, value);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<uint64>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Add<uint64>, "{940584A6-796F-49CB-B2A5-A9596F2A7C75}"_cry_guid, "Add");
		pFunction->SetDescription("Add A to B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Subtract<uint64>, "{FB88513F-E5D0-4735-BAED-5EE26DF87CC8}"_cry_guid, "Subtract");
		pFunction->SetDescription("Subtract B from A");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Multiply<uint64>, "{E398B4BA-C5DD-4B46-B290-50EE0D7D84C4}"_cry_guid, "Multiply");
		pFunction->SetDescription("Multiply A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Divide<uint64>, "{ED8FD28F-0FDA-416C-A2B3-EB3F654D5FF6}"_cry_guid, "Divide");
		pFunction->SetDescription("Divide A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Modulus<uint64>, "{44BA970F-A81A-4541-A05D-21278AABA776}"_cry_guid, "Modulus");
		pFunction->SetDescription("Calculate remainder when A is divided by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Random<uint64>, "{02DCFED4-79D6-41EB-80BE-6E8AE2060946}"_cry_guid, "Random");
		pFunction->SetDescription("Generate random number");
		pFunction->BindInput(1, 'min', "Min", "Minimum value");
		pFunction->BindInput(2, 'max', "Max", "Maximum value", int64(100));
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Equal<uint64>, "{8D1E601E-294E-410B-8827-516071F2C939}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks if A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::NotEqual<uint64>, "{E3D91338-3DF1-40EB-A3BF-CFBFAC95AA43}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks if A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThan<uint64>, "{21380C8D-779C-4B75-ABD4-6D474FD00BD4}"_cry_guid, "LessThan");
		pFunction->SetDescription("Checks if A is less than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThanOrEqual<uint64>, "{7053F250-AC88-4427-9268-1E27F5711F83}"_cry_guid, "LessThanOrEqual");
		pFunction->SetDescription("Checks if A is less than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThan<uint64>, "{9ED39160-C922-4AF3-825D-FB24E8C36A32}"_cry_guid, "GreaterThan");
		pFunction->SetDescription("Checks if A is greater than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThanOrEqual<uint64>, "{620671E2-9A77-4857-A437-FCA22A4136A8}"_cry_guid, "GreaterThanOrEqual");
		pFunction->SetDescription("Checks if A is greater than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToFloat, "{E9A7CD1A-A7B4-48B7-8BA6-0E0776D649E6}"_cry_guid, "ToFloat");
		pFunction->SetDescription("Convert uint64 to float");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToDouble, "{6D3A9507-CED1-42D2-AF96-94DA63906463}"_cry_guid, "ToDouble");
		pFunction->SetDescription("Convert uint64 to double");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Toint64, "{221B1BB6-8472-4691-B1D1-1DF936D74F76}"_cry_guid, "Toint64");
		pFunction->SetDescription("Convert uint64 to int64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt32, "{463DBC34-CAA6-4D0E-A375-AA469205C59F}"_cry_guid, "ToInt32");
		pFunction->SetDescription("Convert uint64 to int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt32, "{C661E392-89A3-4FC0-8F42-9D56EB93B168}"_cry_guid, "ToUInt32");
		pFunction->SetDescription("Convert uint64 to uint32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{2F77F015-BA78-4A36-88BD-3EB9AAA370BC}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to 64-bit unsigned integer");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{B6D6B48D-9617-437D-9A13-58B36513B8D1}"_cry_guid, "ToString");
		pFunction->SetDescription("Convert uint64 to string");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "String");
		scope.Register(pFunction);
	}
}
} // UInt64

namespace Float
{

float Sin(float x)
{
	return sin_tpl(x);
}

float ArcSin(float x)
{
	return asin_tpl(x);
}

float Cos(float x)
{
	return cos_tpl(x);
}

float ArcCos(float x)
{
	return acos_tpl(x);
}

float Tan(float x)
{
	return tan_tpl(x);
}

float ArcTan(float x)
{
	return atan_tpl(x);
}

float Modulus(float a, float b)
{
	return b != 0.0f ? fmodf(a, b) : 0.0f;
}

float Abs(float x)
{
	return fabs_tpl(x);
}

double ToDouble(float input)
{
	return input;
}

int32 ToInt32(float input)
{
	return static_cast<int32>(input);
}

uint32 ToUInt32(float input)
{
	return static_cast<uint32>(input);
}

int64 ToInt64(float input)
{
	return static_cast<int64>(input);
}

uint64 ToUInt64(float input)
{
	return static_cast<uint64>(input);
}

bool FromString(const CSharedString& input, float& result)
{
	if (input.length() == 0)
		return false;

	char* pEnd;
	result = std::strtof(input.c_str(), &pEnd);

	return pEnd != nullptr && pEnd[0] == '\0';
}

void ToString(float input, CSharedString& output)
{
	Schematyc::ToString(output, input);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<float>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Add<float>, "d62aede8-ed2c-47d9-b7f3-6de5ed0ef51f"_cry_guid, "Add");
		pFunction->SetDescription("Add A to B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Subtract<float>, "3a7c8a1d-2938-40f8-b5ac-b6becf32baf2"_cry_guid, "Subtract");
		pFunction->SetDescription("Subtract B from A");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Multiply<float>, "b04f428a-4408-4c3d-b73b-173a9738c857"_cry_guid, "Multiply");
		pFunction->SetDescription("Multiply A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Divide<float>, "fd26901b-dbbe-4f4d-b8c1-6547f55339ae"_cry_guid, "Divide");
		pFunction->SetDescription("Divide A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Clamp<float>, "350ad86a-a61a-40c4-9f2e-30b6fdf2ae06"_cry_guid, "Clamp");
		pFunction->SetDescription("Clamp the value between min and max");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindInput(2, 'min', "Min");
		pFunction->BindInput(3, 'max', "Max");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Modulus, "fc35a48f-7926-4417-b940-08162bc4a9a5"_cry_guid, "Modulus");
		pFunction->SetDescription("Calculate remainder when A is divided by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Sin, "{7F014586-066F-467B-B8C5-F2862BCF193A}"_cry_guid, "Sin");
		pFunction->SetDescription("Calculate sine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ArcSin, "{EF239779-D4A9-406B-82F6-8688E27A3962}"_cry_guid, "ArcSin");
		pFunction->SetDescription("Calculate arc-sine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Cos, "{EFC13075-2624-42D8-8D15-F5E24E68A4E8}"_cry_guid, "Cos");
		pFunction->SetDescription("Calculate cosine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ArcCos, "{3852F6E4-6D24-4278-AA84-4888C04C1B12}"_cry_guid, "ArcCos");
		pFunction->SetDescription("Calculate arc-cosine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Tan, "{746FCEB3-B64F-4384-A988-C2593D70A874}"_cry_guid, "Tan");
		pFunction->SetDescription("Calculate tangent of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ArcTan, "{E7934899-D17F-45EF-BFB4-D37B0D831E13}"_cry_guid, "ArcTan");
		pFunction->SetDescription("Calculate arc-tangent of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Random<float>, "a17b82b7-3b4a-440d-a4f1-082f3d9cf0f5"_cry_guid, "Random");
		pFunction->SetDescription("Generate random number within range");
		pFunction->BindInput(1, 'min', "Min", "Minimum value", 0.0f);
		pFunction->BindInput(2, 'max', "Max", "Minimum value", 100.0f);
		pFunction->BindOutput(0, 'res', "Output");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Abs, "08e66eec-9e40-4fe9-b4dd-0df8073c681d"_cry_guid, "Abs");
		pFunction->SetDescription("Calculate absolute (non-negative) value");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Equal<float>, "{DBF69A60-658D-48A8-802E-2EB6220ED60B}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks if A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::NotEqual<float>, "{94BD55E0-0E9B-4740-9951-00D494B56A81}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks if A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThan<float>, "{C75AEA3B-E845-40A2-887C-861B2704DDFD}"_cry_guid, "LessThan");
		pFunction->SetDescription("Checks if A is less than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThanOrEqual<float>, "{5EF1C108-DE37-4076-BF40-E8652F9DF67D}"_cry_guid, "LessThanOrEqual");
		pFunction->SetDescription("Checks if A is less than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThan<float>, "{E51E4BD3-EC49-48FB-9C00-D354D137857F}"_cry_guid, "GreaterThan");
		pFunction->SetDescription("Checks if A is greater than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThanOrEqual<float>, "{C6F34366-D6E8-460B-AA96-CEC175E0959D}"_cry_guid, "GreaterThanOrEqual");
		pFunction->SetDescription("Checks if A is greater than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Negate<float>, "{7FA0213D-9354-4E2A-A47F-971323EAC9F6}"_cry_guid, "Negate");
		pFunction->SetDescription("Returns the negated value, for example -1 becomes 1, 1 becomes -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Sign<float>, "{00670DC4-663A-424B-ADC3-052B5CF5CEF0}"_cry_guid, "Sign");
		pFunction->SetDescription("Gets the sign of the entity, for example 10 returns 1, -10 returns -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToDouble, "{F9A16BFD-D25C-4B03-95D9-78969940B0DF}"_cry_guid, "ToDouble");
		pFunction->SetDescription("Convert to Double");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt32, "6e6356f3-7cf6-4746-9ef7-e8031c2a4117"_cry_guid, "ToInt32");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to Int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt32, "FAA240D7-7187-4A9F-8345-3D77A095EDE2"_cry_guid, "ToUInt32");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to UInt32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt64, "{CF6E8DF8-69BD-4CC7-81D1-EDBF06000C23}"_cry_guid, "ToInt64");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to Int64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt64, "{B7393432-5BD9-416B-BB68-8DD89DA800FC}"_cry_guid, "ToUInt64"); //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to UInt64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{C33F2524-E096-497D-85D5-951814025B6A}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to 32-bit floating point");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "171188d6-b66a-4d4b-8508-b6c003c9da78"_cry_guid, "ToString");
		pFunction->SetDescription("Convert to String");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
}
} // Float

namespace Double
{

double Sin(double x)
{
	return sin_tpl(x);
}

double ArcSin(double x)
{
	return asin_tpl(x);
}

double Cos(double x)
{
	return cos_tpl(x);
}

double ArcCos(double x)
{
	return acos_tpl(x);
}

double Tan(double x)
{
	return tan_tpl(x);
}

double ArcTan(double x)
{
	return atan_tpl(x);
}

double Modulus(double a, double b)
{
	return b != 0.0f ? fmod(a, b) : 0.0f;
}

double Abs(double x)
{
	return fabs_tpl(x);
}

float ToFloat(double input)
{
	return static_cast<float>(input);
}

int32 ToInt32(double input)
{
	return static_cast<int32>(input);
}

uint32 ToUInt32(double input)
{
	return static_cast<uint32>(input);
}

int64 ToInt64(double input)
{
	return static_cast<int64>(input);
}

uint64 ToUInt64(double input)
{
	return static_cast<uint64>(input);
}

bool FromString(const CSharedString& input, double& result)
{
	if (input.length() == 0)
		return false;

	char* pEnd;
	result = std::strtod(input.c_str(), &pEnd);

	return pEnd != nullptr && pEnd[0] == '\0';
}

void ToString(double input, CSharedString& output)
{
	Schematyc::ToString(output, input);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<double>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Add<double>, "{C995BCED-13D3-4AB8-9277-0D3FD564617F}"_cry_guid, "Add");
		pFunction->SetDescription("Add A to B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Subtract<double>, "{312203BD-DA13-4669-81FA-935F9ED0B2CB}"_cry_guid, "Subtract");
		pFunction->SetDescription("Subtract B from A");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Multiply<double>, "{72123DAE-BBA3-4376-94EC-944933946FDA}"_cry_guid, "Multiply");
		pFunction->SetDescription("Multiply A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Divide<double>, "{0B8A7193-0757-4FA1-BF5C-D2AC3ADD75CF}"_cry_guid, "Divide");
		pFunction->SetDescription("Divide A by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Clamp<double>, "{8C076F73-5128-46C4-9768-CAB27112B82D}"_cry_guid, "Clamp");
		pFunction->SetDescription("Clamp the value between min and max");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindInput(2, 'min', "Min");
		pFunction->BindInput(3, 'max', "Max");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Modulus, "{80FBB748-DDD4-42BA-8987-4578D6F144E8}"_cry_guid, "Modulus");
		pFunction->SetDescription("Calculate remainder when A is divided by B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Sin, "{89AA6170-472E-4C65-ADBC-78EB9C8B81E4}"_cry_guid, "Sin");
		pFunction->SetDescription("Calculate sine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ArcSin, "{30A000A6-7D85-461E-B052-9718F10FCEDA}"_cry_guid, "ArcSin");
		pFunction->SetDescription("Calculate arc-sine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Cos, "{F41EC509-9273-4535-881B-BE9A9368679A}"_cry_guid, "Cos");
		pFunction->SetDescription("Calculate cosine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ArcCos, "{70C7B73B-E532-4B8D-9803-E8EE0867FF62}"_cry_guid, "ArcCos");
		pFunction->SetDescription("Calculate arc-cosine of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Tan, "{1E8305C6-9514-4910-ABC5-2477CC5F913B}"_cry_guid, "Tan");
		pFunction->SetDescription("Calculate tangent of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ArcTan, "{8AB9DC60-6A6A-4982-8E9A-F421DEAFB9A9}"_cry_guid, "ArcTan");
		pFunction->SetDescription("Calculate arc-tangent of X");
		pFunction->BindInput(1, 'x', "X");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Random<double>, "{37FF2FE8-25E2-4680-8954-EE3BFA74C16A}"_cry_guid, "Random");
		pFunction->SetDescription("Generate random number within range");
		pFunction->BindInput(1, 'min', "Min", "Minimum value", 0.0f);
		pFunction->BindInput(2, 'max', "Max", "Minimum value", 100.0f);
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Abs, "{AF0CF093-C7B7-44A7-B5D4-DEB3DFA0C7A0}"_cry_guid, "Abs");
		pFunction->SetDescription("Calculate absolute (non-negative) value");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Equal<double>, "{361D9B2C-A836-4F76-8CF0-33985EFC6EF8}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks if A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::NotEqual<double>, "{A61F9E59-0631-4761-A630-1211A4CE6101}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks if A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThan<double>, "{943EEDC5-5C95-4AF1-8538-A57FF7941EBD}"_cry_guid, "LessThan");
		pFunction->SetDescription("Checks if A is less than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::LessThanOrEqual<double>, "{70664950-A5A3-4917-8AA1-086DD0E807DD}"_cry_guid, "LessThanOrEqual");
		pFunction->SetDescription("Checks if A is less than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThan<double>, "{89B38118-1608-4691-B12E-2D195A81F781}"_cry_guid, "GreaterThan");
		pFunction->SetDescription("Checks if A is greater than B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::GreaterThanOrEqual<double>, "{7BAD9011-69D1-4594-9070-DF1E099341A5}"_cry_guid, "GreaterThanOrEqual");
		pFunction->SetDescription("Checks if A is greater than or equals B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Negate<double>, "{834B4CEF-513B-4EEF-8F98-21C835A836DC}"_cry_guid, "Negate");
		pFunction->SetDescription("Returns the negated value, for example -1 becomes 1, 1 becomes -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Generic::Sign<double>, "{6C411FBC-2895-449C-802F-15B920FA4AD1}"_cry_guid, "Sign");
		pFunction->SetDescription("Gets the sign of the entity, for example 10 returns 1, -10 returns -1");
		pFunction->BindInput(1, 'x', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToFloat, "{44F86B7E-F245-49A0-B789-262755ED27DF}"_cry_guid, "ToFloat");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to Int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt32, "{FFC5D22A-00F9-4A9D-9E05-F8058C673075}"_cry_guid, "ToInt32");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to Int32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt32, "{A855060B-072E-4D67-9221-086A55287554}"_cry_guid, "ToUInt32");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to UInt32");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToInt64, "{C87D0AE2-7691-4971-8556-E2D8A0581F4B}"_cry_guid, "ToInt64");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to Int64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToUInt64, "{F0F075B4-EE58-4284-A9D9-68BF3DD5D5C3}"_cry_guid, "ToUInt64");  //#SchematycTODO: parameter to define how to round
		pFunction->SetDescription("Convert to UInt64");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{E2E49C0A-0DDC-4D02-97ED-1D063B59BDCA}"_cry_guid, "FromString");
		pFunction->SetDescription("Convert string to 64-bit floating point");
		pFunction->BindOutput(0, 'vlid', "Valid");
		pFunction->BindInput(1, 'str', "Input");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToString, "{FB8465E0-B05C-45F6-BD06-4BEF1CE9BB66}"_cry_guid, "ToString");
		pFunction->SetDescription("Convert to String");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
}
} // Double

namespace String
{
void Append(const CSharedString& a, const CSharedString& b, CSharedString& result)
{
	result = a;
	result.append(b.c_str());
}

bool Equal(const CSharedString& a, const CSharedString& b)
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

bool NotEqual(const CSharedString& a, const CSharedString& b)
{
	return strcmp(a.c_str(), b.c_str()) != 0;
}

void Tokenize(const CSharedString& a, const CSharedString& delimeter, Schematyc::CAnyArrayPtr pArray)
{
	if (a.length() > 0 && pArray)
	{
		pArray->Clear();

		const auto& typeDesc = GetTypeDesc<CSharedString>();
		stack_string local(a.c_str());
		int curPos = 0;
		do
		{
			const CSharedString result(local.Tokenize(delimeter.c_str(), curPos).c_str());
			pArray->PushBack(CAnyConstRef(typeDesc, &result));
		} while (curPos != -1);
	}
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<CSharedString>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Append, "f7984665-576c-44cb-8bbb-7401365faa7a"_cry_guid, "Append");
		pFunction->SetDescription("Combine A and B");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(3, 'res', "String"); // #SchematycTODO : Rename 'Result'!
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Tokenize, "{DE3C03E0-15BC-4D40-8292-2F5618573C4B}"_cry_guid, "Tokenize");
		pFunction->SetDescription("Tokenizes a string based on delimeter into the provided array");
		pFunction->BindInput(1, 'inp', "Input");
		pFunction->BindInput(2, 'tokn', "Delimeter");
		pFunction->BindInputArrayType<CSharedString>(3, 'arr', "Array[String]", nullptr);
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Equal, "{86F6ED6B-44E3-4534-B600-37BA398760D5}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks whether A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{A891817C-422C-4121-9A47-5D81742A06AF}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks whether A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
}
} // String

namespace GUID
{
bool Equal(const CryGUID& a, const CryGUID& b)
{
	return a == b;
}

bool NotEqual(const CryGUID& a, const CryGUID& b)
{
	return a != b;
}

CryGUID FromString(const CSharedString& value)
{
	return CryGUID::FromString(value.c_str());
}

void ToStringProper(const CryGUID& value, CSharedString& result)
{
	ToString(result, value);
}

static void RegisterFunctions(IEnvRegistrar& registrar)
{
	CEnvRegistrationScope scope = registrar.Scope(GetTypeDesc<CryGUID>().GetGUID());
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Equal, "{6D0832D2-897F-49B1-BB21-6E577E26E2F1}"_cry_guid, "Equal");
		pFunction->SetDescription("Checks whether A and B are equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&NotEqual, "{6FF243C2-C98F-4DE4-BDD0-6763DAE7E56C}"_cry_guid, "NotEqual");
		pFunction->SetDescription("Checks whether A and B are not equal");
		pFunction->BindInput(1, 'a', "A");
		pFunction->BindInput(2, 'b', "B");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Empty, "{C29AD647-122D-4D39-90BA-D73AF25166EC}"_cry_guid, "CreateEmpty");
		pFunction->SetDescription("Creates an empty GUID");
		pFunction->BindOutput(0, 'res', "GUID");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEmpty, "{D76B37C6-DDD2-440A-979F-E03EF62BD9F6}"_cry_guid, "IsEmpty");
		pFunction->SetDescription("Whether the GUID is empty");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FromString, "{3C2BC963-97AF-46D0-B498-179C4066F3DE}"_cry_guid, "FromString");
		pFunction->SetDescription("Create GUID from a string");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(0, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ToStringProper, "{EB0BC76B-0C78-4AF1-9F26-385A3C52F45A}"_cry_guid, "ToString");
		pFunction->SetDescription("Convert to String");
		pFunction->BindInput(1, 'val', "Value");
		pFunction->BindOutput(2, 'res', "Result");
		scope.Register(pFunction);
	}
}
}

static void RegisterBasicFunctions(IEnvRegistrar& registrar)
{
	Bool::RegisterFunctions(registrar);
	Int32::RegisterFunctions(registrar);
	UInt32::RegisterFunctions(registrar);
	Int64::RegisterFunctions(registrar);
	UInt64::RegisterFunctions(registrar);
	Float::RegisterFunctions(registrar);
	Double::RegisterFunctions(registrar);
	String::RegisterFunctions(registrar);
	GUID::RegisterFunctions(registrar);
}
} // Schematyc

CRY_STATIC_AUTO_REGISTER_FUNCTION(&Schematyc::RegisterBasicFunctions)
