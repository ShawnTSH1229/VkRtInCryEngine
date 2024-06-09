// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "PlatformTypes.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			using AccountIdentifierValue = Cry::GamePlatform::Detail::NumericIdentifierValue;
			using ApplicationIdentifierValue = Cry::GamePlatform::Detail::NumericIdentifierValue;

			inline AccountIdentifier     CreateAccountIdentifier(const AccountIdentifierValue& discordId)         { return AccountIdentifier(DiscordServiceID, discordId); }
			inline ApplicationIdentifier CreateApplicationIdentifier(const ApplicationIdentifierValue& discordId) { return ApplicationIdentifier(DiscordServiceID, discordId); }

			inline void LogWarning(const char* szFormat, ...) PRINTF_PARAMS(1, 2);
			inline void LogWarning(const char* szFormat, ...)
			{
				va_list args;
				va_start(args, szFormat);
				stack_string format("[Discord] ");
				format.append(szFormat);
				GetISystem()->WarningV(VALIDATOR_MODULE_ONLINE, VALIDATOR_WARNING, 0, nullptr, format.c_str(), args);
				va_end(args);
			}

#define WarnOnce(...) do { static bool logged = false; if (!logged) { LogWarning(__VA_ARGS__); logged = true; } } while (false)
		}
	}
}
