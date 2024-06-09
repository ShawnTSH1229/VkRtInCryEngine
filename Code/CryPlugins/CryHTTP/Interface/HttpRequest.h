// Copyright 2020-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryCore/smartptr.h>
#include "HttpTypes.h"

namespace Cry
{
	namespace Http
	{
		//! \note All strings expected to be encoded as UTF-8
		class IRequest : private CMultiThreadRefCount
		{
		public:
			IRequest() = default;
			~IRequest() = default;

			using CMultiThreadRefCount::AddRef;
			using CMultiThreadRefCount::Release;

			virtual void          SetRequestMethod(EMethod m) = 0;
			virtual void          SetRequestUrl(const char* szUrl) = 0;
			virtual const char*   GetRequestUrl() const = 0;
			virtual bool          SetRequestBody(const uint8* pBuffer, size_t bufferSize) = 0;
			virtual bool          SetTrustAllCertificates(bool trustAll) = 0;
			virtual bool          AddRequestHeader(const char* szKey, const char* szValue) = 0;

			virtual bool          Start() = 0;
			virtual void          Cancel() = 0;
			virtual ERequestError GetMostRecentError() const = 0;
			virtual ERequestState GetState() const = 0;

			virtual EStatusCode   GetResponseStatusCode() const = 0;
			virtual size_t        GetResponseSize() const = 0;
			virtual const uint8*  GetResponsePtr() const = 0;
		};

		using RequestPtr = _smart_ptr<IRequest>;
	}
}
