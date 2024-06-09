// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "PlatformTypes.h"
#include <steam/steam_api.h>

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			class CMatchmaking;
			class CNetworking;
			class CServer;
			class CUserLobby;
			class CAccount;
			class CAchievement;
			class CLeaderboards;
			class CStatistics;
			class CUser;
			class CRemoteFile;
			class CRemoteStorage;
			class CSharedRemoteFile;
			class CUserGeneratedContent;
			class CUserGeneratedContentManager;
			class CService;

			using AccountIdentifierValue = Cry::GamePlatform::Detail::NumericIdentifierValue;
			using LobbyIdentifierValue = Cry::GamePlatform::Detail::NumericIdentifierValue;
			using ApplicationIdentifierValue = Cry::GamePlatform::Detail::NumericIdentifierValue;
			using InventoryItemIdentifierValue = Cry::GamePlatform::Detail::NumericIdentifierValue;
			using LeaderboardIdentifierValue = Cry::GamePlatform::Detail::StringIdentifierValue;

			inline AccountIdentifier       CreateAccountIdentifier(AccountIdentifierValue rawSteamId)              { return AccountIdentifier(SteamServiceID, rawSteamId); }
			inline LobbyIdentifier         CreateLobbyIdentifier(LobbyIdentifierValue rawSteamId)                  { return LobbyIdentifier(SteamServiceID, rawSteamId); }
			inline ApplicationIdentifier   CreateApplicationIdentifier(ApplicationIdentifierValue rawSteamId)      { return ApplicationIdentifier(SteamServiceID, rawSteamId); }
			inline InventoryItemIdentifier CreateInventoryItemIdentifier(InventoryItemIdentifierValue rawSteamId)  { return InventoryItemIdentifier(SteamServiceID, rawSteamId); }
			inline LeaderboardIdentifier   CreateLeaderboardIdentifier(LeaderboardIdentifierValue leaderboardName) { return LeaderboardIdentifier(SteamServiceID, leaderboardName); }

			inline AccountIdentifier     CreateAccountIdentifier(const CSteamID& steamId)                   { return CreateAccountIdentifier(steamId.ConvertToUint64()); }
			inline LobbyIdentifier       CreateLobbyIdentifier(const CSteamID& steamId)                     { return CreateLobbyIdentifier(steamId.ConvertToUint64()); }

			inline string ExtractLeaderboardName(const LeaderboardIdentifier& leaderboardId)
			{
				string name;
				leaderboardId.GetAsString(name);
				return name;
			}

			inline CEnumFlags<EPersonaChangeFlags> ToPersonaChangeEnumFlags(int personaChange)
			{
				CEnumFlags<EPersonaChangeFlags> enumFlags;

				if (personaChange & k_EPersonaChangeName)
					enumFlags.Add(EPersonaChangeFlags::Name);
				if (personaChange & k_EPersonaChangeStatus)
					enumFlags.Add(EPersonaChangeFlags::Status);
				if (personaChange & k_EPersonaChangeComeOnline)
					enumFlags.Add(EPersonaChangeFlags::CameOnline);
				if (personaChange & k_EPersonaChangeGoneOffline)
					enumFlags.Add(EPersonaChangeFlags::WentOffline);
				if (personaChange & k_EPersonaChangeGamePlayed)
					enumFlags.Add(EPersonaChangeFlags::GamePlayed);
				if (personaChange & k_EPersonaChangeGameServer)
					enumFlags.Add(EPersonaChangeFlags::GameServer);
				if (personaChange & k_EPersonaChangeAvatar)
					enumFlags.Add(EPersonaChangeFlags::ChangeAvatar);
				if (personaChange & k_EPersonaChangeJoinedSource)
					enumFlags.Add(EPersonaChangeFlags::JoinedSource);
				if (personaChange & k_EPersonaChangeLeftSource)
					enumFlags.Add(EPersonaChangeFlags::LeftSource);
				if (personaChange & k_EPersonaChangeRelationshipChanged)
					enumFlags.Add(EPersonaChangeFlags::RelationshipChanged);
				if (personaChange & k_EPersonaChangeNameFirstSet)
					enumFlags.Add(EPersonaChangeFlags::NameFirstSet);
				if (personaChange & k_EPersonaChangeBroadcast)
					enumFlags.Add(EPersonaChangeFlags::FacebookInfo);
				if (personaChange & k_EPersonaChangeNickname)
					enumFlags.Add(EPersonaChangeFlags::Nickname);
				if (personaChange & k_EPersonaChangeSteamLevel)
					enumFlags.Add(EPersonaChangeFlags::SteamLevel);
				if (personaChange & k_EPersonaChangeRichPresence)
					enumFlags.Add(EPersonaChangeFlags::RichPresence);

				CRY_ASSERT_MESSAGE(personaChange < 0x8000, "Not all bits consumed");

				return enumFlags;
			}
		}
	}
}
