// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

namespace Cry::Scaleform
{

void RegisterElementEvents(Schematyc::CEnvRegistrationScope& scope, IUIElement* pElement, const CryGUID& componentGuid);

} // Cry::Scaleform