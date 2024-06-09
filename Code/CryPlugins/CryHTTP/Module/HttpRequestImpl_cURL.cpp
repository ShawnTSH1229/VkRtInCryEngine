// Copyright 2020-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "HttpRequestImpl_cURL.h"

#include <CrySystem/ISystem.h>
#include <CrySystem/ITimer.h>

namespace Cry::Http::cURL
{

CRequest::CRequest(CURLM* pMulti, CURL* pEasy, ITimer* pTimer) : m_pMulti(pMulti)
	, m_pHandle(pEasy)
	, m_lastTimeHandled(pTimer->GetAsyncCurTime())
	, m_pTimer(pTimer)
{
	curl_easy_setopt(m_pHandle, CURLOPT_WRITEFUNCTION, CurlWriteResponse);
	curl_easy_setopt(m_pHandle, CURLOPT_XFERINFOFUNCTION, CurlTransferInfo);
	curl_easy_setopt(m_pHandle, CURLOPT_PRIVATE, this);
	curl_easy_setopt(m_pHandle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_pHandle, CURLOPT_ERRORBUFFER, m_errbuf);
}

CRequest::~CRequest()
{
	Cleanup();
}

void CRequest::Cleanup()
{
	if (m_pHandle)
	{
		curl_easy_setopt(m_pHandle, CURLOPT_WRITEFUNCTION, nullptr);
		curl_easy_setopt(m_pHandle, CURLOPT_PRIVATE, nullptr);
		curl_easy_setopt(m_pHandle, CURLOPT_WRITEDATA, nullptr);
		curl_easy_setopt(m_pHandle, CURLOPT_ERRORBUFFER, nullptr);
	}

	if (m_pMulti != nullptr && m_pHandle != nullptr)
	{
		curl_multi_remove_handle(m_pMulti, m_pHandle);
	}
	m_pMulti = nullptr;
	m_pHandle = nullptr;

	if (m_pHeaderList != nullptr)
	{
		curl_slist_free_all(m_pHeaderList);
		m_pHeaderList = nullptr;
	}
}

/*static*/ CRequest* CRequest::GetRequest(CURL* pHandle)
{
	char* pPrivate = nullptr;
	const CURLcode res = curl_easy_getinfo(pHandle, CURLINFO_PRIVATE, &pPrivate);

	if (res == CURLE_OK && pPrivate)
	{
		return reinterpret_cast<CRequest*>(pPrivate);
	}

	return nullptr;
}

/*static*/ size_t CRequest::CurlWriteResponse(char* pBuffer, size_t size, size_t nitems, void* pUserData)
{
	if (CRequest* pThis = reinterpret_cast<CRequest*>(pUserData))
	{
		return pThis->WriteResponse(pBuffer, size * nitems);
	}

	return 0;
}

/*static*/ int CRequest::CurlTransferInfo(void* pUserData, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	if (CRequest* pThis = reinterpret_cast<CRequest*>(pUserData))
	{
		pThis->m_lastTimeHandled = pThis->m_pTimer->GetAsyncCurTime();
	}

	return CURLE_OK;
}

void CRequest::SetRequestMethod(EMethod m)
{
	if (m_pHandle)
	{
		CURLcode code = curl_easy_setopt(m_pHandle, CURLOPT_POST, (m == EMethod::Post || m == EMethod::Delete) ? 1 : 0);
		if (ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_POST)"))
		{
			switch (m)
			{
			case EMethod::Delete:
				code = curl_easy_setopt(m_pHandle, CURLOPT_CUSTOMREQUEST, "DELETE");
				break;
			case EMethod::Patch:
				code = curl_easy_setopt(m_pHandle, CURLOPT_CUSTOMREQUEST, "PATCH");
				break;
			case EMethod::Put:
				code = curl_easy_setopt(m_pHandle, CURLOPT_CUSTOMREQUEST, "PUT");
				break;
			case EMethod::Get:
				code = curl_easy_setopt(m_pHandle, CURLOPT_CUSTOMREQUEST, "GET");
				break;
			case EMethod::Post:
				code = curl_easy_setopt(m_pHandle, CURLOPT_CUSTOMREQUEST, "POST");
				break;
			default:
				code = curl_easy_setopt(m_pHandle, CURLOPT_CUSTOMREQUEST, nullptr);
			}

			ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_CUSTOMREQUEST)");
		}
	}
}

