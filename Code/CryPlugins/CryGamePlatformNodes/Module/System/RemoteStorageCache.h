// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>
#include <IRemoteStorageCache.h>

namespace Cry::GamePlatform
{

class CRemoteStorageCache : public IRemoteStorageCache
{

	struct SPlatformCache
	{
		std::set<CCachedLocal, std::less<>>  remoteFiles;
		std::set<CCachedShared, std::less<>> sharedFiles;
	};

public:

	CRemoteStorageCache();
	~CRemoteStorageCache();

	virtual const CCachedLocal&  GetOrCreateCache(const char* name, EGamePlatform platform) override;
	virtual const CCachedShared& GetOrCreateCache(const ISharedRemoteFile::Identifier& id, EGamePlatform platform) override;

	virtual void ReleaseCache(const CCachedLocal& cache) override;
	virtual void ReleaseCache(const CCachedShared& cache) override;
	virtual void ReleaseCache(EGamePlatform platform) override;
	virtual void ReleaseAll() override;

#ifndef _RELEASE
	virtual void VisitCache(EGamePlatform platform, const TLocalCacheVisitor& visitor) override
	{
		m_iterating = true;

		if (m_cacheLists.count(platform))
		{
			for (const auto& cache : m_cacheLists.at(platform).remoteFiles)
			{
				visitor(cache);
			}
		}

		m_iterating = false;
	}

	virtual void VisitCache(EGamePlatform platform, const TSharedCacheVisitor& visitor) override
	{
		m_iterating = true;

		if (m_cacheLists.count(platform))
		{
			for (const auto& cache : m_cacheLists.at(platform).sharedFiles)
			{
				visitor(cache);
			}
		}

		m_iterating = false;
	}

	bool m_iterating = false;
#endif

protected:

	SPlatformCache& GetOrCreatePlatformCache(EGamePlatform platform);

private:

	std::map<EGamePlatform, SPlatformCache> m_cacheLists;

};

} // Cry::GamePlatform