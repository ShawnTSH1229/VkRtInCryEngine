// Copyright 2020-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

namespace Cry
{
	namespace Http
	{
		//! Subset of HTTP Request Methods the plugin aims to support
		//! \sa RFC 7231 section 4: Hypertext Transfer Protocol (HTTP/1.1): Semantics and Content - Request methods 
		//! \sa RFC 5789 section 2: PATCH Method for HTTP - Patch method
		enum class EMethod
		{
			Post,  //! POST
			Get,   //! GET
			Put,   //! PUT
			Patch, //! PATCH
			Delete //! DELETE
		};

		//! Inspired by Windows::Foundation::AsyncStatus
		enum class ERequestState
		{
			Started,   //! The operation has started.
			Completed, //! The operation has completed.
			Canceled,  //! The operation was canceled.
			Error,     //! The operation has encountered an error.
		};
		
		//! Common errors that can be encountered while performing requests
		enum class ERequestError
		{
			None = 0,
			Aborted,
			AccessDenied,
			BadPac,           //! Bad Proxy Auto-Configuration script
			BadProxyUrl,
			BadScheme,
			BadUrl,
			BadCert,          //! Generic certificate issue
			BadCertCn,        //! Bad certificate common name
			BadCertRoot,
			BadCertUsage,
			ExpiredCert,
			FailCertRev,      //! Failure in checking certificate revocation
			RevokedCert,
			NeedClientCert,
			BadClientCert,
			FailSecChannel,   //! Generic failure in secure channel
			FailIO,           //! Generic I/O failure
			FailLogin,
			FailRedirect,
			InternalError,
			InvalidArgument,
			InvalidOperation,
			InvalidResponse,
			NoNetwork,
			NotFound,         //! Generic DNS failure
			NotImplemented,
			NotSupported,
			OutOfMemory,
			Pending,
			ResetConn,
			ShuttingDown,
			Timeout
		};

		//! Common HTTP status codes
		//! \sa The "Hypertext Transfer Protocol (HTTP) Status Code Registry" by IANA
		enum class EStatusCode
		{
			// 1xx: Informational response 
			Continue = 100,                      //! OK to continue with request
			SwitchProtocols = 101,               //! server has switched protocols in upgrade header
			Processing = 102,                    //! server has received and is processing the request, but no response is available yet
			EarlyHints = 103,                    //! used to return some response headers before final HTTP message

			// 2xx: Success
			OK = 200,                            //! request completed
			Created = 201,                       //! object created, reason = new URI
			Accepted = 202,                      //! async completion (TBS)
			NonAuthoritativeInformation = 203,   //! a transforming proxy (e.g. Web accelerator) received a 200 OK from its origin, but is returning a modified response
			NoContent = 204,                     //! no info to return
			ResetContent = 205,                  //! request completed, but clear form
			PartialContent = 206,                //! server is delivering only part of the resource (byte serving) due to a range header sent by the client.
			MultiStatus = 207,                   //! response is an XML message and can contain a number of separate response codes, depending on how many sub-requests were made
			AlreadyReported = 208,               //! members of a DAV binding already enumerated in a preceding part of the (multistatus) response, and are not being included again 
			InstanceManipulationUsed = 226,      //! server has fulfilled a request for the resource, and the response is a the result of one or more instance-manipulations

			// 3xx: Redirection
			MultipleChoices = 300,               //! server couldn't decide what to return
			MovedPermanently = 301,              //! object permanently moved
			Redirect = 302,                      //! object temporarily moved
			SeeOther = 303,                      //! redirection w/ new access method
			NotModified = 304,                   //! if-modified-since was not modified
			UseProxy = 305,                      //! redirection to proxy, location header specifies proxy to use
			SwitchProxy = 306,                   //! legacy: "Subsequent requests should use the specified proxy."
			TemporaryRedirect = 307,             //! request should be repeated with another URI; however, future requests should still use the original URI
			PermanentRedirect = 308,             //! this and all future requests should be repeated using another URI
			
			// 4xx: Client errors
			BadRequest = 400,                    //! server cannot or will not process the request due to an apparent client error
			Unauthorized = 401,                  //! similar Forbidden, but specifically for use when authentication is required and has failed or has not yet been provided
			PaymentRequired = 402,               //! payment required
			Forbidden = 403,                     //! request was valid, but the server is refusing action. The user might lack permissions or credentials
			NotFound = 404,                      //! object not found
			MethodNotAllowed = 405,              //! method is not allowed
			NotAcceptable = 406,                 //! no response acceptable to client found
			ProxyAuthRequired = 407,             //! proxy authentication required
			RequestTimeout = 408,                //! server timed out waiting for request
			Conflict = 409,                      //! user should resubmit with more info
			Gone = 410,                          //! the resource is no longer available
			LengthRequired = 411,                //! the server refused to accept request w/o a length
			PrecondFailed = 412,                 //! precondition given in request failed
			PayloadTooLarge = 413,               //! request entity was too large
			UriTooLong = 414,                    //! request URI too long
			UnsupportedMedia = 415,              //! unsupported media type
			RangeNotSatisfiable = 416,           //! client has asked for a portion of the file (byte serving), but the server cannot supply that portion
			ExpectationFailed = 417,             //! server cannot meet the requirements of the Expect request-header field
			MisdirectedRequest = 421,            //! request was directed at a server that is not able to produce a response (for example because of connection reuse)
			UnprocessableEntity = 422,           //! the request was well-formed but was unable to be followed due to semantic errors
			Locked = 423,                        //! the resource that is being accessed is locked
			FailedDependency = 424,              //! request failed because it depended on another request and that request failed (e.g PROPPATCH)
			UpgradeRequired = 426,               //! client should switch to a different protocol such as TLS / 1.0, given in the Upgrade header field
			PreconditionRequired = 428,          //! the request has to be conditional. For preventing conflicts in concurrent GET/SET of same resource
			TooManyRequests = 429,               //! user has sent too many requests in a given amount of time. Intended for use with rate - limiting schemes
			HeaderTooLarge = 431,                //! either an individual header field, or all the header fields collectively, are too large
			LoginTimedOut = 440,                 //! the client's session has expired and must log in again
			RetryWith = 449,                     //! retry after doing the appropriate action.
			BlockedByParentalControls = 450,     //! Parental Controls are blocking access to the requested resource
			UnavailableForLegalReasons = 451,    //! server operator has received a legal demand to deny access to the resource
			
