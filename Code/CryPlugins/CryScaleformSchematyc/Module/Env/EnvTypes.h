// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySchematyc/Utils/Any.h>

namespace Cry::Scaleform
{

class CClassDesc;
class CClassMemberDesc;

// Enum of types available for dynamic allocation of parameters
enum class EDynamicType
{
	Boolean,
	Float,
	Integer,
	String,

	Any
};

}; // Cry::Scaleform