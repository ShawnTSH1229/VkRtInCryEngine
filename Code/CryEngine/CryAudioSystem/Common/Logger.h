// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryAudio/IAudioSystem.h>
#include <CrySystem/ISystem.h>

/**
 * @namespace Cry::Audio
 * @brief Most parent audio namespace used throughout the engine.
 */
namespace Cry::Audio
{
/**
 * A utility function to log audio specific messages.
 * @param type - log message type (ELogType)
 * @param szFormat, ... - printf-style format string and its arguments
 */
template<typename ... Args>
static void Log(CryAudio::ELogType const type, char const* const szFormat, Args&& ... args)
{
	if (gEnv->pAudioSystem != nullptr)
	{
		gEnv->pAudioSystem->Log(type, szFormat, std::forward<Args>(args) ...);
	}
}
} // namespace Cry::Audio
