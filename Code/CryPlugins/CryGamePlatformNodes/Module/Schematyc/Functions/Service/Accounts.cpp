// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <Types/Service.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/Utils/AnyArray.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryGamePlatform/Interface/IPlatformAccount.h>
#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform::Accounts
{

bool GetNickname(const AccountIdentifier& accountId, Schematyc::CSharedString& nickname)
{
	bool success = false;
	if (IService* pService = Helper::GetService(accountId.Service()))
	{
		if (IAccount* pAccount = pService->GetAccountById(accountId))
		{
			nickname = pAccount->GetNickname();
			success = true;
		}
	}

	return success;
}

bool GetAvatar(const AccountIdentifier& accountId, EAvatarSize size, int32& textureId)
{
	bool success = false;
	if (IService* pService = Helper::GetService(accountId.Service()))
	{
		if (IAccount* pAccount = pService->GetAccountById(accountId))
		{
			if (auto pTexture = pAccount->GetAvatar(size))
			{
				textureId = pTexture->GetTextureID();
				success = true;
			}
		}
	}

	return success;
}

bool GetConnnectedAccounts(const AccountIdentifier& accountId, Schematyc::CAnyArrayPtr pArray)
{
	bool success = false;

	if (pArray)
	{
		if (IService* pService = Helper::GetService(accountId.Service()))
		{
			if (IAccount* pAccount = pService->GetAccountById(accountId))
			{
				pArray->Clear();

				const auto& typeDesc = Schematyc::GetTypeDesc<AccountIdentifier>();
				for (auto& accountId : pAccount->GetConnectedAccounts())
				{
					pArray->PushBack(Schematyc::CAnyConstRef(typeDesc, &accountId));
				}
				success = true;
			}
		}
	}

	return success;
}

bool IsLocal(const AccountIdentifier& accountId, bool& isLocal)
{
	bool success = false;
	if (IService* pService = Helper::GetService(accountId.Service()))
	{
		if (IAccount* pAccount = pService->GetAccountById(accountId))
		{
			isLocal = pAccount->IsLocal();
			success = true;
		}
	}

	return success;
}

bool GetLocalAccount(EGamePlatform platform, AccountIdentifier& account)
{
	if (auto pService = Helper::GetService(platform))
	{
		if (auto pAccount = pService->GetLocalAccount())
		{
			account = pAccount->GetIdentifier();
			return true;
		}
	}

	return false;
}

bool RequestUserInformation(const AccountIdentifier& accountId, const CEnumFlags<EUserInformationFlags> info)
{
	if (auto pService = Helper::GetService(accountId.Service()))
	{
		return pService->RequestUserInformation(accountId, info.Value());
	}

	return false;
}

bool GetFriendAccounts(EGamePlatform platform, Schematyc::CAnyArrayPtr pFriends)
{
	if (pFriends)
	{
		if (auto pService = Helper::GetService(platform))
		{
			pFriends->Clear();
			for (auto f : pService->GetFriendAccounts())
			{
				(*pFriends).PushBack(Schematyc::CAnyConstRef(f->GetIdentifier()));
			}
			return true;
		}
	}

	return false;
}

bool GetBlockedAccounts(EGamePlatform platform, Schematyc::CAnyArrayPtr pBlocked)
{
	if (pBlocked)
	{
		if (auto pService = Helper::GetService(platform))
		{
			pBlocked->Clear();
			for (auto f : pService->GetBlockedAccounts())
			{
				(*pBlocked).PushBack(Schematyc::CAnyConstRef(f->GetIdentifier()));
			}
			return true;
		}
	}

	return false;
}

bool GetMutedAccounts(EGamePlatform platform, Schematyc::CAnyArrayPtr pMuted)
{
	if (pMuted)
	{
		if (auto pService = Helper::GetService(platform))
		{
			pMuted->Clear();
			for (auto f : pService->GetMutedAccounts())
			{
				(*pMuted).PushBack(Schematyc::CAnyConstRef(f->GetIdentifier()));
			}
			return true;
		}
	}

	return false;
}

bool RefreshFriendAccounts(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		pService->RefreshFriendAccounts();
		return true;
	}

	return false;
}

bool RefreshBlockedAccounts(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		pService->RefreshBlockedAccounts();
		return true;
	}

	return false;
}

bool RefreshMutedAccounts(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		pService->RefreshMutedAccounts();
		return true;
	}

	return false;
}

bool RefreshPermissions(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		pService->RefreshPermissions();
		return true;
	}

	return false;
}

bool RefreshPrivacyPermissions(EGamePlatform platform, Schematyc::CAnyArrayPtr pTargetAccountIds)
{
	if (auto pService = Helper::GetService(platform))
	{
		if (pTargetAccountIds && pTargetAccountIds->GetSize())
		{
			DynArray<AccountIdentifier> dyn;
			for (int i = 0; i < pTargetAccountIds->GetSize(); i++)
			{
				dyn.push_back(Schematyc::DynamicCast<AccountIdentifier>((*pTargetAccountIds)[i]));
			}

			pService->RefreshPrivacyPermissions(&dyn);
		}
		else
		{
			pService->RefreshPrivacyPermissions(nullptr);
		}

		return true;
	}

	return false;
}

bool AddAccountToLocalSession(const AccountIdentifier& accountId)
{
	if (auto pService = Helper::GetService(accountId.Service()))
	{
		pService->AddAccountToLocalSession(accountId);
		return true;
	}

	return false;
}

bool RemoveAccountFromLocalSession(const AccountIdentifier& accountId)
{
	if (auto pService = Helper::GetService(accountId.Service()))
	{
		pService->RemoveAccountFromLocalSession(accountId);
		return true;
	}

	return false;
}

