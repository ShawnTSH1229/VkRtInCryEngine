// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include "IHttpPlugin.h"

#include <CrySystem/ITimer.h>

#ifdef CRY_PLATFORM_WINDOWS
#include <CryCore/Platform/CryWindows.h>
	#include <winsock2.h>
#endif

#include <curl/curl.h>

namespace Cry::Http::cURL
{

class CRequest;

class CPlugin final : public Cry::Http::IPlugin
{
	CRYINTERFACE_BEGIN()
	CRYINTERFACE_ADD(Cry::Http::IPlugin)
	CRYINTERFACE_ADD(Cry::IEnginePlugin)
	CRYINTERFACE_END()

	CRYGENERATE_SINGLETONCLASS_GUID(CPlugin, "Plugin_CryHTTP", "{D6F141B9-CDD8-44AB-ACF5-9C70EBC646CB}"_cry_guid)

	CPlugin() = default;
	virtual ~CPlugin();

public:
	// ICryPlugin
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	virtual void MainUpdate(float frameTime) override;
	// ~ICryPlugin

	// IPlugin_HTTP
	virtual RequestPtr    CreateRequest() override final;
	virtual EUpdateResult ProcessRequests() override final;
	// ~IPlugin_HTTP

private:
	static void        FormatHexDump(CryStackStringT<char, 1>& text, const char* pData, size_t size);
	static const char* CurlStrInfoType(curl_infotype type);
	static int         CurlDebugFunction(CURL* pHandle, curl_infotype type, char* pData, size_t size, void* pUser);

private:
	float                     m_requestTimeOutInSeconds = FLT_MAX;
	int                       m_logVerbosity = 3;
	ITimer*                   m_pTimer = nullptr;
	CURLM*                    m_pMultiHandle = nullptr;
	std::unordered_set<CURL*> m_handles;       // All known handles, used for timeouts
};

}