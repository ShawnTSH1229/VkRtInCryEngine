// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/RemoteStorage.h>

#include <IPlugin.h>
#include <Utils/PluginUtils.h>
#include <IRemoteStorageCache.h>

namespace Cry::GamePlatform
{

class COnFileShared : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnFileShared;

	COnFileShared()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_localCache()
		, m_sharedCache()
	{}

	COnFileShared(EGamePlatform platform, IRemoteFile* pRemoteFile)
		: CBaseEventContext(platform, EventType)
	{
		IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get();
		IRemoteStorageCache* pRemoteStorageCache = pPlugin->GetRemoteStorageCache();
		
		m_localCache = pRemoteStorageCache->GetOrCreateCache(pRemoteFile->GetName(), platform);
		m_sharedCache = pRemoteStorageCache->GetOrCreateCache(pRemoteFile->GetSharedIdentifier(), platform);
	}

	static inline void ReflectType(Schematyc::CTypeDesc<COnFileShared>& typeDesc)
	{
		typeDesc.SetGUID("{FD6AD05C-9D6F-497D-994E-C9AFA8CFCFDF}"_cry_guid);
		typeDesc.SetLabel("RemoteStorage::OnFileShared");
		typeDesc.AddMember(&COnFileShared::m_localCache, 'rfil', "remotefile", "Remote File", nullptr, CCachedLocal());
		typeDesc.AddMember(&COnFileShared::m_sharedCache, 'sfil', "sharedfile", "Shared File", nullptr, CCachedShared());
	}

	const CCachedLocal& GetRemoteFile() const
	{
		return m_localCache;
	}

	const CCachedShared& GetSharedFile() const
	{
		return m_sharedCache;
	}

private:

	CCachedLocal m_localCache;
	CCachedShared m_sharedCache;

};

} // Cry::GamePlatform