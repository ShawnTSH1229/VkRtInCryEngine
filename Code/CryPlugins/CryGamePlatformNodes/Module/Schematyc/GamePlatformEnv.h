// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySchematyc/Env/IEnvRegistrar.h>

namespace Cry::GamePlatform
{
// Copied from CoreEnv
static constexpr CryGUID g_stdModuleGUID = "a929e72e-d3a0-46a0-949b-17d22162ec33"_cry_guid;

static constexpr CryGUID g_gamePlatformGUID = "{93C40466-A5BB-4C73-81CE-2CE903DF7BE6}"_cry_guid;
static constexpr CryGUID g_gamePlatformDataTypesGUID = "{0BBD7503-E436-418F-A5CF-81BB94529EE2}"_cry_guid;
static constexpr CryGUID g_gamePlatformServiceGUID = "{CEEDB07E-FF96-4A3F-BBB0-4439966847FF}"_cry_guid;

void RegisterPlatformEnv(Schematyc::IEnvRegistrar& registrar);
} // Cry::GamePlatform
