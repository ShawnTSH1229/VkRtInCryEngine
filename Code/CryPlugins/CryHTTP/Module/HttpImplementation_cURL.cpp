// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>
#include <CrySystem/ConsoleRegistration.h>

#include "HttpImplementation_cURL.h"
#include "HttpRequestImpl_cURL.h"

namespace Cry::Http::cURL
{

/*static*/ void CPlugin::FormatHexDump(CryStackStringT<char, 1>& text, const char* pData, size_t size)
{
	constexpr size_t width = 0x10;
	const size_t lines = (size / width) + 1;

	text.clear();
	text.reserve(lines * 4 * (width + 2));

	stack_string fmt;
	for (size_t baseAddr = 0; baseAddr < size; baseAddr += width)
	{
		const size_t maxC = std::min(width, size - baseAddr);

		fmt.Format("%04zX: ", baseAddr);
		text.append(fmt);

		// show hex to the left
		for (size_t c = 0; c < maxC; c++)
		{
			fmt.Format("%02X ", static_cast<uint8>(pData[baseAddr + c]));
			text.append(fmt);
		}

		if (maxC < width)
		{
			text.append(3 * (width - maxC), ' ');
		}

		// show data on the right
		for (size_t c = 0; c < maxC; c++)
		{
			const char x = (pData[baseAddr + c] >= 0x20 && pData[baseAddr + c] < 0x80) ? pData[baseAddr + c] : '.';
			text.append(1, x);
		}

		text += "\n";
	}
}

/*static*/ const char* CPlugin::CurlStrInfoType(curl_infotype type)
{
	switch (type)
	{
	case CURLINFO_TEXT:
		return "TEXT";
	case CURLINFO_HEADER_OUT:
		return "HEADER_OUT";
	case CURLINFO_DATA_OUT:
		return "DATA_OUT";
	case CURLINFO_SSL_DATA_OUT:
		return "SSL_DATA_OUT";
	case CURLINFO_HEADER_IN:
		return "HEADER_IN";
	case CURLINFO_DATA_IN:
		return "DATA_IN";
	case CURLINFO_SSL_DATA_IN:
		return "SSL_DATA_IN";
	case CURLINFO_END:
		break;
	}

	return "N/A";
}

/*static*/ int CPlugin::CurlDebugFunction(CURL* pHandle, curl_infotype type, char* pData, size_t size, void* pUser)
{
	static CryStackStringT<char, 1> prettyPrintData;

	CPlugin* pThis = reinterpret_cast<CPlugin*>(pUser);
	if (type == CURLINFO_TEXT)
	{
		if (pThis->m_logVerbosity >= 4)
		{
			prettyPrintData = pData;
			prettyPrintData.TrimRight('\n');       // Remove extra newline coming from cURL
			CryLog("[cURL][%s][%p] %s", CurlStrInfoType(type), pHandle, prettyPrintData.c_str());
		}
	}
	else if (pThis->m_logVerbosity >= 5)
	{
		FormatHexDump(prettyPrintData, pData, size);
		CryComment("[cURL][%s][%p] Data:\n%s", CurlStrInfoType(type), pHandle, prettyPrintData.c_str());
	}
	else if (pThis->m_logVerbosity >= 4)
	{
		CryComment("[cURL][%s][%p] %zu bytes (0X%08zX)", CurlStrInfoType(type), pHandle, size, size);
	}

	return 0;
}

CPlugin::~CPlugin()
{
	for (CURL* pHandle : m_handles)
	{
		CRequest::OnCleanup(pHandle);
		curl_easy_cleanup(pHandle);
	}
	m_handles.clear();

	if (m_pMultiHandle != nullptr)
	{
		curl_multi_cleanup(m_pMultiHandle);
	}

#if !defined(RESOURCE_COMPILER)
	gEnv->pConsole->UnregisterVariable("http_timeout");
	gEnv->pConsole->UnregisterVariable("http_log_verbosity");
#endif
}

bool CPlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	if (!env.pTimer)
	{
		return false;
	}
	m_pTimer = env.pTimer;

	m_pMultiHandle = curl_multi_init();
	if (!m_pMultiHandle)
	{
		return false;
	}

#if !defined(RESOURCE_COMPILER)
	REGISTER_CVAR2("http_timeout", &m_requestTimeOutInSeconds, m_requestTimeOutInSeconds, VF_NULL, "Timeout in seconds for HTTP requests");
	REGISTER_CVAR2("http_log_verbosity", &m_logVerbosity, m_logVerbosity, VF_NULL, "Log Http internal messages (default: 3). 0 = Off, 1 = Errors, 2 = Warnings, 3 = Important, 4 = Information, 5 = Verbose");

	EnableUpdate(EUpdateStep::MainUpdate, true);
#endif
	return true;
}

void CPlugin::MainUpdate(float frameTime)
{
	ProcessRequests();
}

RequestPtr CPlugin::CreateRequest()
{
	if (CURL* pEasy = curl_easy_init())
	{
		if (m_logVerbosity > 0)
		{
			curl_easy_setopt(pEasy, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(pEasy, CURLOPT_DEBUGDATA, this);
			curl_easy_setopt(pEasy, CURLOPT_DEBUGFUNCTION, &CPlugin::CurlDebugFunction);
		}

		m_handles.emplace(pEasy);
		return new CRequest(m_pMultiHandle, pEasy, m_pTimer);
	}

	return nullptr;
}

CPlugin::EUpdateResult CPlugin::ProcessRequests()
{
	if (m_handles.empty())
	{
		return EUpdateResult::Idle;
	}

	int numRemainingRequests = 1;
	curl_multi_perform(m_pMultiHandle, &numRemainingRequests);

	// Now check the status of our requests
	int numRemainingMessages;
	while (CURLMsg* pMessage = curl_multi_info_read(m_pMultiHandle, &numRemainingMessages))
	{
		CRequest::OnMsg(*pMessage);
	}

	// Check for timeouts
	for (auto it = m_handles.begin(); it != m_handles.end();)
	{
		const float lastTimeHandled = CRequest::GetLastTimeHandled(*it);

		if (lastTimeHandled < 0.0f || m_pTimer->GetAsyncCurTime() - lastTimeHandled > m_requestTimeOutInSeconds)
		{
			CRequest::OnCleanup(*it);
			curl_easy_cleanup(*it);
			it = m_handles.erase(it);
		}
		else
		{
			++it;
		}
	}

	return numRemainingRequests != 0 ? EUpdateResult::ProcessingRequests : EUpdateResult::Idle;
}

CRYREGISTER_SINGLETON_CLASS(CPlugin)

}

#include <CryCore/CrtDebugStats.h>
