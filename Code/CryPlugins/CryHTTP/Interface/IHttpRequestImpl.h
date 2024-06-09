// Copyright 2020-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "HttpTypes.h"

namespace Cry
{
	namespace Http
	{
		struct SResponse;

		namespace Detail
		{
			struct IRequestImpl
			{
				virtual ~IRequestImpl() = default;
				virtual ERequestStatus GetStatus() const = 0;
				virtual void Cancel() = 0;
				virtual std::unique_ptr<SResponse> ExtractResponse() = 0;
			};
		}
	}
}