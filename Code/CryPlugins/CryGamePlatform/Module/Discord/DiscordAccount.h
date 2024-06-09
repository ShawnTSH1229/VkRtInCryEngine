// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformAccount.h"
#include "DiscordHelper.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			class CService;

			class CAccount
				: public IAccount
			{
			public:
				CAccount() = delete;
				explicit CAccount(CService& discordService, const discord::User& user);
				virtual ~CAccount();

				// IAccount
				virtual const char* GetNickname() const override;
				virtual AccountIdentifier GetIdentifier() const override;
				virtual ServiceIdentifier GetServiceIdentifier() const override;
				virtual void SetStatus(const char* szStatus) override;
				virtual void SetPresence(const SRichPresence& presence) override;
				virtual void GetPresence(SRichPresence& presence) const override;
				virtual void ClearPresence() override;
				virtual _smart_ptr<ITexture> GetAvatar(EAvatarSize size) const override;
				virtual const DynArray<AccountIdentifier>& GetConnectedAccounts() const override;
				virtual bool IsLocal() const override;
				// ~IAccount

				const discord::UserId& GetDiscordID() const { return m_id; }
				void SetDiscordUser(const discord::User& user);

			private:
				void GetData(size_t index, discord::Result result, discord::ImageHandle handle) const;

			private:
				CService& m_service;

			protected:
				discord::UserId m_id;
				string m_nickname;
				string m_avatarHash;
				string m_partyId;
				string m_joinId;
				string m_matchId;
				string m_spectateId;

				SRichPresence m_presence;

				DynArray<AccountIdentifier> m_connectedAccounts;
				mutable _smart_ptr<ITexture> m_pAvatars[3]; // Indexed by EAvatarSize
			};
		}
	}
}