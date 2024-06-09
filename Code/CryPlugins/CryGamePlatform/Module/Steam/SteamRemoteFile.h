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
			class CRemoteFile
				: public IRemoteFile
			{
			public:
				CRemoteFile(CService& steamService, const char* name);
				virtual ~CRemoteFile() = default;

				// IRemoteFile
				virtual void AddListener(IListener& listener, const char* szName) override { m_listeners.Add(&listener, szName); }
				virtual void RemoveListener(IListener& listener) override { m_listeners.Remove(&listener); }

				virtual const char* GetName() const override { return m_name.c_str(); }
				virtual int GetFileSize() const override;

				virtual bool Exists() const override;
				virtual bool Write(const char* pData, int length) override;
				virtual bool Read(std::vector<char>& bufferOut) override;

				virtual bool Delete() override;

				virtual bool Share() override;
				virtual ISharedRemoteFile::Identifier GetSharedIdentifier() const override { return m_sharedHandle; }
				// ~IRemoteFile

			private:
				void OnFileShared(RemoteStorageFileShareResult_t* pResult, bool bIOFailure);
				CCallResult<CRemoteFile, RemoteStorageFileShareResult_t> m_callResultFileShared;

			protected:
				CService & m_service;

				CListenerSet<IListener*> m_listeners = 1;
				string m_name;

				ISharedRemoteFile::Identifier m_sharedHandle;
			};
		}
	}
}