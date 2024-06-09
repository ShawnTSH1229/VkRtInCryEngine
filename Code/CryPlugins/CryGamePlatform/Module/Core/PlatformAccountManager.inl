// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

namespace Cry
{
	namespace GamePlatform
	{
		template<typename TAccount>
		bool CAccountManager<TAccount>::IsFriendWith(const AccountIdentifier& otherAccountId) const
		{
			return stl::has_value_if(m_friends, [&otherAccountId](const IAccount* pAcc) { return pAcc->GetIdentifier() == otherAccountId; });
		}

		template<typename TAccount>
		IAccount* CAccountManager<TAccount>::GetAccountById(const AccountIdentifier& accountId) const
		{
			for (const std::unique_ptr<TAccount>& pAccount : m_accounts)
			{
				if (pAccount->GetIdentifier() == accountId)
				{
					return pAccount.get();
				}
			}

			return nullptr;
		}

		template<typename TAccount>
		void CAccountManager<TAccount>::Clear()
		{
			for (const std::unique_ptr<TAccount>& pRemovedAccount : m_accounts)
			{
				NotifyAccountRemoved(*pRemovedAccount);
			}

			m_accounts.clear();
			m_friends.clear();
		}

		template<typename TAccount>
		void CAccountManager<TAccount>::ClearFriends()
		{
			m_friends.clear();
		}

		template<typename TAccount>
		void CAccountManager<TAccount>::AddFriend(TAccount& account)
		{
			stl::push_back_unique(m_friends, &account);
		}

		template<typename TAccount>
		void CAccountManager<TAccount>::RemoveFriend(const AccountIdentifier& accountId)
		{
			const int idx = stl::find_index_if(m_friends, [accountId](const IAccount* pAccount) { return pAccount->GetIdentifier() == accountId; });
			if (idx >= 0)
			{
				m_friends.erase(m_friends.begin() + idx);
			}
		}

		template<typename TAccount>
		std::unique_ptr<TAccount> CAccountManager<TAccount>::RemoveAccount(const AccountIdentifier& accountId)
		{
			std::unique_ptr<TAccount> pRemovedAccount;
			for (auto pos = m_accounts.begin(); pos != m_accounts.end(); ++pos)
			{
				if ((*pos)->GetIdentifier() == accountId)
				{
					pRemovedAccount.swap(*pos);

					NotifyAccountRemoved(*pRemovedAccount);

					m_accounts.erase(pos);
					stl::find_and_erase(m_friends, pRemovedAccount.get());
					break;
				}
			}

			return pRemovedAccount;
		}

		template<typename TAccount>
		inline TAccount* CAccountManager<TAccount>::FindAccount(const AccountIdentifier& accountId) const
		{
			for (const std::unique_ptr<TAccount>& pAccount : m_accounts)
			{
				if (pAccount->GetIdentifier() == accountId)
				{
					return pAccount.get();
				}
			}

			return nullptr;
		}

		template<typename TAccount>
		void CAccountManager<TAccount>::NotifyAccountAdded(TAccount& account)
		{
			m_listeners.ForEach([&account](IListener* pListener)
			{
				pListener->OnAccountAdded(account);
			});
		}

		template<typename TAccount>
		void CAccountManager<TAccount>::NotifyAccountRemoved(TAccount& account)
		{
			m_listeners.ForEach([&account](IListener* pListener)
			{
				pListener->OnAccountRemoved(account);
			});
		}
	}
}