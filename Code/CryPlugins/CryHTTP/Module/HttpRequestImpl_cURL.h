// Copyright 2020-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "HttpRequest.h"

#ifdef CRY_PLATFORM_WINDOWS
#include <CryCore/Platform/CryWindows.h>
#include <winsock2.h>
#endif

#include <CryCore/Containers/CryArray.h>
#include <curl/curl.h>

struct ITimer;

namespace Cry
{
namespace Http
{
namespace cURL
{
class CRequest final : public IRequest
{
public:
	CRequest(CURLM* pMulti, CURL* pEasy, ITimer* pTimer);
	~CRequest();

	virtual void          SetRequestMethod(EMethod m) override;
	virtual void          SetRequestUrl(const char* szUrl) override;
	virtual const char*   GetRequestUrl() const override { return m_url.c_str(); }
	virtual bool          SetRequestBody(const uint8* pBuffer, size_t bufferSize) override;
	virtual bool          SetTrustAllCertificates(bool trustAll) override;
	virtual bool          AddRequestHeader(const char* szKey, const char* szValue) override;

	virtual bool          Start() override;
	virtual void          Cancel() override;
	virtual ERequestError GetMostRecentError() const override { return m_lastError; }
	virtual ERequestState GetState() const override           { return m_state; }

	virtual EStatusCode   GetResponseStatusCode() const override;
	virtual size_t        GetResponseSize() const override { return m_response.size(); }
	virtual const uint8*  GetResponsePtr() const override  { return m_response.data(); }

	static void           OnMsg(CURLMsg& message);
	static void           OnCleanup(CURL* pHandle);
	static float          GetLastTimeHandled(CURL* pHandle);

private:
	void                 Cleanup();
	static CRequest*     GetRequest(CURL* pHandle);
	static size_t        CurlWriteResponse(char* pBuffer, size_t size, size_t nitems, void* pUserData);
	static int           CurlTransferInfo(void* pUserData, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	size_t               WriteResponse(const char* pBuffer, size_t bytes);
	float                GetLastTimeHandled() const;
	bool                 ProcessErrorCode(CURLcode code, const char* szOffender) const;
	bool                 ProcessErrorCode(CURLMcode code, const char* szOffender) const;
	static ERequestError ToRequestError(CURLcode errCode);
	static ERequestError ToRequestError(CURLMcode errCode);

	string                m_url;

	CURLM*                m_pMulti = nullptr;
	CURL*                 m_pHandle = nullptr;
	curl_slist*           m_pHeaderList = nullptr;
	char                  m_errbuf[CURL_ERROR_SIZE] = "";

	DynArray<uint8>       m_response;

	ERequestState         m_state = ERequestState::Error;
	mutable ERequestError m_lastError = ERequestError::InvalidOperation;
	float                 m_lastTimeHandled = 0.0f;
	ITimer*               m_pTimer;
};
}
}
}