void CRequest::SetRequestUrl(const char* szUrl)
{
	if (m_pHandle)
	{
		m_url = szUrl;       // cURL doesn't copy the string
		const CURLcode code = curl_easy_setopt(m_pHandle, CURLOPT_URL, m_url.c_str());
		ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_URL)");
	}
}

bool CRequest::SetRequestBody(const uint8* pBuffer, size_t bufferSize)
{
	if (m_pHandle)
	{
		CURLcode code = curl_easy_setopt(m_pHandle, CURLOPT_POSTFIELDSIZE, bufferSize);
		if (ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_POSTFIELDSIZE)"))
		{
			code = curl_easy_setopt(m_pHandle, CURLOPT_COPYPOSTFIELDS, pBuffer);
			return ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_COPYPOSTFIELDS)");
		}
	}

	return false;
}

bool CRequest::SetTrustAllCertificates(bool trustAll)
{
	if (m_pHandle)
	{
		const CURLcode code = curl_easy_setopt(m_pHandle, CURLOPT_SSL_VERIFYPEER, trustAll ? 0 : 1);
		return ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_SSL_VERIFYPEER)");
	}

	return false;
}

bool CRequest::AddRequestHeader(const char* szKey, const char* szValue)
{
	const size_t keyLen = strlen(szKey);
	const size_t valueLen = strlen(szValue);

	if (keyLen > 0 && valueLen > 0)
	{
		CryStackStringT<char, 1> headerStr;

		headerStr.reserve(keyLen + valueLen + 2);
		headerStr.append(szKey, keyLen);
		headerStr.append(": ");
		headerStr.append(szValue, valueLen);

		m_pHeaderList = curl_slist_append(m_pHeaderList, headerStr.c_str());
		if (!m_pHeaderList)
		{
			return ProcessErrorCode(CURLE_OUT_OF_MEMORY, "curl_slist_append");
		}
	}

	if (m_pHandle)
	{
		const CURLcode code = curl_easy_setopt(m_pHandle, CURLOPT_HTTPHEADER, m_pHeaderList);
		return ProcessErrorCode(code, "curl_easy_setopt(CURLOPT_HTTPHEADER)");
	}

	return false;
}

bool CRequest::Start()
{
	if (m_pMulti != nullptr && m_pHandle != nullptr)
	{
		const CURLMcode res = curl_multi_add_handle(m_pMulti, m_pHandle);
		if (ProcessErrorCode(res, "curl_multi_add_handle"))
		{
			m_lastTimeHandled = m_pTimer->GetAsyncCurTime();
			m_state = ERequestState::Started;
			return true;
		}
	}

	return false;
}

void CRequest::Cancel()
{
	if (m_state == ERequestState::Started)
	{
		if (m_pMulti != nullptr && m_pHandle != nullptr)
		{
			curl_multi_remove_handle(m_pMulti, m_pHandle);
			m_state = ERequestState::Canceled;
		}
	}
}

EStatusCode CRequest::GetResponseStatusCode() const
{
	long status = 0;
	if (m_pHandle)
	{
		const CURLcode code = curl_easy_getinfo(m_pHandle, CURLINFO_RESPONSE_CODE, &status);
		ProcessErrorCode(code, "curl_easy_getinfo(CURLINFO_RESPONSE_CODE)");
	}

	return static_cast<EStatusCode>(status);
}

/*static*/ void CRequest::OnMsg(CURLMsg& message)
{
	if (message.msg == CURLMSG_DONE)
	{
		if (CRequest* pRequest = GetRequest(message.easy_handle))
		{
			const CURLcode errCode = message.data.result;
			if (errCode == CURLE_OK)
			{
				pRequest->m_state = ERequestState::Completed;
			}
			else
			{
				pRequest->m_state = ERequestState::Error;
				pRequest->ProcessErrorCode(errCode, "CURLMsg::data::result");
			}
		}
	}
}

