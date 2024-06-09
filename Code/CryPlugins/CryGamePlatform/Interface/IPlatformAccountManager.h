// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "PlatformTypes.h"

namespace Cry
{
	namespace GamePlatform
	{
		//! Listener for account manager changes
		//! See IAccountManager::AddListener and RemoveListener
		struct IAccountManagerListener
		{
			virtual ~IAccountManagerListener() = default;
			//! Called when an account has been added
			virtual void OnAccountAdded(IAccount& account) {}
			//! Called right before removing an account
			virtual void OnAccountRemoved(IAccount& account) {}
		};

		//! Manager for known accounts and friends of local account
		struct IAccountManager
		{
			using IListener = IAccountManagerListener;

			virtual ~IAccountManager() = default;

			//! Adds a service event listener
			virtual void AddListener(IListener& listener, const char* szName) = 0;
			//! Removes a service event listener
			virtual void RemoveListener(IListener& listener) = 0;
			//! Gets local user's friend accounts
			virtual const DynArray<IAccount*>& GetFriends() const = 0;
			//! Checks if the local user has the other account in their friends list for this service
			virtual bool IsFriendWith(const AccountIdentifier& otherAccountId) const = 0;
			//! Gets an IAccount representation of another account by account id
			virtual IAccount* GetAccountById(const AccountIdentifier& accountId) const = 0;
		};
	}
}