// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/RemoteStorage.h>

#include <IPlugin.h>
#include <Utils/PluginUtils.h>
#include <IRemoteStorageCache.h>

namespace Cry::GamePlatform
{

class COnDownloadedSharedFile : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnDownloadedSharedFile;

	COnDownloadedSharedFile()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_sharedCache()
	{}

	COnDownloadedSharedFile(EGamePlatform platform, ISharedRemoteFile* pSharedFile)
		: CBaseEventContext(platform, EventType)
	{
		IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get();
		IRemoteStorageCache* pRemoteStorageCache = pPlugin->GetRemoteStorageCache();

		m_sharedCache = pRemoteStorageCache->GetOrCreateCache(pSharedFile->GetIdentifier(), platform);
	}

	static inline void ReflectType(Schematyc::CTypeDesc<COnDownloadedSharedFile>& typeDesc)
	{
		typeDesc.SetGUID("{0A6D14F1-3163-4076-97AF-E3D3E55E0027}"_cry_guid);
		typeDesc.SetLabel("RemoteStorage::OnDownloadedSharedFile");
		typeDesc.AddMember(&COnDownloadedSharedFile::m_sharedCache, 'sfil', "sharedfile", "Shared File", nullptr, CCachedShared());
	}

	const CCachedShared& GetSharedFile() const
	{
		return m_sharedCache;
	}

private:

	CCachedShared m_sharedCache;

};

} // Cry::GamePlatform