/*static*/ void CRequest::OnCleanup(CURL* pHandle)
{
	if (CRequest* pRequest = GetRequest(pHandle))
	{
		pRequest->Cleanup();
	}
}

/*static*/ float CRequest::GetLastTimeHandled(CURL* pHandle)
{
	if (CRequest* pRequest = GetRequest(pHandle))
	{
		return pRequest->GetLastTimeHandled();
	}

	return -1.0f;
}

size_t CRequest::WriteResponse(const char* pBuffer, size_t bytes)
{
	m_lastTimeHandled = m_pTimer->GetAsyncCurTime();

	if (m_response.empty())
	{
		double dataSize = 0.0;
		curl_easy_getinfo(m_pHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dataSize);
		if (dataSize > 0.0)
		{
			m_response.reserve(static_cast<size_t>(std::ceil(dataSize)));
		}
	}

	const size_t prevResponseSize = m_response.size();

	m_response.resize(prevResponseSize + bytes);
	m_response.copy(prevResponseSize, reinterpret_cast<const uint8*>(pBuffer), bytes);

	return bytes;
}

float CRequest::GetLastTimeHandled() const
{
	if (m_pHandle)
	{
		if (GetState() == ERequestState::Started)
		{
			return m_lastTimeHandled;
		}

		return m_pTimer->GetAsyncCurTime();
	}

	return -1.0f;
}

bool CRequest::ProcessErrorCode(CURLcode code, const char* szOffender) const
{
	const ERequestError lastError = ToRequestError(code);
	if (lastError != ERequestError::None)
	{
		m_lastError = lastError;
		CryWarning(VALIDATOR_MODULE_ONLINE, VALIDATOR_ERROR, "[Http] %s failed with error %d : %s\n%s", szOffender, static_cast<int>(code), curl_easy_strerror(code), m_errbuf);
	}

	return code == CURLE_OK;
}

bool CRequest::ProcessErrorCode(CURLMcode code, const char* szOffender) const
{
	const ERequestError lastError = ToRequestError(code);
	if (lastError != ERequestError::None)
	{
		m_lastError = lastError;
		CryWarning(VALIDATOR_MODULE_ONLINE, VALIDATOR_ERROR, "[Http] %s failed with error %d : %s\n%s", szOffender, static_cast<int>(code), curl_multi_strerror(code), m_errbuf);
	}

	return code == CURLM_OK;
}

