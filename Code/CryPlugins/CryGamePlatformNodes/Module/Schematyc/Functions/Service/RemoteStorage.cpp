// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include <IPlugin.h>
#include <GamePlatformHelper.h>
#include <IRemoteStorageCache.h>
#include <Schematyc/GamePlatformEnv.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvDataType.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform::RemoteStorage
{

bool IsEnabled(EGamePlatform platform)
{
	if (auto pService = Helper::GetService(platform))
	{
		if (auto pRemoteStorage = pService->GetRemoteStorage())
		{
			return pRemoteStorage->IsEnabled();
		}
	}

	return false;
}

bool GetRemoteFiles(EGamePlatform platform, Schematyc::CAnyArrayPtr pArray)
{
	bool success = false;

	const auto& typeDesc = Schematyc::GetTypeDesc<CCachedLocal>();

	if (pArray)
	{
		pArray->Clear();

		IService* pService = Helper::GetService(platform);
		IRemoteStorage* pRemoteStorage = pService ? pService->GetRemoteStorage() : nullptr;
		IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache();

		if (pRemoteStorage && pRemoteStorageCache)
		{

			pRemoteStorage->VisitRemoteFiles([pRemoteStorageCache, &platform, &pArray, &typeDesc](std::shared_ptr<IRemoteFile> pRemoteFile) {
				const CCachedLocal& cache = pRemoteStorageCache->GetOrCreateCache(pRemoteFile->GetName(), platform);
				pArray->PushBack(Schematyc::CAnyConstRef(typeDesc, &cache));
			});

			success = true;
		}
	}

	return success;
}

bool GetRemoteFileByName(const Schematyc::CSharedString& name, EGamePlatform platform, CCachedLocal& result)
{
	if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
	{
		result = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);
		if (result.GetPtr() != nullptr)
		{
			return true;
		}
		else
		{
			pRemoteStorageCache->ReleaseCache(result);
		}
	}

	return false;
}

bool GetSharedFileById(ISharedRemoteFile::Identifier id, EGamePlatform platform, CCachedShared& result)
{
	if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
	{
		result = pRemoteStorageCache->GetOrCreateCache(id, platform);
		if (result.GetPtr() != nullptr)
		{
			return true;
		}
		else
		{
			pRemoteStorageCache->ReleaseCache(result);
		}
	}

	return false;
}

template <class CACHETYPE>
bool Read(const CACHETYPE& cache, Schematyc::CSharedString& data)
{
	std::vector<char> vec;
	if (cache.GetPtr() && cache.GetPtr()->Read(vec))
	{
		data.assign(vec.data(), 0, vec.size());
		return true;
	}

	return false;
}

template <class CACHETYPE>
void GetFileSize(const CACHETYPE& cache, int32& size)
{
	size = cache.GetPtr() ? cache.GetPtr()->GetFileSize() : 0;
}

template<class CACHETYPE>
bool ReadToFile(const CACHETYPE& cache, const Schematyc::CSharedString& outFile)
{
	return cache.GetPtr() ? cache.GetPtr()->ReadToFile(outFile.c_str()) : false;
}

template<class CACHETYPE>
void ReleaseCache(const CACHETYPE& cache)
{
	if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
	{
		pRemoteStorageCache->ReleaseCache(cache);
	}
}

void ReleaseAllFiles(EGamePlatform platform)
{
	if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
	{
		pRemoteStorageCache->ReleaseAll();
	}
}

void DownloadSharedFile(const CCachedShared& cache, int downloadPriority)
{
	if (cache.GetPtr())
		cache.GetPtr()->Download(downloadPriority);
}

bool GetSharedId(const CCachedLocal& cache, ISharedRemoteFile::Identifier& identifier)
{
	identifier = cache.GetPtr() ? cache.GetPtr()->GetSharedIdentifier() : 0;

	return cache.GetPtr() != nullptr;
}

bool Exists(const CCachedLocal& cache)
{
	return cache.GetPtr() ? cache.GetPtr()->Exists() : false;
}

bool Write(const CCachedLocal& cache, const Schematyc::CSharedString& data)
{
	return cache.GetPtr() ? cache.GetPtr()->Write(data.c_str(), data.length()) : false;
}

bool WriteFromFile(const CCachedLocal& cache, const Schematyc::CSharedString& sourceFile)
{
	return cache.GetPtr() ? cache.GetPtr()->WriteFromFile(sourceFile.c_str()) : false;
}

bool Delete(const CCachedLocal& cache)
{
	return cache.GetPtr() ? cache.GetPtr()->Delete() : false;
}