bool IsFriendWith(const AccountIdentifier& otherAccountId)
{
	if (auto pService = Helper::GetService(otherAccountId.Service()))
	{
		return pService->IsFriendWith(otherAccountId);
	}

	return false;
}

bool GetPermission(EGamePlatform platform, const IService::EPermission permission, IService::EPermissionResult& result)
{
	if (auto pService = Helper::GetService(platform))
	{
		result = pService->GetPermission(permission);
		return true;
	}

	return false;
}

bool GetPrivacyPermission(const AccountIdentifier& targetAccountId, const IService::EPrivacyPermission permission, const IService::ERelationship targetRelationship, IService::EPermissionResult& result)
{
	if (auto pService = Helper::GetService(targetAccountId.Service()))
	{
		result = pService->GetPrivacyPermission(targetAccountId, permission, targetRelationship);
		return true;
	}

	return false;
}

bool GetPresence(const AccountIdentifier& accountId, SRichPresence& presence)
{
	bool success = false;
	if (IService* pService = Helper::GetService(accountId.Service()))
	{
		if (IAccount* pAccount = pService->GetAccountById(accountId))
		{
			pAccount->GetPresence(presence);
			success = true;
		}
	}

	return success;
}

bool SetPresence(const AccountIdentifier& accountId, const SRichPresence& presence)
{
	bool success = false;
	if (IService* pService = Helper::GetService(accountId.Service()))
	{
		if (IAccount* pAccount = pService->GetAccountById(accountId))
		{
			pAccount->SetPresence(presence);
			success = true;
		}
	}

	return success;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{78C88FE5-38CD-4417-B917-89523149D055}"_cry_guid, "Accounts"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetNickname, "{43111F22-56F0-4D82-8EE1-B8D4E20FD0B9}"_cry_guid, "GetNickname");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindOutput(2, 'rp', "Nickname");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetAvatar, "{883D1F9B-D3A7-4EDB-BF06-3E4A1EC8C359}"_cry_guid, "GetAvatar");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindInput(2, 'size', "Avatar Size");
		pFunction->BindOutput(3, 'tid', "Texture Id");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetConnnectedAccounts, "{BF6374C7-7C01-44BA-AE51-DA560632FF22}"_cry_guid, "GetConnnectedAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'arr', "Array[Account]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsLocal, "{47F00560-B768-4DAD-963F-DAD1C236B2F0}"_cry_guid, "IsLocal");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindOutput(2, 'locl', "Is Local");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetLocalAccount, "{ED2C4137-008D-4C62-8464-A8836E2D1833}"_cry_guid, "GetLocalAccount");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindOutput(2, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RequestUserInformation, "{704D26A0-2863-445B-B789-DBF9C0DE407F}"_cry_guid, "RequestUserInformation");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindInput(2, 'usrf', "UserInfoFlags");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetFriendAccounts, "{BD60B721-97DA-485D-A85B-55911F4ABE4A}"_cry_guid, "GetFriendAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'arr', "Array[Account]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetBlockedAccounts, "{D09744AE-E07B-46E0-80A0-337BAFE25C5E}"_cry_guid, "GetBlockedAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'arr', "Array[Account]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetMutedAccounts, "{0C86BA5C-250E-4678-8AF8-EE3948680E8B}"_cry_guid, "GetMutedAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'arr', "Array[Account]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RefreshFriendAccounts, "{70BABB54-4C8F-4342-8775-2F213212DC72}"_cry_guid, "RefreshFriendAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RefreshBlockedAccounts, "{2D9CA4A6-78B3-49C7-8BE2-8F4E7EAFA010}"_cry_guid, "RefreshBlockedAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RefreshMutedAccounts, "{8BED77A0-BF03-4501-B786-2DAD11224AF1}"_cry_guid, "RefreshMutedAccounts");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RefreshPermissions, "{75F5293D-E51D-463C-A65C-AC00BD97217D}"_cry_guid, "RefreshPermissions");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RefreshPrivacyPermissions, "{E004CA33-A030-449E-9891-8689DD7B9685}"_cry_guid, "RefreshPrivacyPermissions");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'arr', "Array[Account]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&AddAccountToLocalSession, "{30A5AD0E-21BC-4EDB-8DED-CCD90885FB8D}"_cry_guid, "AddAccountToLocalSession");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&RemoveAccountFromLocalSession, "{62F4E825-C2AC-48CE-BB3D-883AFE8BA795}"_cry_guid, "RemoveAccountFromLocalSession");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsFriendWith, "{830C86E2-2CE2-4A0A-90F4-C055CA8586C7}"_cry_guid, "IsFriendWith");
		pFunction->BindOutput(0, 'succ', "Is Friend");
		pFunction->BindInput(1, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPermission, "{C8416BA1-DF21-422A-8E07-7E3376A94341}"_cry_guid, "GetPermission");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInput(2, 'pmsn', "Permission");
		pFunction->BindOutput(3, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPrivacyPermission, "{7CD02545-D709-4EB5-A8D2-6977B7880F18}"_cry_guid, "GetPrivacyPermission");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Target Account");
		pFunction->BindInput(2, 'ppmn', "Privacy Permission");
		pFunction->BindInput(3, 'trel', "Target Relationship");
		pFunction->BindOutput(4, 'res', "Result");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetPresence, "{BCE7E071-E823-40C4-A224-5CCDAE2252EF}"_cry_guid, "GetPresence");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindOutput(2, 'rp', "Rich Presence");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetPresence, "{49816BCF-3E33-476B-AE56-46B50DDB45F3}"_cry_guid, "SetPresence");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindInput(2, 'rp', "Rich Presence");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Accounts