/*static*/ ERequestError CRequest::ToRequestError(CURLcode errCode)
{
	switch (errCode)
	{
	case CURLE_NO_CONNECTION_AVAILABLE:                      // Never returned by libcurl. No connection available, the session will be queued.
	case CURLE_HTTP_RETURNED_ERROR:                          // Returned for some specific status codes, we consider request done anyway in those cases
	case CURLE_GOT_NOTHING:                                  // Empty reply, we consider request done anyway in those cases
	case CURLE_OK:
		return ERequestError::None;

	case CURLE_UNSUPPORTED_PROTOCOL:                         // Protocol is not supported. The support might be a compile-time option or not provided by libcurl
		return ERequestError::BadScheme;

	case CURLE_URL_MALFORMAT:                                // The URL was not properly formatted
		return ERequestError::BadUrl;

	case CURLE_COULDNT_RESOLVE_PROXY:                        // Couldn't resolve proxy. The given proxy host could not be resolved
		return ERequestError::BadProxyUrl;

	case CURLE_AGAIN:                                        // Socket is not ready for send/recv wait till it's ready and try again
		return ERequestError::Pending;

	case CURLE_OPERATION_TIMEDOUT:                           // Operation timeout. The specified time-out period was reached according to the conditions
		return ERequestError::Timeout;

	case CURLE_ABORTED_BY_CALLBACK:                          // Aborted by callback. A callback returned "abort" to libcurl
		return ERequestError::Aborted;

	case CURLE_BAD_FUNCTION_ARGUMENT:                        // A function was called with a bad parameter
		return ERequestError::InvalidArgument;

	case CURLE_TOO_MANY_REDIRECTS:                           // Too many redirects. When following redirects, libcurl hit the maximum amount. Set your limit with CURLOPT_MAXREDIRS
		return ERequestError::FailRedirect;

	case CURLE_COULDNT_RESOLVE_HOST:                         // Couldn't resolve host. The given remote host was not resolved
	case CURLE_COULDNT_CONNECT:                              // Failed to connect() to host or proxy
		return ERequestError::NotFound;

	case CURLE_FILESIZE_EXCEEDED:                            // Maximum file size exceeded
	case CURLE_OUT_OF_MEMORY:                                // A memory allocation request failed. This is serious badness and things are severely screwed up if this ever occurs.
		return ERequestError::OutOfMemory;

	case CURLE_SSL_CACERT_BADFILE:                           // Problem with reading the SSL CA cert (path? access rights?)
	case CURLE_SSL_CACERT:
	case CURLE_PEER_FAILED_VERIFICATION:                     // The remote server's SSL certificate or SSH md5 fingerprint was deemed not OK. Equivalent to CURLE_SSL_CACERT
	case CURLE_SSL_ISSUER_ERROR:                             // Issuer check failed
		return ERequestError::BadCert;

	case CURLE_SSL_CONNECT_ERROR:                            // A problem occurred somewhere in the SSL/TLS handshake. Check error buffer for more info
		return ERequestError::FailSecChannel;

	case CURLE_FUNCTION_NOT_FOUND:                           // E.g. Attempting to use a cURL feature that was compiled out
	case CURLE_NOT_BUILT_IN:
		return ERequestError::NotImplemented;

	case CURLE_WEIRD_SERVER_REPLY:                           // E.g. server replies with body data when previously said it wouldn't
	case CURLE_PARTIAL_FILE:                                 // A file transfer was shorter or larger than reported in content size
	case CURLE_BAD_CONTENT_ENCODING:                         // Unrecognized transfer encoding
		return ERequestError::InvalidResponse;

	case CURLE_SEND_ERROR:                                   // Failed sending network data
	case CURLE_WRITE_ERROR:                                  // An error occurred when writing received data to a local file, or an error was returned to libcurl from a write callback
	case CURLE_SEND_FAIL_REWIND:                             // When doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed
	case CURLE_READ_ERROR:                                   // curl_seek_callback() behaved unexpectedly
	case CURLE_RECV_ERROR:                                   // Failure with receiving network data
		return ERequestError::FailIO;

	case CURLE_FAILED_INIT:                                  // Very early initialization code failed
		return ERequestError::InternalError;

	default:
		break;
	}

	return ERequestError::InternalError;
}

/*static*/ ERequestError CRequest::ToRequestError(CURLMcode errCode)
{
	switch (errCode)
	{
	case CURLM_OK:
	case CURLM_CALL_MULTI_PERFORM:
		return ERequestError::None;

	case CURLM_BAD_EASY_HANDLE:                              // An easy handle was not good/valid
	case CURLM_BAD_HANDLE:                                   // The passed-in handle is not a valid CURLM handle
	case CURLM_BAD_SOCKET:                                   // The passed in socket argument did not match
	case CURLM_UNKNOWN_OPTION:                               // curl_multi_setopt() with unsupported option
		return ERequestError::InvalidArgument;

	case CURLM_ADDED_ALREADY:                                // An easy handle already added to a multi handle was attempted to get added - again
		return ERequestError::InvalidOperation;

	case CURLM_OUT_OF_MEMORY:
		return ERequestError::OutOfMemory;

	case CURLM_INTERNAL_ERROR:
		return ERequestError::InternalError;

	case CURLM_LAST:
		break;
	}

	return ERequestError::InternalError;
}
}
