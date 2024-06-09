// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "SteamSharedRemoteFile.h"
#include "SteamService.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			CSharedRemoteFile::CSharedRemoteFile(CService& steamService, ISharedRemoteFile::Identifier sharedId)
				: m_service(steamService)
				, m_sharedHandle(sharedId)
			{
			}

			void CSharedRemoteFile::Download(int downloadPriority)
			{
				ISteamRemoteStorage* pSteamRemoteStorage = SteamRemoteStorage();
				if (!pSteamRemoteStorage)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Steam remote storage service not available");
					return;
				}
				SteamAPICall_t hSteamAPICall = pSteamRemoteStorage->UGCDownload(m_sharedHandle, downloadPriority);
				m_callResultDownloaded.Set(hSteamAPICall, this, &CSharedRemoteFile::OnDownloaded);
			}

			bool CSharedRemoteFile::Read(std::vector<char>& bufferOut)
			{
				if (m_data.empty())
					return false;

				bufferOut = m_data;

				return true;
			}

			void CSharedRemoteFile::OnDownloaded(RemoteStorageDownloadUGCResult_t* pResult, bool bIOFailure)
			{
				ISteamRemoteStorage* pSteamRemoteStorage = SteamRemoteStorage();
				if (!pSteamRemoteStorage)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Steam remote storage service not available");
					return;
				}

				m_data.resize(pResult->m_nSizeInBytes);

				int32 result = pSteamRemoteStorage->UGCRead(pResult->m_hFile, m_data.data(), m_data.size(), 0, k_EUGCRead_ContinueReadingUntilFinished);
				if (result == 0)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] Failed to read shared file!");
					return;
				}

				m_listeners.ForEach([this](IListener* pListener) {
					if (pListener->OnDownloadedSharedFile(this) == false)
					{
						m_listeners.Remove(pListener);
					}
				});
			}
		}
	}
}