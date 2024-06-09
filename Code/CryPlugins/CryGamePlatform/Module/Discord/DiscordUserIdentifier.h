// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "DiscordTypes.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			inline AccountIdentifierValue ExtractDiscordID(const AccountIdentifier& accountId)
			{
				if (accountId.Service() == DiscordServiceID)
				{
					AccountIdentifierValue rawId;
					accountId.GetAsUint64(rawId);

					return AccountIdentifierValue(rawId);
				}

				CRY_ASSERT(false, "[Discord][UserIdentifier] AccountIdentifier '%s' does not seem to contain a valid Discord ID", accountId.ToDebugString());
				return AccountIdentifierValue();
			}
		}
	}
}