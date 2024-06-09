// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "IPlatformAccountManager.h"
#include "IPlatformAccount.h"

namespace Cry
{
	namespace GamePlatform
	{
		template<typename TAccount>
		class CAccountManager : public IAccountManager
		{
		public:
			static_assert(std::is_base_of<IAccount, TAccount>::value, "Only subclasses of IAccount are allowed as types");

			// IAccountManager
			virtual void AddListener(IListener& listener, const char* szName) override { m_listeners.Add(&listener, szName); }
			virtual void RemoveListener(IListener& listener) override { m_listeners.Remove(&listener); }
			virtual const DynArray<IAccount*>& GetFriends() const override { return m_friends; }
			virtual bool IsFriendWith(const AccountIdentifier& otherAccountId) const override;
			virtual IAccount* GetAccountById(const AccountIdentifier& accountId) const override;
			// ~IAccountManager

			void Clear();
			void ClearFriends();
			
			template<class... TArgs>
			TAccount* AddAccount(TArgs&&... args)
			{
				std::unique_ptr<TAccount> newAccount = stl::make_unique<TAccount>(std::forward<TArgs>(args)...);
				m_accounts.emplace_back(std::move(newAccount));
				TAccount* pAccount = m_accounts.back().get();
				NotifyAccountAdded(*pAccount);

				return pAccount;
			}

			template<class... TArgs>
			TAccount* AddFriend(const AccountIdentifier& accountId, TArgs&&... args)
			{
				TAccount* pFriend = FindAccount(accountId);
				if (pFriend == nullptr)
				{
					pFriend = AddAccount(std::forward<TArgs>(args)...);
					CRY_ASSERT_MESSAGE(pFriend->GetIdentifier() == accountId, "%s: Unexpected id mismatch during friend account creation : expected '%s' == '%s'", 
						__FUNC__, pFriend->GetIdentifier().ToDebugString(), accountId.ToDebugString());
				}

				stl::push_back_unique(m_friends, pFriend);

				return pFriend;
			}

			//! \note No check is made to ensure account is known
			void AddFriend(TAccount& account);
			void RemoveFriend(const AccountIdentifier& accountId);
			std::unique_ptr<TAccount> RemoveAccount(const AccountIdentifier& accountId);
			TAccount* FindAccount(const AccountIdentifier& accountId) const;

			template<typename TPredicate>
			TAccount* FindAccountIf(const TPredicate& predicate) const
			{
				for (const std::unique_ptr<TAccount>& pAcc : m_accounts)
				{
					if (predicate(*pAcc))
					{
						return pAcc.get();
					}
				}

				return nullptr;
			}

		private:
			void NotifyAccountAdded(TAccount& pAccount);
			void NotifyAccountRemoved(TAccount& pAccount);

			std::vector<std::unique_ptr<TAccount>> m_accounts;
			DynArray<IAccount*> m_friends;
			CListenerSet<IListener*> m_listeners = 4;
		};
	}
}

#include "PlatformAccountManager.inl"
