// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <GamePlatformHelper.h>
#include <IPlatformEventManager.h>
#include <IPlugin.h>
#include <Events/OnTextFiltered.h>
#include <Events/OnCheckMultiplayerAccess.h>
#include <Utils/PluginUtils.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformNetworking.h>

namespace Cry::GamePlatform::Networking
{
	
void CheckMultiplayerAccess()
{
	if (auto pService = Helper::GetMainService())
	{
		const EGamePlatform platform = Helper::GetServiceEnum(pService->GetServiceIdentifier());
		pService->CanAccessMultiplayerServices([&platform](bool bAuthorized) { 
			if (IPlatformEventManager* pEventManager = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetPlatformEventManager())
			{
				pEventManager->HandleEvent(COnCheckMultiplayerAccess(platform, bAuthorized));
			}
		});
	}
}

EConnectionStatus GetConnectionStatus()
{
	if (auto pService = Helper::GetMainService())
	{
		return pService->GetConnectionStatus();
	}

	return EConnectionStatus::Disconnected;
}

bool GetAuthToken(Schematyc::CSharedString& token, int& issuerId)
{
	if (auto pService = Helper::GetMainService())
	{
		string value;
		if (pService->GetAuthToken(value, issuerId))
		{
			token.assign(value.c_str());
			return true;
		}
	}

	return false;
}

bool SendPacket(const AccountIdentifier& remoteUser, const Schematyc::CSharedString& data)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pNetworking = pService->GetNetworking())
		{
			return pNetworking->SendPacket(remoteUser, static_cast<const void*>(data.c_str()), data.length());
		}
	}

	return false;
}

bool CloseSession(const AccountIdentifier& accountId)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pNetworking = pService->GetNetworking())
		{
			return pNetworking->CloseSession(accountId);
		}
	}

	return false;
}

bool IsPacketAvailable(uint32& packetSize)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pNetworking = pService->GetNetworking())
		{
			return pNetworking->IsPacketAvailable(&packetSize);
		}
	}

	return false;
}

bool ReadPacket(uint32 maxLength, Schematyc::CSharedString& data, AccountIdentifier& sender)
{
	bool success = false;

	if (auto pService = Helper::GetMainService())
	{
		if (auto pNetworking = pService->GetNetworking())
		{
			char* pInput = new char[maxLength + 1];
			pInput[maxLength] = 0;

			uint32 bytesRead = 0;
			success = pNetworking->ReadPacket(static_cast<void*>(pInput), maxLength, &bytesRead, &sender);

			if (success)
			{
				data.assign(pInput, 0, bytesRead);
			}
		}
	}

	return success;
}

bool IsLoggedIn(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		return pService->IsLoggedIn();
	}

	return false;
}

bool FilterText(EGamePlatform platform, const Schematyc::CSharedString& message)
{
	if (auto pService = Helper::GetService(platform))
	{
		return pService->FilterText(message.c_str(), [&platform](const char* szFiltered)
		{
			auto pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get();
			if (IPlatformEventManager* pEventManager = pPlugin->GetPlatformEventManager())
			{
				pEventManager->HandleEvent(COnTextFiltered(platform, szFiltered));
			}
		});
	}

	return false;
}

bool SetIsInMultiplayer(EGamePlatform platform, bool isInMultiplayer)
{
	if (auto pService = Helper::GetService(platform))
	{
		pService->SetIsInMultiplayer(isInMultiplayer);
		return true;
	}

	return false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{3485D47B-8763-4CC7-8ECE-5EA9120BBCE7}"_cry_guid, "Networking"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CheckMultiplayerAccess, "{19ED663E-5775-4B6E-BB2A-021DC1EAA3E9}"_cry_guid, "CheckMultiplayerAccess");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetConnectionStatus, "{147E583B-E148-4F92-97A4-3CF87ACC43A1}"_cry_guid, "GetConnectionStatus");
		pFunction->BindOutput(0, 'stat', "ConnectionStatus");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetAuthToken, "{9D314D58-BCC6-4C21-9F1D-51C45C7A4B3E}"_cry_guid, "GetAuthToken");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindOutput(1, 'tokn', "Token");
		pFunction->BindOutput(2, 'iid', "IssuerId");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SendPacket, "{540EA531-622B-48EC-AD30-E2FD22FFFA0C}"_cry_guid, "SendPacket");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		pFunction->BindInput(2, 'data', "Data");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CloseSession, "{2F1D1ECF-55A8-4095-B05F-79C4212ACAA2}"_cry_guid, "CloseSession");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsPacketAvailable, "{AB9C12F1-3387-4ACA-9CED-4F8B3BF3C8BD}"_cry_guid, "IsPacketAvailable");
		pFunction->BindOutput(0, 'succ', "Is Available");
		pFunction->BindOutput(1, 'sz', "Data Size");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ReadPacket, "{70DCD836-BB86-4A67-8BEF-D3945D3750EC}"_cry_guid, "ReadPacket");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'mlen', "Max Length");
		pFunction->BindOutput(2, 'data', "Packet Data");
		pFunction->BindOutput(3, 'acc', "Account");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsLoggedIn, "{3EF14B98-ED8A-49F7-9BA0-393A1E7974DB}"_cry_guid, "IsLoggedIn");
		pFunction->BindOutput(0, 'succ', "Logged In");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&FilterText, "{8E3D9E56-A14D-4EAB-B756-05FA67A55011}"_cry_guid, "FilterText");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInput(2, 'txt', "Message");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetIsInMultiplayer, "{E7E05227-5695-4FF2-8639-D9683DD1EFC6}"_cry_guid, "SetIsInMultiplayer");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'pfrm', "Platform");
		pFunction->BindInput(2, 'iim', "Is in Multiplayer");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::Networking
