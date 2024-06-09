// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "SteamRemoteStorage.h"
#include "SteamSharedRemoteFile.h"
#include "SteamRemoteFile.h"
#include "UserGeneratedContentManager.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			CRemoteStorage::CRemoteStorage(CService& steamService)
				: m_service(steamService)
			{
				m_pUGCManager = stl::make_unique<CUserGeneratedContentManager>(m_service);
			}

			bool CRemoteStorage::IsEnabled() const
			{
				if (ISteamRemoteStorage* pSteamRemoteStorage = SteamRemoteStorage())
					return pSteamRemoteStorage->IsCloudEnabledForApp();

				return false;
			}

			std::shared_ptr<IRemoteFile> CRemoteStorage::GetFile(const char* name)
			{
				if (!IsEnabled())
					return nullptr;

				return std::make_shared<CRemoteFile>(m_service, name);
			}

			std::shared_ptr<ISharedRemoteFile> CRemoteStorage::GetSharedFile(ISharedRemoteFile::Identifier id)
			{
				if (id == 0)
					return nullptr;

				return std::make_shared<CSharedRemoteFile>(m_service, id);
			}

			void CRemoteStorage::VisitRemoteFiles(std::function<void(std::shared_ptr<IRemoteFile>)> visitor)
			{
				if (ISteamRemoteStorage* pSteamRemoteStorage = SteamRemoteStorage())
				{
					int32 numFiles = pSteamRemoteStorage->GetFileCount();
					for (int32 i=0; i<numFiles; i++)
					{
						const char* szName = pSteamRemoteStorage->GetFileNameAndSize(i, nullptr);
						visitor(GetFile(szName));
					}
				}
			}
		}
	}
}