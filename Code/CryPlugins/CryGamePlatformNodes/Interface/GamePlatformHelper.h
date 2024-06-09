// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>
#include <Types/StatisticValue.h>
#include <Utils/PluginUtils.h>
#include <CryGamePlatform/Interface/IGamePlatform.h>
#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform::Helper
{

inline IService* GetMainService()
{
	if (IPlugin* pPlugin = CPluginHelper<IPlugin>::Get())
	{
		return pPlugin->GetMainService();
	}

	return nullptr;
}

inline IService* GetService(const ServiceIdentifier& serviceId)
{
	if (IPlugin* pPlugin = CPluginHelper<IPlugin>::Get())
	{
		return pPlugin->GetService(serviceId);
	}

	return nullptr;
}

inline IService* GetService(EGamePlatform platform)
{

	switch (platform)
	{
	case EGamePlatform::Main:
		return GetMainService();
	case EGamePlatform::Discord:
		return GetService(DiscordServiceID);
	case EGamePlatform::Steam:
		return GetService(SteamServiceID);
	case EGamePlatform::Playstation:
		return GetService(PSNServiceID);
	case EGamePlatform::Xbox:
		return GetService(XboxServiceID);
	}

	return nullptr;
}

inline EGamePlatform GetServiceEnum(const char* szName)
{
	if (stricmp(szName, "Discord") == 0) return EGamePlatform::Discord;
	else if (stricmp(szName, "Steam") == 0) return EGamePlatform::Steam;
	else if (stricmp(szName, "Playstation") == 0) return EGamePlatform::Playstation;
	else if (stricmp(szName, "Xbox") == 0) return EGamePlatform::Xbox;
	else return EGamePlatform::None;
}

inline EGamePlatform GetServiceEnum(const ServiceIdentifier& serviceId)
{
	if (serviceId == DiscordServiceID) return EGamePlatform::Discord;
	else if (serviceId == SteamServiceID) return EGamePlatform::Steam;
	else if (serviceId == PSNServiceID) return EGamePlatform::Playstation;
	else if (serviceId == XboxServiceID) return EGamePlatform::Xbox;
	else return EGamePlatform::None;
}

inline const char* GetServiceName(EGamePlatform platform)
{
	switch (platform)
	{
	case EGamePlatform::None:
		return "None";
	case EGamePlatform::Main:
		if (IService* pMainService = GetMainService())
			return GetServiceName(GetServiceEnum(pMainService->GetServiceIdentifier()));
		else
			return "Main: Unknown";
	case EGamePlatform::Discord:
		return "Discord";
	case EGamePlatform::Steam:
		return "Steam";
	case EGamePlatform::Playstation:
		return "Playstation";
	case EGamePlatform::Xbox:
		return "Xbox";
	}

	return nullptr;
}

inline bool IsMatchingService(EGamePlatform lhs, EGamePlatform rhs)
{
	EGamePlatform main = GetMainService()? GetServiceEnum(GetMainService()->GetServiceIdentifier()) : EGamePlatform::Main;

	const EGamePlatform left = lhs == EGamePlatform::Main ? main : lhs;
	const EGamePlatform right = rhs == EGamePlatform::Main ? main : rhs;

	return left == right;
}

inline const char* StatTypeToName(EStatDataType dataType)
{
	switch (dataType)
	{
	default:
		return "None";
	case EStatDataType::Float:
		return "Float";
	case EStatDataType::Integer:
		return "Integer";
	}
}

inline EStatDataType StatTypeFromName(const char* szDataType)
{
	if (stricmp(szDataType, "Float") == 0)
		return EStatDataType::Float;

	if (stricmp(szDataType, "Integer") == 0)
		return EStatDataType::Integer;

	return EStatDataType::None;
}

} // Cry::GamePlatform::Helper