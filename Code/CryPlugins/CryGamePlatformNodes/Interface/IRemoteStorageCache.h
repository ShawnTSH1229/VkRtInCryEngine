// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/RemoteStorage.h>

namespace Cry::GamePlatform
{

class IRemoteStorageCache
{

public:

	virtual const CCachedLocal&  GetOrCreateCache(const char* name, EGamePlatform platform) = 0;
	virtual const CCachedShared& GetOrCreateCache(const ISharedRemoteFile::Identifier& id, EGamePlatform platform) = 0;
	
	virtual void                 ReleaseCache(const CCachedLocal& cache) = 0;
	virtual void                 ReleaseCache(const CCachedShared& cache) = 0;
	virtual void                 ReleaseCache(EGamePlatform platform) = 0;
	virtual void                 ReleaseAll() = 0;

#ifndef _RELEASE
	using TLocalCacheVisitor = std::function<void(const CCachedLocal&)>;
	using TSharedCacheVisitor = std::function<void(const CCachedShared&)>;

	virtual void                 VisitCache(EGamePlatform platform, const TLocalCacheVisitor& visitor) = 0;
	virtual void                 VisitCache(EGamePlatform platform, const TSharedCacheVisitor& visitor) = 0;
#endif

};

} // Cry::GamePlatform