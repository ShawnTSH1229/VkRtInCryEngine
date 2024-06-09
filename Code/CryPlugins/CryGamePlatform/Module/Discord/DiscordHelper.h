// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "DiscordTypes.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Discord
		{
			//! Collection of boilerplate code often used by the plugin to work with the SDK.
			class Helper
			{
			public:
				//! Initializes the SDK.
				//! \return Returns true on success.
				static bool Initialize(discord::ClientId gameId, bool requireDiscord);
				//! Shuts down the SDK and unloads the DLL.
				static void Finalize();

				static bool RunCallbacks();

				static discord::Core* Core() { return s_pInstance; }
				static discord::ApplicationManager* ApplicationMgr() { return s_pInstance ? &s_pInstance->ApplicationManager() : nullptr; }
				static discord::UserManager* UserMgr() { return s_pInstance ? &s_pInstance->UserManager() : nullptr; }
				static discord::ImageManager* ImageMgr() { return s_pInstance ? &s_pInstance->ImageManager() : nullptr; }
				static discord::ActivityManager* ActivityMgr() { return s_pInstance ? &s_pInstance->ActivityManager() : nullptr; }
				static discord::RelationshipManager* RelationshipMgr() { return s_pInstance ? &s_pInstance->RelationshipManager() : nullptr; }
				static discord::LobbyManager* LobbyMgr() { return s_pInstance ? &s_pInstance->LobbyManager() : nullptr; }
				static discord::NetworkManager* NetworkMgr() { return s_pInstance ? &s_pInstance->NetworkManager() : nullptr; }
				static discord::OverlayManager* OverlayMgr() { return s_pInstance ? &s_pInstance->OverlayManager() : nullptr; }
				static discord::StorageManager* StorageMgr() { return s_pInstance ? &s_pInstance->StorageManager() : nullptr; }
				static discord::StoreManager* StoreMgr() { return s_pInstance ? &s_pInstance->StoreManager() : nullptr; }
				static discord::VoiceManager* VoiceMgr() { return s_pInstance ? &s_pInstance->VoiceManager() : nullptr; }
				static discord::AchievementManager* AchievementMgr() { return s_pInstance ? &s_pInstance->AchievementManager() : nullptr; }
				
				static discord::UserId PlayerID();
				static discord::ClientId GameID();

				static uint32_t GetAvatarSize(const EAvatarSize& size);
				static bool GetCurrentUser(discord::User& outCurrentUser);

// 				//! Logging and troubleshooting.
// 				//! ##@{
 				static const char* ErrorString(discord::Result code);

				static int cl_setDiscord;

			private:
				static void OnLogMessage(discord::LogLevel level, char const* szMessage);

				static discord::Core* s_pInstance;
			};
		}
	}
}
