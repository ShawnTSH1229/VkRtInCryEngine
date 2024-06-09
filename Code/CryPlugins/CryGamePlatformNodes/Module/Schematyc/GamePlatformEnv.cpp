// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "GamePlatformEnv.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

namespace Cry::GamePlatform
{
void RegisterPlatformSignalComponent(Schematyc::IEnvRegistrar&);
#define DECL_REGISTER(NAMESPACE) namespace NAMESPACE { void RegisterElements(Schematyc::IEnvRegistrar&); }

DECL_REGISTER(_UserGeneratedContent)
DECL_REGISTER(UserGeneratedContentType)
DECL_REGISTER(UserGeneratedContentVisibility)
DECL_REGISTER(_AccountIdentifier)
DECL_REGISTER(AvatarSize)
DECL_REGISTER(Permission)
DECL_REGISTER(PermissionResult)
DECL_REGISTER(PersonaChangeFlags)
DECL_REGISTER(PrivacyPermission)
DECL_REGISTER(Relationship)
DECL_REGISTER(RichPresence)
DECL_REGISTER(RichPresenceTimerType)
DECL_REGISTER(UserInformationFlags)
DECL_REGISTER(CatalogSortOrder)
DECL_REGISTER(ProductType)
DECL_REGISTER(StoreItem)
DECL_REGISTER(StoreItemDetails)
DECL_REGISTER(FixedString)
DECL_REGISTER(InventoryItem)
DECL_REGISTER(_InventoryItemIdentifier)
DECL_REGISTER(_LeaderboardIdentifier)
DECL_REGISTER(LeaderboardRequest)
DECL_REGISTER(LeaderboardScoreType)
DECL_REGISTER(_LobbyIdentifier)
DECL_REGISTER(LobbyQueryComparator)
DECL_REGISTER(LobbyVisibility)
DECL_REGISTER(ConnectionStatus)
DECL_REGISTER(Dialog)
DECL_REGISTER(UserDialog)
DECL_REGISTER(LocalCache)
DECL_REGISTER(SharedCache)
DECL_REGISTER(_ApplicationIdentifier)
DECL_REGISTER(Platform)
DECL_REGISTER(_PlatformEventFlags)
DECL_REGISTER(_TransactionIdentifier)
DECL_REGISTER(Achievement)
DECL_REGISTER(StatDataType)
DECL_REGISTER(Statistic)
DECL_REGISTER(StatisticValue)
DECL_REGISTER(Accounts)
DECL_REGISTER(General)
DECL_REGISTER(Catalog)
DECL_REGISTER(Inventory)
DECL_REGISTER(Leaderboards)
DECL_REGISTER(Lobby)
DECL_REGISTER(Networking)
DECL_REGISTER(RemoteStorage)
DECL_REGISTER(Statistics)
DECL_REGISTER(Overlay)
DECL_REGISTER(UserGeneratedContent)

#undef DECL_REGISTER

void RegisterPlatformEnv(Schematyc::IEnvRegistrar& registrar)
{
	auto moduleScope = registrar.RootScope().Register(SCHEMATYC_MAKE_ENV_MODULE(g_gamePlatformGUID, "GamePlatform"));
	moduleScope.Register(SCHEMATYC_MAKE_ENV_MODULE(g_gamePlatformDataTypesGUID, "DataTypes"));
	moduleScope.Register(SCHEMATYC_MAKE_ENV_MODULE(g_gamePlatformServiceGUID, "Service"));

	RegisterPlatformSignalComponent(registrar);
#define CALL_REGISTER(NAMESPACE) NAMESPACE::RegisterElements(registrar)
	CALL_REGISTER(_UserGeneratedContent);
	CALL_REGISTER(UserGeneratedContentType);
	CALL_REGISTER(UserGeneratedContentVisibility);
	CALL_REGISTER(_AccountIdentifier);
	CALL_REGISTER(AvatarSize);
	CALL_REGISTER(Permission);
	CALL_REGISTER(PermissionResult);
	CALL_REGISTER(PersonaChangeFlags);
	CALL_REGISTER(PrivacyPermission);
	CALL_REGISTER(Relationship);
	CALL_REGISTER(RichPresence);
	CALL_REGISTER(RichPresenceTimerType);
	CALL_REGISTER(UserInformationFlags);
	CALL_REGISTER(CatalogSortOrder);
	CALL_REGISTER(ProductType);
	CALL_REGISTER(StoreItem);
	CALL_REGISTER(StoreItemDetails);
	CALL_REGISTER(FixedString);
	CALL_REGISTER(InventoryItem);
	CALL_REGISTER(_InventoryItemIdentifier);
	CALL_REGISTER(_LeaderboardIdentifier);
	CALL_REGISTER(LeaderboardRequest);
	CALL_REGISTER(LeaderboardScoreType);
	CALL_REGISTER(_LobbyIdentifier);
	CALL_REGISTER(LobbyQueryComparator);
	CALL_REGISTER(LobbyVisibility);
	CALL_REGISTER(ConnectionStatus);
	CALL_REGISTER(Dialog);
	CALL_REGISTER(UserDialog);
	CALL_REGISTER(LocalCache);
	CALL_REGISTER(SharedCache);
	CALL_REGISTER(_ApplicationIdentifier);
	CALL_REGISTER(Platform);
	CALL_REGISTER(_PlatformEventFlags);
	CALL_REGISTER(_TransactionIdentifier);
	CALL_REGISTER(Achievement);
	CALL_REGISTER(StatDataType);
	CALL_REGISTER(Statistic);
	CALL_REGISTER(StatisticValue);
	CALL_REGISTER(Accounts);
	CALL_REGISTER(General);
	CALL_REGISTER(Catalog);
	CALL_REGISTER(Inventory);
	CALL_REGISTER(Leaderboards);
	CALL_REGISTER(Lobby);
	CALL_REGISTER(Networking);
	CALL_REGISTER(RemoteStorage);
	CALL_REGISTER(Statistics);
	CALL_REGISTER(Overlay);
	CALL_REGISTER(UserGeneratedContent);
#undef CALL_REGISTER
}
} // namespace Cry::GamePlatform
