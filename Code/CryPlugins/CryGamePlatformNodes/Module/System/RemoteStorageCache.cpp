// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "RemoteStorageCache.h"
#include <GamePlatformHelper.h>

namespace Cry::GamePlatform
{

CRemoteStorageCache::CRemoteStorageCache()
{}

CRemoteStorageCache::~CRemoteStorageCache()
{}

const CCachedLocal& CRemoteStorageCache::GetOrCreateCache(const char* name, EGamePlatform platform)
{
	CRY_ASSERT(std::strlen(name) > 0);

	SPlatformCache& platformCache = GetOrCreatePlatformCache(platform);

	auto it = platformCache.remoteFiles.find(name);
	if (it == platformCache.remoteFiles.end())
	{
		if (auto pService = Helper::GetService(platform))
		{
			if (auto pRemoteStorage = pService->GetRemoteStorage())
			{
				it = platformCache.remoteFiles.insert(CCachedLocal(name, platform, pRemoteStorage->GetFile(name))).first;
			}
		}
	}

	if (it == platformCache.remoteFiles.end())
	{
		static const CCachedLocal s_default = CCachedLocal();
		return s_default;
	}

	return *it;
}

const CCachedShared& CRemoteStorageCache::GetOrCreateCache(const ISharedRemoteFile::Identifier& id, EGamePlatform platform)
{
	SPlatformCache& platformCache = GetOrCreatePlatformCache(platform);

	auto it = platformCache.sharedFiles.find(id);
	if (it == platformCache.sharedFiles.end())
	{
		if (auto pService = Helper::GetService(platform))
		{
			if (auto pRemoteStorage = pService->GetRemoteStorage())
			{
				it = platformCache.sharedFiles.insert(CCachedShared(id, platform, pRemoteStorage->GetSharedFile(id))).first;
			}
		}
	}

	if (it == platformCache.sharedFiles.end())
	{
		static const CCachedShared s_default = CCachedShared();
		return s_default;
	}

	return *it;
}

inline CRemoteStorageCache::SPlatformCache& CRemoteStorageCache::GetOrCreatePlatformCache(EGamePlatform platform)
{
	return m_cacheLists[platform];
}

void CRemoteStorageCache::ReleaseCache(const CCachedLocal& cache)
{
#ifndef _RELEASE
	CRY_ASSERT(!m_iterating, "[Remote Storage Cache] Attempting to release a Remote File from the cache while iterating a cache list.");
	if (m_iterating) return;
#endif

	SPlatformCache& platformCache = m_cacheLists[cache.GetPlatform()];

	auto it = platformCache.remoteFiles.find(cache);
	if (it != platformCache.remoteFiles.end())
	{
		platformCache.remoteFiles.erase(it);
	}
}

void CRemoteStorageCache::ReleaseCache(const CCachedShared& cache)
{
#ifndef _RELEASE
	CRY_ASSERT(!m_iterating, "[Remote Storage Cache] Attempting to release a Shared File from the cache while iterating a cache list.");
	if (m_iterating) return;
#endif

	SPlatformCache& platformCache = m_cacheLists[cache.GetPlatform()];

	auto it = platformCache.sharedFiles.find(cache);
	if (it != platformCache.sharedFiles.end())
	{
		platformCache.sharedFiles.erase(it);
	}
}

void CRemoteStorageCache::ReleaseCache(EGamePlatform platform)
{
#ifndef _RELEASE
	CRY_ASSERT(!m_iterating, "[Remote Storage Cache] Attempting to release a Platform Cache while iterating a cache list.");
	if (m_iterating) return;
#endif

	m_cacheLists.erase(platform);
}

void CRemoteStorageCache::ReleaseAll()
{
#ifndef _RELEASE
	CRY_ASSERT(!m_iterating, "[Remote Storage Cache] Attempting to release the entire cache while iterating a cache list.");
	if (m_iterating) return;
#endif

	m_cacheLists.clear();
}

} // Cry::GamePlatform