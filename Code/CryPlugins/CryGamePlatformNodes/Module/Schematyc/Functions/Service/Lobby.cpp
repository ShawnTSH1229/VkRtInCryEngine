// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <GamePlatformHelper.h>
#include <Types/Lobby.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <CryGamePlatform/Interface/IPlatformMatchmaking.h>
#include <CryGamePlatform/Interface/IPlatformLobby.h>

namespace Cry::GamePlatform::Lobby
{

namespace Detail
{

IUserLobby* GetLobby(const LobbyIdentifier& lobbyId)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			return pMatchmaking->GetLobbyById(lobbyId);
		}
	}

	return nullptr;
}

} // Detail

bool HostServer(const LobbyIdentifier& lobbyId, const Schematyc::CSharedString& level, bool isLocal)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		return pLobby->HostServer(level.c_str(), isLocal);
	}

	return false;
}

bool GetMemberLimit(const LobbyIdentifier& lobbyId, int& memberLimit)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		memberLimit = pLobby->GetMemberLimit();
		return true;
	}

	return false;
}

bool GetNumMembers(const LobbyIdentifier& lobbyId, int& numMembers)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		numMembers = pLobby->GetNumMembers();
		return true;
	}

	return false;
}

bool GetMemberAtIndex(const LobbyIdentifier& lobbyId, int index, AccountIdentifier& member)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		member = pLobby->GetMemberAtIndex(index);
		return true;
	}

	return false;
}

bool IsInServer(const LobbyIdentifier& lobbyId, bool& inServer)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		inServer = pLobby->IsInServer();
		return true;
	}

	return false;
}

bool Leave(const LobbyIdentifier& lobbyId)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		pLobby->Leave();
		return true;
	}

	return false;
}

bool GetOwner(const LobbyIdentifier& lobbyId, AccountIdentifier& accountId)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		accountId = pLobby->GetOwnerId();
		return true;
	}

	return false;
}

bool SendChatMessage(const LobbyIdentifier& lobbyId, const Schematyc::CSharedString& message)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		return pLobby->SendChatMessage(message.c_str());
	}

	return false;
}

bool ShowInviteDialog(const LobbyIdentifier& lobbyId)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		pLobby->ShowInviteDialog();
		return true;
	}

	return false;
}

bool InvitePlayers(const LobbyIdentifier& lobbyId, Schematyc::CAnyArrayPtr memberList)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		if (memberList && memberList->GetSize())
		{
			DynArray<AccountIdentifier> dynList;
			for (uint32 i = 0; i < memberList->GetSize(); i++)
			{
				dynList.push_back(std::move(Schematyc::DynamicCast<AccountIdentifier>((*memberList)[i])));
			}

			return pLobby->InvitePlayers(dynList);
		}
	}

	return false;
}

bool SetData(const LobbyIdentifier& lobbyId, const Schematyc::CSharedString& key, const Schematyc::CSharedString& data)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		return pLobby->SetData(key.c_str(), data.c_str());
	}

	return false;
}

bool GetData(const LobbyIdentifier& lobbyId, const Schematyc::CSharedString& key, Schematyc::CSharedString& data)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		if (const char* szData = pLobby->GetData(key.c_str()))
		{
			data = szData;
			return true;
		}
	}

	return false;
}

bool SetMemberData(const LobbyIdentifier& lobbyId, const Schematyc::CSharedString& key, const Schematyc::CSharedString& data)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		pLobby->SetMemberData(key.c_str(), data.c_str());
		return true;
	}

	return false;
}

bool GetMemberData(const LobbyIdentifier& lobbyId, const AccountIdentifier& account, const Schematyc::CSharedString& key, Schematyc::CSharedString& data)
{
	if (auto pLobby = Detail::GetLobby(lobbyId))
	{
		if (const char* szData = pLobby->GetMemberData(account, key.c_str()))
		{
			data = szData;
			return true;
		}
	}

	return false;
}

bool CreateLobby(const IUserLobby::EVisibility visibility, int maxMemberCount)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			return pMatchmaking->CreateLobby(visibility, maxMemberCount);
		}
	}

	return false;
}

bool GetUserLobby(const AccountIdentifier& account, LobbyIdentifier& lobbyId)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			if (IUserLobby* pLobby = pMatchmaking->GetUserLobby(account))
			{
				lobbyId = pLobby->GetIdentifier();
				return true;
			}
		}
	}

	return false;
}

bool JoinLobby(const LobbyIdentifier& lobbyId)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			pMatchmaking->JoinLobby(lobbyId);
			return true;
		}
	}

	return false;
}

