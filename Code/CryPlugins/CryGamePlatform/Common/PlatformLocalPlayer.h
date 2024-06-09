// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformService.h"

namespace Cry
{
	namespace GamePlatform
	{
		inline IService::EPermissionResult MakePermissionResult(bool permissionGranted) { return permissionGranted ? IService::EPermissionResult::Granted : IService::EPermissionResult::Denied; }

		class CLocalPlayer
		{
		public:
			using TAllowances = std::unordered_map<IService::EPermission, IService::EPermissionResult>;
			using TPrivacyPermissions = std::unordered_map<IService::EPrivacyPermission, IService::EPermissionResult>;
			using TPrivacyPermissionsMap = VectorMap<AccountIdentifier, TPrivacyPermissions>;
			using TCrossNeworkPermissionsMap = std::unordered_map<IService::ERelationship, TPrivacyPermissions>;

			CLocalPlayer();
			~CLocalPlayer();

			void SetAccount(IAccount* pAccount);
			IAccount* GetAccount() const;

			void SetAllowance(IService::EPermission permission, IService::EPermissionResult result);
			IService::EPermissionResult GetAllowance(IService::EPermission permission) const;

			const TPrivacyPermissionsMap& GetPrivacyPermissionsMap() const { return m_privacyPermissions; }

			void SetPrivacyPermission(const IAccount& account, IService::EPrivacyPermission permission, IService::EPermissionResult result);
			IService::EPermissionResult GetPrivacyPermission(const IAccount& account, IService::EPrivacyPermission permission) const;

			void SetCrossNetworkPermission(IService::ERelationship relationship, IService::EPrivacyPermission permission, IService::EPermissionResult result);
			IService::EPermissionResult GetCrossNetworkPermission(IService::ERelationship relationship, IService::EPrivacyPermission permission) const;

			void SetBlockedAccounts(DynArray<IAccount*> blockedAccounts);
			void SetMutedAccounts(DynArray<IAccount*> mutedAccounts);

			const DynArray<IAccount*>& GetBlockedAccounts();
			const DynArray<IAccount*>& GetMutedAccounts();

			void OnAccountRemoved(IAccount& account);
			void AddAccountToLocalSession(IAccount& account);
			void RemoveAccountFromLocalSession(IAccount& account);

			void OnPlatformSignedIn(IAccount& account);
			void OnPlatformNotSignedIn();

		private:
			IAccount* m_pAccount = nullptr;
			TAllowances m_allowances;
			TPrivacyPermissionsMap m_privacyPermissions;
			TCrossNeworkPermissionsMap m_crossNetworkPermissions;
			DynArray<IAccount*> m_blockedAccounts;
			DynArray<IAccount*> m_mutedAccounts;
		};
	}
}
