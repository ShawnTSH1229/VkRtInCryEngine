// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "PlatformLocalPlayer.h"
#include "IPlatformAccount.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Detail
		{
			const char* GetRelationshipString(IService::ERelationship value)
			{
				switch (value)
				{
				case IService::ERelationship::Default: return "Default";
				case IService::ERelationship::Friend: return "Friend";
				default: return "<Missing>";
				}
			}

			const char* GetPermissionString(IService::EPermission value)
			{
				switch (value)
				{
				case IService::EPermission::Communication: return "Communication";
				case IService::EPermission::Multiplayer: return "Multiplayer";
				case IService::EPermission::ViewProfiles: return "ViewProfiles";
				case IService::EPermission::WebBrowser: return "WebBrowser";
				case IService::EPermission::UserGeneratedContent: return "UserGeneratedContent";
				case IService::EPermission::MultiplayerSubscription: return "MultiplayerSubscription";
				case IService::EPermission::CrossNetworkPlay: return "CrossNetworkPlay";
				default: return "<Missing>";
				}
			}

			const char* GetPrivacyPermissionString(IService::EPrivacyPermission value)
			{
				switch (value)
				{
				case IService::EPrivacyPermission::VoiceCommunication: return "VoiceCommunication";
				case IService::EPrivacyPermission::TextCommunication: return "TextCommunication";
				case IService::EPrivacyPermission::PlayMultiplayer: return "PlayMultiplayer";
				default: return "<Missing>";
				}
			}

			const char* GetPermissionResultString(IService::EPermissionResult value)
			{
				switch (value)
				{
				case IService::EPermissionResult::NotApplicable: return "NotApplicable";
				case IService::EPermissionResult::Pending: return "Pending";
				case IService::EPermissionResult::Granted: return "Granted";
				case IService::EPermissionResult::Denied: return "Denied";
				default: return "<Missing>";
				}
			}

			void Log(const char* szFormat, ...) PRINTF_PARAMS(1, 2);
			void Log(const char* szFormat, ...)
			{
#if !defined(EXCLUDE_NORMAL_LOG)
				va_list args;
				va_start(args, szFormat);
				stack_string format("[GamePlatform] ");
				format.append(szFormat);
				gEnv->pLog->LogV(ILog::eMessage, format.c_str(), args);
				va_end(args);
#endif
			}
		}

		CLocalPlayer::CLocalPlayer()
		{
		}

		CLocalPlayer::~CLocalPlayer()
		{
		}

		void CLocalPlayer::SetAccount(IAccount* pAccount)
		{
			m_pAccount = pAccount;
		}

		IAccount* CLocalPlayer::GetAccount() const
		{
			return m_pAccount;
		}

		void CLocalPlayer::SetAllowance(IService::EPermission permission, IService::EPermissionResult result)
		{
			IService::EPermissionResult& curResult = m_allowances[permission];

			Detail::Log("[CLocalPlayer::SetAllowance] permission:%s, curValue:%s, newValue:%s",
				Detail::GetPermissionString(permission),
				Detail::GetPermissionResultString(curResult),
				Detail::GetPermissionResultString(result));

			curResult = result;
		}

		IService::EPermissionResult CLocalPlayer::GetAllowance(IService::EPermission permission) const
		{
			const TAllowances::const_iterator it = m_allowances.find(permission);
			if (it != std::end(m_allowances))
			{
				return it->second;
			}
			return IService::EPermissionResult::NotApplicable;
		}

		void CLocalPlayer::SetPrivacyPermission(const IAccount& account, IService::EPrivacyPermission permission, IService::EPermissionResult result)
		{
			IService::EPermissionResult& curResult = m_privacyPermissions[account.GetIdentifier()][permission];

			Detail::Log("[CLocalPlayer::SetPrivacyPermission] account:%s, permission:%s, curValue:%s, newValue:%s",
				account.GetNickname(),
				Detail::GetPrivacyPermissionString(permission),
				Detail::GetPermissionResultString(curResult),
				Detail::GetPermissionResultString(result));

			curResult = result;
		}

		IService::EPermissionResult CLocalPlayer::GetPrivacyPermission(const IAccount& account, IService::EPrivacyPermission permission) const
		{
			const TPrivacyPermissionsMap::const_iterator parentIt = m_privacyPermissions.find(account.GetIdentifier());
			if (parentIt != std::end(m_privacyPermissions))
			{
				const TPrivacyPermissions& childMap = parentIt->second;
				const TPrivacyPermissions::const_iterator childIt = childMap.find(permission);
				if (childIt != std::end(childMap))
				{
					return childIt->second;
				}
			}
			return IService::EPermissionResult::NotApplicable;
		}

		void CLocalPlayer::SetCrossNetworkPermission(IService::ERelationship relationship, IService::EPrivacyPermission permission, IService::EPermissionResult result)
		{
			IService::EPermissionResult& curResult = m_crossNetworkPermissions[relationship][permission];

			Detail::Log("[CLocalPlayer::SetCrossNetworkPermission] relationship:%s, permission:%s, curValue:%s, newValue:%s",
				Detail::GetRelationshipString(relationship),
				Detail::GetPrivacyPermissionString(permission),
				Detail::GetPermissionResultString(curResult),
				Detail::GetPermissionResultString(result));

			curResult = result;
		}

		IService::EPermissionResult CLocalPlayer::GetCrossNetworkPermission(IService::ERelationship relationship, IService::EPrivacyPermission permission) const
		{
			const TCrossNeworkPermissionsMap::const_iterator parentIt = m_crossNetworkPermissions.find(relationship);
			if (parentIt != std::end(m_crossNetworkPermissions))
			{
				const TPrivacyPermissions& childMap = parentIt->second;
				const TPrivacyPermissions::const_iterator childIt = childMap.find(permission);
				if (childIt != std::end(childMap))
				{
					return childIt->second;
				}
			}
			return IService::EPermissionResult::NotApplicable;
		}

		void CLocalPlayer::SetBlockedAccounts(DynArray<IAccount*> blockedAccounts)
		{
			m_blockedAccounts = std::move(blockedAccounts);
		}

		void CLocalPlayer::SetMutedAccounts(DynArray<IAccount*> mutedAccounts)
		{
			m_mutedAccounts = std::move(mutedAccounts);
		}

		const DynArray<IAccount*>& CLocalPlayer::GetBlockedAccounts()
		{
			return m_blockedAccounts;
		}

		const DynArray<IAccount*>& CLocalPlayer::GetMutedAccounts()
		{
			return m_mutedAccounts;
		}

		void CLocalPlayer::OnAccountRemoved(IAccount& account)
		{
			if (m_pAccount == &account)
			{
				m_pAccount = nullptr;
			}
			else
			{
				stl::find_and_erase(m_blockedAccounts, &account);
				stl::find_and_erase(m_mutedAccounts, &account);
				stl::member_find_and_erase(m_privacyPermissions, account.GetIdentifier());
			}
		}

		void CLocalPlayer::AddAccountToLocalSession(IAccount& account)
		{
			m_privacyPermissions.insert(std::make_pair<>(account.GetIdentifier(), TPrivacyPermissions()));
		}

		void CLocalPlayer::RemoveAccountFromLocalSession(IAccount& account)
		{
			if (m_pAccount != &account)
			{
				stl::member_find_and_erase(m_privacyPermissions, account.GetIdentifier());
			}
		}

		void CLocalPlayer::OnPlatformSignedIn(IAccount& account)
		{
			m_pAccount = &account;
		}

		void CLocalPlayer::OnPlatformNotSignedIn()
		{
			m_pAccount = nullptr;
		}
	}
}
