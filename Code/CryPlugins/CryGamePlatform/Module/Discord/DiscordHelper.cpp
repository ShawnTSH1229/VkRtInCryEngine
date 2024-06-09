// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "DiscordHelper.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			discord::Core* Helper::s_pInstance = nullptr;

			const char* Helper::ErrorString(discord::Result code)
			{
				switch (code)
				{
				case discord::Result::Ok:
					return "Ok";
				case discord::Result::ServiceUnavailable:
					return "ServiceUnavailable";
				case discord::Result::InvalidVersion:
					return "InvalidVersion";
				case discord::Result::LockFailed:
					return "LockFailed";
				case discord::Result::InternalError:
					return "InternalError";
				case discord::Result::InvalidPayload:
					return "InvalidPayload";
				case discord::Result::InvalidCommand:
					return "InvalidCommand";
				case discord::Result::InvalidPermissions:
					return "InvalidPermissions";
				case discord::Result::NotFetched:
					return "NotFetched";
				case discord::Result::NotFound:
					return "NotFound";
				case discord::Result::Conflict:
					return "Conflict";
				case discord::Result::InvalidSecret:
					return "InvalidSecret";
				case discord::Result::InvalidJoinSecret:
					return "InvalidJoinSecret";
				case discord::Result::NoEligibleActivity:
					return "NoEligibleActivity";
				case discord::Result::InvalidInvite:
					return "InvalidInvite";
				case discord::Result::NotAuthenticated:
					return "NotAuthenticated";
				case discord::Result::InvalidAccessToken:
					return "InvalidAccessToken";
				case discord::Result::ApplicationMismatch:
					return "ApplicationMismatch";
				case discord::Result::InvalidDataUrl:
					return "InvalidDataUrl";
				case discord::Result::InvalidBase64:
					return "InvalidBase64";
				case discord::Result::NotFiltered:
					return "NotFiltered";
				case discord::Result::LobbyFull:
					return "LobbyFull";
				case discord::Result::InvalidLobbySecret:
					return "InvalidLobbySecret";
				case discord::Result::InvalidFilename:
					return "InvalidFilename";
				case discord::Result::InvalidFileSize:
					return "InvalidFileSize";
				case discord::Result::InvalidEntitlement:
					return "InvalidEntitlement";
				case discord::Result::NotInstalled:
					return "NotInstalled";
				case discord::Result::NotRunning:
					return "NotRunning";
				case discord::Result::InsufficientBuffer:
					return "InsufficientBuffer";
				case discord::Result::PurchaseCanceled:
					return "PurchaseCanceled";
				case discord::Result::InvalidGuild:
					return "InvalidGuild";
				case discord::Result::InvalidEvent:
					return "InvalidEvent";
				case discord::Result::InvalidChannel:
					return "InvalidChannel";
				case discord::Result::InvalidOrigin:
					return "InvalidOrigin";
				case discord::Result::RateLimited:
					return "RateLimited";
				case discord::Result::OAuth2Error:
					return "OAuth2Error";
				case discord::Result::SelectChannelTimeout:
					return "SelectChannelTimeout";
				case discord::Result::GetGuildTimeout:
					return "GetGuildTimeout";
				case discord::Result::SelectVoiceForceRequired:
					return "SelectVoiceForceRequired";
				case discord::Result::CaptureShortcutAlreadyListening:
					return "CaptureShortcutAlreadyListening";
				case discord::Result::UnauthorizedForAchievement:
					return "UnauthorizedForAchievement";
				case discord::Result::InvalidGiftCode:
					return "InvalidGiftCode";
				case discord::Result::PurchaseError:
					return "PurchaseError";
				case discord::Result::TransactionAborted:
					return "TransactionAborted";
				default:
					return "Unknown Result";
				}
			}

			void Helper::OnLogMessage(discord::LogLevel level, char const* szMessage)
			{
				EValidatorSeverity severity = VALIDATOR_COMMENT;
				switch (level)
				{
				case discord::LogLevel::Error:
					severity = VALIDATOR_ERROR;
					break;
				case discord::LogLevel::Warn:
					severity = VALIDATOR_WARNING;
					break;
				case discord::LogLevel::Info:
				case discord::LogLevel::Debug:
					severity = VALIDATOR_COMMENT;
					break;
				}

				CryWarning(VALIDATOR_MODULE_SYSTEM, severity, "[Discord][Helper] SDK message : %s", szMessage);
			}

			bool Helper::Initialize(discord::ClientId gameId, bool requireDiscord)
			{
				if (s_pInstance)
				{
					Helper::Finalize();
				}

				const std::uint64_t flags = static_cast<std::uint64_t>(requireDiscord ? discord::CreateFlags::Default : discord::CreateFlags::NoRequireDiscord);
				const discord::Result res = discord::Core::Create(gameId, flags, &s_pInstance);
				if (res != discord::Result::Ok)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Helper] Initialize() failed with error : %s", ErrorString(res));
					return false;
				}

				if (s_pInstance)
				{
					s_pInstance->SetLogHook(discord::LogLevel::Debug, Helper::OnLogMessage);
				}

				return s_pInstance != nullptr;
			}

			void Helper::Finalize()
			{
				if (s_pInstance)
				{
					delete s_pInstance;
					s_pInstance = nullptr;
				}
			}

			bool Helper::RunCallbacks()
			{
				if (s_pInstance)
				{
					const discord::Result res = s_pInstance->RunCallbacks();
					if (res == discord::Result::Ok)
					{
						return true;
					}
					WarnOnce("[Helper] %s returned error : %s", __FUNC__, ErrorString(res));
				}
				return false;
			}

			bool Helper::GetCurrentUser(discord::User& outCurrentUser)
			{
				if (discord::UserManager* pMgr = UserMgr())
				{
					const discord::Result result = pMgr->GetCurrentUser(&outCurrentUser);
					if (result != discord::Result::Ok)
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Helper] GetCurrentUser(): GetCurrentUser() failed with error %s", ErrorString(result));
					}
					return result == discord::Result::Ok;
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Helper] GetCurrentUser(): Could not access User Manager");
				}
				return false;
			}

			// Size must be a power of 2
			uint32_t Helper::GetAvatarSize(const EAvatarSize& size)
			{
				switch (size)
				{
				case EAvatarSize::Large:
					return 128;
				case EAvatarSize::Medium:
					return 64;
				case EAvatarSize::Small:
					return 32;
				default:
					return 0;
				}
			}
		}
	}
}