			// 5xx: Server errors
			InternalServerError = 500,           //! internal server error
			NotImplemented = 501,                //! required not supported
			BadGateway = 502,                    //! error response received from gateway
			ServiceUnavailable = 503,            //! temporarily overloaded
			GatewayTimeout = 504,                //! timed out waiting for gateway
			VersionNotSupported = 505,           //! HTTP version not supported
			VariantAlsoNegotiates = 506,         //! transparent content negotiation for the request results in a circular reference
			InsufficientStorage = 507,           //! server is unable to store the representation needed to complete the request
			LoopDetected = 508,                  //! server detected an infinite loop while processing the request
			BandwidthLimitExceeded = 509,        //! server has exceeded the bandwidth specified by the server administrator
			NotExtended = 510,                   //! further extensions to the request are required for the server to fulfill it
			NetworkAuthenticationRequired = 511, //! client needs to authenticate to gain network access (e.g. public Wi-Fi)
			InvalidCertificate = 526,            //! failure to validate the SSL/TLS certificate that the origin server presented
			NetworkReadTimeout = 598             //! used by some HTTP proxies to signal a network read timeout behind the proxy to a client in front of the proxy
		};

		inline const char* GetErrorString(ERequestError error)
		{
			switch (error)
			{
				case ERequestError::None:
					return "No error";
				case ERequestError::Aborted:
					return "Operation was aborted by user canceling or prematurely releasing a required resource";
				case ERequestError::AccessDenied:
					return "Access to requested resource was denied";
				case ERequestError::BadCert:
					return "The server certificate is not valid";
				case ERequestError::BadCertCn:
					return "The server certificate's CN does not match the expected value";
				case ERequestError::BadCertRoot:
					return "The RootCA that issued the server certificate is not trusted";
				case ERequestError::BadCertUsage:
					return "The server certificate is not valid for the requested usage";
				case ERequestError::ExpiredCert:
					return "The server certificate is not within its validity period when verifying against the current system clock or the timestamp in the signed file";
				case ERequestError::FailCertRev:
					return "Revocation of the server certificate cannot be checked, possibly because the revocation server is offline";
				case ERequestError::RevokedCert:
					return "The certificate or signature has been revoked";
				case ERequestError::NeedClientCert:
					return "The server requires SSL client authentication";
				case ERequestError::BadClientCert:
					return "The client certificate is not valid or does not have a private key associated with it";
				case ERequestError::BadPac:
					return "The Proxy Auto-Configuration (PAC) script could not be downloaded, or an error occurred while attempting to execute it";
				case ERequestError::BadProxyUrl:
					return "The proxy URL is not in a valid format or could not be resolved";
				case ERequestError::BadScheme:
					return "The provided URL scheme is not supported by the underlying implementation (use HTTP or HTTPS)";
				case ERequestError::BadUrl:
					return "The provided URL was not properly formatted";
				case ERequestError::FailLogin:
					return "Login attempt failed";
				case ERequestError::FailRedirect:
					return "Redirection failed because of scheme change or too many redirects";
				case ERequestError::FailSecChannel:
					return "Failure with the secure communication channel";
				case ERequestError::FailIO:
					return "I/O failure while sending/receiving network data or while writing to local file";
				case ERequestError::InternalError:
					return "An internal error was triggered by the underlying implementation. It could be a bug in the plugin or in the underlying implementation";
				case ERequestError::InvalidArgument:
					return "An invalid argument was provided to the underlying implementation. Can be caused by incorrect usage of the plugin's API or by a bug";
				case ERequestError::InvalidOperation:
					return "An invalid operation was performed on the underlying implementation. Can be caused by incorrect usage of the plugin's API or by a bug";
				case ERequestError::InvalidResponse:
					return "An invalid response was received from server. E.g. header is too big or not recognized, unexpected/unsupported encoding of body, content size mismatch...";
				case ERequestError::NoNetwork:
					return "The network is unavailable or unreachable";
				case ERequestError::Pending:
					return "An operation in the underlying implementation is still pending. If encountered on request failure, it's a plugin bug";
				case ERequestError::NotFound:
					return "Couldn't resolve or reach host/address";
				case ERequestError::NotImplemented:
					return "Operation or feature is not implemented by server or underlying implementation. It could be a bug in the plugin or in the underlying implementation";
				case ERequestError::NotSupported:
					return "Operation or feature is not supported by server or underlying implementation. It could be a bug in the plugin or in the underlying implementation";
				case ERequestError::OutOfMemory:
					return "The underlying implementation is out of stack, heap or buffer memory. It can be triggered by a response exceeding an internal limit";
				case ERequestError::ResetConn:
					return "The connection was lost due to a reset or unexpected termination";
				case ERequestError::ShuttingDown:
					return "A service or system is shutting down and cannot be used anymore";
				case ERequestError::Timeout:
					return "A request or operation has timed out";
			}

			return "Unknown ERequestError value";
		}
	}
}
