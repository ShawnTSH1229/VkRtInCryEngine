// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformRemoteStorage.h"
#include "SteamTypes.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			class CSharedRemoteFile
				: public ISharedRemoteFile
			{
			public:
				CSharedRemoteFile(CService& steamService, ISharedRemoteFile::Identifier sharedId);
				virtual ~CSharedRemoteFile() = default;

				// ISharedRemoteFile
				virtual void AddListener(IListener& listener, const char* szName) override { m_listeners.Add(&listener, szName); }
				virtual void RemoveListener(IListener& listener) override { m_listeners.Remove(&listener); }

				virtual void Download(int downloadPriority) override;
				virtual ISharedRemoteFile::Identifier GetIdentifier() const override { return m_sharedHandle; }

				virtual bool Read(std::vector<char>& bufferOut) override;
				virtual int GetFileSize() const override { return m_data.size(); }
				// ~ISharedRemoteFile

				void OnDownloaded(RemoteStorageDownloadUGCResult_t* pResult, bool bIOFailure);
				CCallResult<CSharedRemoteFile, RemoteStorageDownloadUGCResult_t> m_callResultDownloaded;

			protected:
				CService& m_service;

				CListenerSet<IListener*> m_listeners = 1;
				UGCHandle_t m_sharedHandle;

				std::vector<char> m_data;
			};
		}
	}
}