// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformBase.h"

namespace Cry
{
	namespace GamePlatform
	{
		struct IStreamInstall
		{
			struct IListener
			{
				virtual ~IListener() = default;

				virtual void OnInstallPercentageChanged(uint32 percentInstalled) = 0;
			};

			virtual ~IStreamInstall() {}

			enum class InstallStatus
			{
				Completed,
				Error,
				Paused,
				Pending,
				Running,
				Unknown,
			};

			//! Adds a service event listener
			virtual void AddListener(IListener& listener, const char* szName) = 0;
			//! Removes a service event listener
			virtual void RemoveListener(IListener& listener) = 0;
			//! Query the current installation status
			virtual InstallStatus GetStatus() const = 0;
		};
	}
}