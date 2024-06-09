// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

namespace Cry::Scaleform
{

void RegisterDynamicFunctions(Schematyc::CEnvRegistrationScope& scope, IUIElement* pElement, const Schematyc::CCommonTypeDesc* pComponentDesc);

} // Cry::Scaleform