bool AddLobbyQueryFilterString(const Schematyc::CSharedString& key, const Schematyc::CSharedString& value, const IUserLobby::EComparison comparison)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			pMatchmaking->AddLobbyQueryFilterString(key.c_str(), value.c_str(), comparison);
			return true;
		}
	}

	return false;
}

bool QueryLobbies()
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			pMatchmaking->QueryLobbies();
			return true;
		}
	}

	return false;
}

bool GetQueriedLobbyIdByIndex(int index, LobbyIdentifier& lobbyId)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pMatchmaking = pService->GetMatchmaking())
		{
			lobbyId = pMatchmaking->GetQueriedLobbyIdByIndex(index);
			return lobbyId != LobbyIdentifier();
		}
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{71B52316-1421-4633-9CB3-9C381DBBD6B8}"_cry_guid, "Lobby"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetMemberLimit, "{1FC01E7A-67AB-4CAB-80A8-15BA8D65D96C}"_cry_guid, "GetMemberLimit");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindOutput(2, 'mlim', "Member Limit");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetNumMembers, "{EF6134B0-3F10-446C-AEFC-54EECE43B28B}"_cry_guid, "GetNumMembers");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindOutput(2, 'nmem', "Member Count");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetMemberAtIndex, "{46ACB1FF-D630-4237-B811-C87B9D4F3E09}"_cry_guid, "GetMemberAtIndex");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInput(2, 'idx', "Index");
		pFunction->BindOutput(3, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsInServer, "{949F7D04-840F-4DF7-BCC2-9D188FCAF6DE}"_cry_guid, "IsInServer");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindOutput(2, 'insv', "In Server");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Leave, "{BA5F6397-A165-4D1F-8C32-036C9EB24A88}"_cry_guid, "Leave");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetOwner, "{362475FA-7E35-4998-B86F-AAA9BC3926D3}"_cry_guid, "GetOwner");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindOutput(2, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SendChatMessage, "{08FBD5B4-3199-4857-B0FE-1275C6D3EA37}"_cry_guid, "SendChatMessage");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInput(2, 'msg', "Message");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ShowInviteDialog, "{9651C857-F846-4CEA-8F21-5D8AB5DFA3FF}"_cry_guid, "ShowInviteDialog");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&InvitePlayers, "{F678F060-4BE9-4263-B6F8-F6F71117A150}"_cry_guid, "InvitePlayers");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInputArrayType<AccountIdentifier>(2, 'lst', "Array[Account]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetData, "{FEC56DA0-E3A3-4496-96BB-F4ABC34EBF34}"_cry_guid, "SetLobbyData");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInput(2, 'key', "Key Name");
		pFunction->BindInput(3, 'data', "Data");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetData, "{13D4F821-BDE1-468F-A697-C4C6DF270C7D}"_cry_guid, "GetLobbyData");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInput(2, 'key', "Key Name");
		pFunction->BindOutput(3, 'data', "Data");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetMemberData, "{31BDD38D-0B4E-492C-9899-23FAA9C0B8D4}"_cry_guid, "SetMemberData");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInput(2, 'key', "Key Name");
		pFunction->BindInput(3, 'data', "Data");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetMemberData, "{9923DB44-3E7B-4E0F-A1FB-19A94129BB28}"_cry_guid, "GetMemberData");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'loby', "Lobby");
		pFunction->BindInput(2, 'acc', "Account");
		pFunction->BindInput(3, 'key', "Key Name");
		pFunction->BindOutput(4, 'data', "Data");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CreateLobby, "{C96ADD19-C785-4992-83ED-1A12715CD46F}"_cry_guid, "CreateLobby");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'vis', "Visibility");
		pFunction->BindInput(2, 'maxm', "Max Members");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetUserLobby, "{FA560900-AAAF-4EBA-8D1F-24AAF663D98A}"_cry_guid, "GetUserLobby");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindOutput(2, 'lob', "Lobby");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&JoinLobby, "{395F8E01-8B4D-46F0-8506-5C49A23D3F50}"_cry_guid, "JoinLobby");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'lob', "Lobby");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&AddLobbyQueryFilterString, "{BFA1BF5B-C901-48E9-A627-F56AE02CF64E}"_cry_guid, "AddQueryLobbiesFilter");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'key', "Key");
		pFunction->BindInput(2, 'val', "Value");
		pFunction->BindInput(3, 'cmp', "Comparison Type");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&QueryLobbies, "{2B45D25C-03FD-4A29-AD8D-F9AE96BEF77B}"_cry_guid, "QueryLobbies");
		pFunction->BindOutput(0, 'succ', "Success");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetQueriedLobbyIdByIndex, "{E27359D6-4850-4703-A078-6B99EDD496AC}"_cry_guid, "GetQueriedLobbyIdByIndex");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'idx', "Index");
		pFunction->BindOutput(2, 'lob', "Lobby");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Lobby
