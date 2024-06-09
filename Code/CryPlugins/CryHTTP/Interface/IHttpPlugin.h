// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <CrySystem/ICryPlugin.h>
#include "HttpRequest.h"

namespace Cry
{
	namespace Http
	{
		//! Represents a plug-in that exposes functionality for sending HTTP requests and handling responses
		class IPlugin : public Cry::IEnginePlugin
		{
		public:
			CRYINTERFACE_DECLARE_GUID(IPlugin, "{BC0BA532-EAAD-4B91-AA71-C65E435ABDC1}"_cry_guid);

			virtual ~IPlugin() { }

			enum class EUpdateResult
			{
				Idle = 0,
				ProcessingRequests
			};

			//! Allocates a request object
			//! \returns A request object, or nullptr if request failed
			virtual RequestPtr CreateRequest() = 0;

			//! Processes currently on-going HTTP requests, downloading / uploading and triggering callbacks
			//! This can be called as many times as desired even in a single frame, but will also be automatically done once per frame
			//! \return Idle if no requests are being handled, otherwise ProcessingRequests
			virtual EUpdateResult ProcessRequests() = 0;
		};
	}
}