bool Share(const CCachedLocal& cache)
{
	return cache.GetPtr() ? cache.GetPtr()->Share() : false;
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{3846D174-21AC-4364-9D9A-F63D5D8C5A6D}"_cry_guid, "RemoteStorage"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&IsEnabled, "{C5D7935E-9865-46D7-A74A-8D6045A1BCD8}"_cry_guid, "IsEnabled");
		pFunction->BindOutput(0, 'succ', "Enabled");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ReleaseAllFiles, "{BDAEED8A-0501-4D6A-BEDC-04C4CB7CAFFD}"_cry_guid, "ReleaseAllFiles");
		pFunction->BindInput(1, 'pfrm', "Platform");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetRemoteFileByName, "{28ED7FA4-E102-481C-9D4D-9DAAEBF3D0A4}"_cry_guid, "GetRemoteFileByName");
		pFunction->BindOutput(0, 'vald', "IsValid");
		pFunction->BindInput(1, 'name', "FileName");
		pFunction->BindInput(2, 'plat', "Platform");
		pFunction->BindOutput(3, 'file', "RemoteFile");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetSharedFileById, "{E1E980BB-D7BA-452F-A528-7F235366F57B}"_cry_guid, "GetSharedFileById");
		pFunction->BindOutput(0, 'vald', "IsValid");
		pFunction->BindInput(1, 'sid', "SharedId");
		pFunction->BindInput(2, 'plat', "Platform");
		pFunction->BindOutput(3, 'file', "SharedFile");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetRemoteFiles, "{6DE6E10A-5ED9-47F4-8B99-4307577F394D}"_cry_guid, "GetRemoteFiles");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'plat', "Platform");
		pFunction->BindInputArrayType<CCachedLocal>(2, 'arr', "Array[RemoteFile]");
		scope.Register(pFunction);
	}

	auto remoteScope = scope.Register(SCHEMATYC_MAKE_ENV_MODULE("{23AD349C-7A3F-42BF-8091-C4BDB18853B8}"_cry_guid, "RemoteFile"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Read<CCachedLocal>, "{FF6962B8-588A-461E-9E2E-AA38B4077BE4}"_cry_guid, "Read");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindOutput(2, 'data', "Data");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetFileSize<CCachedLocal>, "{087BEBBA-E586-47A9-9E11-C112054B6629}"_cry_guid, "GetFileSize");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindOutput(2, 'size', "File Size");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ReadToFile<CCachedLocal>, "{9158ED6A-4C40-4CB5-870C-42419F4E7338}"_cry_guid, "ReadToFile");
		pFunction->BindOutput(0, 'succ', "Enabled");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindInput(2, 'targ', "Target File");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ReleaseCache<CCachedLocal>, "{CEBD4D18-AD0E-4D6B-B42B-21A07FD4C1F3}"_cry_guid, "ReleaseCache");
		pFunction->BindInput(1, 'cach', "RemoteFile");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetSharedId, "{FC39814F-74ED-441F-965F-303951975283}"_cry_guid, "GetSharedId");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindOutput(2, 'id', "SharedId");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Exists, "{AF679578-8050-4080-A639-E86FF6E2C630}"_cry_guid, "Exists");
		pFunction->BindOutput(0, 'succ', "Exists");
		pFunction->BindInput(1, 'file', "RemoteFile");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Write, "{B6A7A013-C453-43B1-B161-2E0B4F82096C}"_cry_guid, "Write");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindInput(2, 'data', "Data");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&WriteFromFile, "{8BC97D29-4296-4EF3-B941-33C7002ABB3A}"_cry_guid, "WriteFromFile");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindInput(2, 'src', "SourceFile");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Delete, "{07FD2F65-0BDB-4463-82D0-98A4CE86AC96}"_cry_guid, "Delete");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "RemoteFile");
		remoteScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Share, "{63053409-9232-43C0-9AE1-0DEEB85D4EFA}"_cry_guid, "Share");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "RemoteFile");
		remoteScope.Register(pFunction);
	}

	auto sharedScope = scope.Register(SCHEMATYC_MAKE_ENV_MODULE("{43722370-AB93-442E-8CB6-D9A16FE9F6E3}"_cry_guid, "SharedFile"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Read<CCachedShared>, "{694FF96C-FD78-4105-AC99-9271D3D02C6D}"_cry_guid, "Read");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'file', "SharedFile");
		pFunction->BindOutput(2, 'data', "Data");
		sharedScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&GetFileSize<CCachedShared>, "{0D79800C-C90A-407B-80C6-13D6F8213611}"_cry_guid, "GetFileSize");
		pFunction->BindInput(1, 'file', "SharedFile");
		pFunction->BindOutput(2, 'size', "FileSize");
		sharedScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ReadToFile<CCachedShared>, "{D6AA72CB-3A45-4029-B7D8-D2BE0D8797D5}"_cry_guid, "ReadToFile");
		pFunction->BindOutput(0, 'succ', "Enabled");
		pFunction->BindInput(1, 'file', "SharedFile");
		pFunction->BindInput(2, 'targ', "TargetFile");
		sharedScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&ReleaseCache<CCachedShared>, "{92176930-8B2F-4A43-B4F1-98051A6D8228}"_cry_guid, "ReleaseCache");
		pFunction->BindInput(1, 'cach', "SharedFile");
		sharedScope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&DownloadSharedFile, "{780FAE2D-C38D-4BD0-B71C-9768AF60292E}"_cry_guid, "Download");
		pFunction->BindInput(1, 'file', "RemoteFile");
		pFunction->BindInput(2, 'prio', "DownloadPriority");
		sharedScope.Register(pFunction);
	}
}

} // Cry::GamePlatform::RemoteStorage
