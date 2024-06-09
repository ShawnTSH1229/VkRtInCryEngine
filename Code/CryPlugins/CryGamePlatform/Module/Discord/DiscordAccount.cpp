// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "DiscordAccount.h"
#include "DiscordService.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			CAccount::CAccount(CService& discordService, const discord::User& user)
				: m_service(discordService)
			{
				m_pAvatars[static_cast<size_t>(EAvatarSize::Large)] = nullptr;
				m_pAvatars[static_cast<size_t>(EAvatarSize::Medium)] = nullptr;
				m_pAvatars[static_cast<size_t>(EAvatarSize::Small)] = nullptr;

				SetDiscordUser(user);
			}

			CAccount::~CAccount()
			{
			}

			const char* CAccount::GetNickname() const
			{
				return m_nickname.c_str();
			}

			AccountIdentifier CAccount::GetIdentifier() const
			{
				return CreateAccountIdentifier(m_id);
			}

			ServiceIdentifier CAccount::GetServiceIdentifier() const
			{
				return DiscordServiceID;
			}

			void CAccount::SetStatus(const char* szStatus)
			{
				SRichPresence presence{};
				presence.headline = szStatus;

				SetPresence(presence);
			}

			void CAccount::SetPresence(const SRichPresence& presence)
			{
				m_presence = presence;

				discord::Activity discordPresence{};

				// NOTE: Field names can be a bit misleading:
				// * 'details' is the first, and shortest, presence line
				// * 'state' is optional and provides additional information 
				discordPresence.SetDetails(m_presence.headline.c_str());
				discordPresence.SetState(m_presence.activity.c_str());
				discordPresence.GetParty().GetSize().SetCurrentSize(m_presence.partySize);
				discordPresence.GetParty().GetSize().SetMaxSize(m_presence.partyMax);

				if (m_presence.seconds > 0)
				{
					if (m_presence.countdownTimer == SRichPresence::ETimer::Remaining)
					{
						discordPresence.GetTimestamps().SetEnd(time(0) + m_presence.seconds);
					}
					else if (m_presence.countdownTimer == SRichPresence::ETimer::Elapsed)
					{
						discordPresence.GetTimestamps().SetStart(time(0) + m_presence.seconds);
					}
				}

				CryComment("[Discord][Account] Setting rich presence (first line): %s", m_presence.headline.c_str());
				if (!m_presence.activity.empty())
				{
					CryComment("[Discord][Account]\t\t\t\t\t\t(second line): %s (%d of %d)", m_presence.activity.c_str(), m_presence.partySize, m_presence.partyMax);
				}

				if (m_presence.seconds > 0)
				{
					CryComment("[Discord][Account]\t\t\t\t\t\t(timer): %" PRIi64, m_presence.seconds);
				}

				if (discord::ActivityManager* pMgr = Helper::ActivityMgr())
				{
					pMgr->UpdateActivity(discordPresence, [](discord::Result res) {
						if (res != discord::Result::Ok)
						{
							CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Service] UpdatePresence(): UpdateActivity() failed with error : %s", Helper::ErrorString(res));
						}
					});
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] SetPresence(): Could not access Activity Manager");
				}
			}

			void CAccount::GetPresence(SRichPresence& presence) const
			{
				presence = m_presence;
			}

			void CAccount::ClearPresence()
			{
				// Hack to fully clear rich presence. ClearActivity should be used when it has the right behavior
				if (!m_service.Initialize())
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] ClearPresence(): Failed to reinitialize core");
				}
			}

			const DynArray<Cry::GamePlatform::AccountIdentifier>& CAccount::GetConnectedAccounts() const
			{
				return m_connectedAccounts;
			}

			bool CAccount::IsLocal() const
			{
				return m_service.GetLocalAccount() == this;
			}

			_smart_ptr<ITexture> CAccount::GetAvatar(EAvatarSize size) const
			{
				const size_t avatarIdx = static_cast<size_t>(size);
				if (m_pAvatars[avatarIdx])
				{
					return m_pAvatars[avatarIdx];
				}

				if (discord::ImageManager* pMgr = Helper::ImageMgr())
				{
					constexpr bool refresh = false; // use cached data

					discord::ImageHandle handle{};
					handle.SetId(m_id);
					handle.SetType(discord::ImageType::User);
					handle.SetSize(Helper::GetAvatarSize(size));

					pMgr->Fetch(handle, refresh, std::bind(&CAccount::GetData, this, avatarIdx, std::placeholders::_1, std::placeholders::_2));
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] GetAvatar(): Could not access Image Manager");
				}

				return nullptr;
			}

			void CAccount::SetDiscordUser(const discord::User& user)
			{
				m_id = user.GetId();
				m_nickname = user.GetUsername();
				m_avatarHash = user.GetAvatar();
			}

			void CAccount::GetData(size_t index, discord::Result result, discord::ImageHandle handle) const
			{
				if (discord::ImageManager* pMgr = Helper::ImageMgr())
				{
					if (result == discord::Result::Ok)
					{
						discord::ImageDimensions dims{};
						result = pMgr->GetDimensions(handle, &dims);
						if (result == discord::Result::Ok)
						{
							std::vector<uint8_t> imageBuffer;
							const size_t imageSize = dims.GetWidth() * dims.GetHeight() * 4;
							imageBuffer.resize(imageSize);
							uint8_t* pData = imageBuffer.data();
							result = pMgr->GetData(handle, pData, imageBuffer.size());
							if (result == discord::Result::Ok)
							{
								ETEX_Format format = eTF_R8G8B8A8;
								int textureId = gEnv->pRenderer->UploadToVideoMemory(imageBuffer.data(), dims.GetWidth(), dims.GetHeight(), format, format, 1);
								m_pAvatars[index].reset(gEnv->pRenderer->EF_GetTextureByID(textureId));

								m_service.OnAvatarImageLoaded(GetIdentifier());
							}
							else
							{
								CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] GetData(): GetData() failed with error %s", Helper::ErrorString(result));
							}
						}
						else
						{
							CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] GetData(): GetDimensions() failed with error %s", Helper::ErrorString(result));
						}
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] GetData(): caller failed with error %s", Helper::ErrorString(result));
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "[Discord][Account] GetData(): Could not access Image Manager");
				}
			}
		}
	}
}