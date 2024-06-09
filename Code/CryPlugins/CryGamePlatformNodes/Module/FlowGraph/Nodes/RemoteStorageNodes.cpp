// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "RemoteStorageNodes.h"
#include <GamePlatformHelper.h>
#include <IPlugin.h>
#include <Utils/PluginUtils.h>
#include <IRemoteStorageCache.h>
#include <FlowGraph/PlatformConfigs.h>

namespace Cry::GamePlatform
{

void CFlowNode_DeleteRemoteFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Delete", _HELP("Deletes the remote file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to delete the file from the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to delete a file from remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_DeleteRemoteFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Delete))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache();
		if (pRemoteStorageCache)
		{
			const CCachedLocal& cache = pRemoteStorageCache->GetOrCreateCache(name, platform);
			if (auto pRemoteFile = cache.GetPtr())
			{
				if (pRemoteFile->Delete())
				{
					isSuccess = true;
					pRemoteStorageCache->ReleaseCache(cache);
				}
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_DownloadSharedFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Download", _HELP("Download the shared file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("SharedFileId", _HELP("The unique identifier of the shared file.")),
		InputPortConfig<int>("DownloadPriority", _HELP("Specifies the priority of the download, a value of 0 being immediate - otherwise the lowest value is prioritized.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to queue the download from the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to download the specified shared file from remote storage. Listen for OnDownloadedSharedFile events.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_DownloadSharedFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Download))
	{
		bool success = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string sharedId = GetPortString(pActInfo, eIn_SharedFileId);
		const int priority = crymath::clamp(GetPortInt(pActInfo, eIn_DownloadPriority), 0, std::numeric_limits<int>::max());

		if (sharedId.length() > 0)
		{
			const size_t id = std::strtoull(sharedId.c_str(), nullptr, 10);

			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedShared& sharedCache = pRemoteStorageCache->GetOrCreateCache(id, platform);
				if (ISharedRemoteFile* pRemoteFile = sharedCache.GetPtr())
				{
					pRemoteFile->Download(priority);
					success = true;
				}
			}
		}

		ActivateOutput(pActInfo, eOut_Success, success);
	}
}

void CFlowNode_FileExists::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("checks if the file exists on the remote storage.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the remote file belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to get a handle to the file.")),
		OutputPortConfig<bool>("Exists", _HELP("True if the file exists on the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks to see if the specified file exists on the remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_FileExists::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		bool hasFile = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);
			hasFile = localCache.GetPtr() != nullptr;

			if (hasFile)
			{
				ActivateOutput(pActInfo, eOut_Exists, localCache.GetPtr()->Exists());
			}
		}

		ActivateOutput(pActInfo, eOut_Success, hasFile);
	}
}

void CFlowNode_GetCacheList::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Query the platform cache list.")),
		InputPortConfig_Void("GetNext", _HELP("Get the next cache that was retrieved (if available).")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform of the cache to query."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("CacheType", 0, _HELP("The cache list to query."),"CacheType", _UICONFIG("enum_int:Local=0,Shared=1")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<int>("Remaining", _HELP("Remaining friends you can get from the list using GetNext.")),
		OutputPortConfig<string>("NameOrId", _HELP("The name, or shared id, of the cache.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Get all cached files in memory of the specified type for the specified platform. Not Available in RELEASE.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_DEBUG);
}

void CFlowNode_GetCacheList::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{

#ifndef _RELEASE

	if (event == eFE_Activate)
	{
		if (IsPortActive(pActInfo, eIn_Get))
		{
			// Empty the stored cache list
			m_cacheList = {};

			EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
			const int cacheType = GetPortInt(pActInfo, eIn_CacheType);

			if (IRemoteStorageCache* pStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				CRY_ASSERT(crymath::clamp(cacheType, 0, 1) == cacheType, "[Game Platform Nodes] Invalid cache type given as parameter to Get Cache List node.");

				switch (cacheType)
				{
				case 0: /* Local */
					pStorageCache->VisitCache(platform, [this](const CCachedLocal& cache) { m_cacheList.emplace(cache.GetName().c_str()); });
					break;

				case 1: /* Shared */
					pStorageCache->VisitCache(platform, [this](const CCachedShared& cache) { m_cacheList.emplace(string().Format("%zu", cache.GetSharedFileId())); });
					break;
				}
			}
		}
		else if (IsPortActive(pActInfo, eIn_GetNext))
		{
			if (m_cacheList.size())
			{
				ActivateOutput(pActInfo, eOut_NameOrId, m_cacheList.front());
				m_cacheList.pop();
			}
		}

		ActivateOutput(pActInfo, eOut_Remaining, static_cast<int>(m_cacheList.size()));
	}

#endif

}

void CFlowNode_GetFileSize::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the size of the remote file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to get a handle to the file.")),
		OutputPortConfig<int>("Filesize", _HELP("Size of the file in bytes.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to get the size of a file from remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetFileSize::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		bool hasFile = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);
			hasFile = localCache.GetPtr() != nullptr;

			if (hasFile)
			{
				ActivateOutput(pActInfo, eOut_FileSize, localCache.GetPtr()->GetFileSize());
			}
		}

		ActivateOutput(pActInfo, eOut_Success, hasFile);
	}
}

void CFlowNode_GetSharedFileId::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Get the shared id if set.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to get the shared id.")),
		OutputPortConfig<int>("SharedId", _HELP("Shared file id.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to get the shared file id for the specified remote file.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetSharedFileId::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		bool hasFile = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		if (name.length() > 0)
		{
			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name, platform);
				if (localCache.GetPtr() != nullptr)
				{
					hasFile = true;

					string sharedId;
					sharedId.Format("%" PRIu64, localCache.GetPtr()->GetSharedIdentifier());
					ActivateOutput(pActInfo, eOut_SharedId, sharedId);
				}
				else
				{
					pRemoteStorageCache->ReleaseCache(localCache);
				}
			}
		}
	}
}

void CFlowNode_GetSharedFileSize::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the size of the shared file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("SharedFileId", _HELP("Unique identifier of the shared file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to get a handle to the shared file.")),
		OutputPortConfig<int>("Filesize", _HELP("Size of the shared file in bytes.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to get the size of a shared file from remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetSharedFileSize::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Get))
	{
		bool hasFile = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string sharedId = GetPortString(pActInfo, eIn_SharedId);

		if (sharedId.length() > 0)
		{
			const uint64 id = std::strtoull(sharedId.c_str(), nullptr, 10);

			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedShared& sharedCache = pRemoteStorageCache->GetOrCreateCache(id, platform);
				if (sharedCache.GetPtr() != nullptr)
				{
					hasFile = true;
					ActivateOutput(pActInfo, eOut_FileSize, sharedCache.GetPtr()->GetFileSize());
				}
				else
				{
					pRemoteStorageCache->ReleaseCache(sharedCache);
				}
			}
		}

		ActivateOutput(pActInfo, eOut_Success, hasFile);
	}
}

void CFlowNode_IsEnabled::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Check if remote storage is available.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("IsAvailable", _HELP("True if remote storage is available.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks if remote storage capability is available on the platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_IsEnabled::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (IRemoteStorage* pRemoteStorage = pService->GetRemoteStorage())
			{
				const bool isAvailable = pRemoteStorage->IsEnabled();
				ActivateOutput(pActInfo, eOut_IsEnabled, isAvailable);
			}
		}
	}
}

void CFlowNode_ReadData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Read", _HELP("Read data from a file via platform RemoteStorage.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the remote file belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to read the file from the remote storage.")),
		OutputPortConfig<string>("Data", _HELP("The data that was read. Warning: Binary data may be truncated.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to read data from remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReadData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Read))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);

			if (auto pRemoteFile = localCache.GetPtr())
			{
				std::vector<char> buf;
				isSuccess = pRemoteFile->Read(buf);

				string data;
				data.assign(buf.data(), buf.size());

				ActivateOutput(pActInfo, eOut_Data, data);
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_ReadDataToFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Read", _HELP("Read data from a file via platform RemoteStorage into a local file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the remote file belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		InputPortConfig<string>("Filepath", _HELP("Name and path to the local file to store the data (can use path alias's such as %USER% and %ENGINEROOT% etc).")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to read the file from the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to read data from remote storage and write that data to a local file.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReadDataToFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Read))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);
		const CryPathString path = GetPortString(pActInfo, eIn_FilePath);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);

			if (auto pRemoteFile = localCache.GetPtr())
			{
				isSuccess = pRemoteFile->ReadToFile(path);
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_ReadSharedData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Read", _HELP("Read data from the shared file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the shared file is associated with."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("SharedFileId", _HELP("The unique identifier of the shared file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to read the shared file.")),
		OutputPortConfig<string>("Data", _HELP("The data that was read. Warning: Binary data may be truncated.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to read a shared file from remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReadSharedData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Read))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string sharedId = GetPortString(pActInfo, eIn_SharedId);

		if (sharedId.length() > 0)
		{
			const size_t id = std::strtoull(sharedId.c_str(), nullptr, 10);

			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedShared& sharedCache = pRemoteStorageCache->GetOrCreateCache(id, platform);
				if (auto pSharedFile = sharedCache.GetPtr())
				{
					std::vector<char> buf;
					isSuccess = pSharedFile->Read(buf);

					string data;
					data.assign(buf.data(), buf.size());
					ActivateOutput(pActInfo, eOut_Data, data);
				}
				else
				{
					pRemoteStorageCache->ReleaseCache(sharedCache);
				}
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_ReadSharedDataToFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Read", _HELP("Read data from the shared file.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the shared file is associated with."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("SharedFileId", _HELP("The unique identifier of the shared file.")),
		InputPortConfig<string>("Filepath", _HELP("Name and path to the local file to store the data (can use path alias's such as %USER% and %ENGINEROOT% etc).")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to read the shared file.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to read a shared file and write the data to the specified local file.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReadSharedDataToFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Read))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string sharedId = GetPortString(pActInfo, eIn_SharedId);
		const CryPathString path = GetPortString(pActInfo, eIn_FilePath);

		if (sharedId.length() > 0)
		{
			const size_t id = std::strtoull(sharedId.c_str(), nullptr, 10);

			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedShared& sharedCache = pRemoteStorageCache->GetOrCreateCache(id, platform);
				if (auto pSharedFile = sharedCache.GetPtr())
				{
					isSuccess = pSharedFile->ReadToFile(path);
				}
				else
				{
					pRemoteStorageCache->ReleaseCache(sharedCache);
				}
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_ReleaseRemoteFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Release", _HELP("Release the remote file cache.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the cached file belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig_Void("OnReleased", _HELP("Fired after the file was released successfully")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Releases a cached remote file from the Remote Storage cache.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReleaseRemoteFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Release))
	{
		bool isReleased = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& cache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);
			pRemoteStorageCache->ReleaseCache(cache);
			isReleased = true;
		}

		ActivateOutput(pActInfo, eOut_OnReleased, isReleased);
	}
}

void CFlowNode_ReleaseSharedFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Release", _HELP("Release the shared file cache.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the shared file is associated with."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("SharedId", _HELP("Shared Id of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig_Void("OnReleased", _HELP("Fired after the file was released successfully")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Releases a cached shared file from the Remote Storage cache.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ReleaseSharedFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Release))
	{
		bool isReleased = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string sharedId = GetPortString(pActInfo, eIn_SharedId);

		if (sharedId.length() > 0)
		{
			const uint64 id = std::strtoull(sharedId.c_str(), 0, 10);

			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedShared& cache = pRemoteStorageCache->GetOrCreateCache(id, platform);
				pRemoteStorageCache->ReleaseCache(cache);
				isReleased = true;
			}
		}

		ActivateOutput(pActInfo, eOut_OnReleased, isReleased);
	}
}

void CFlowNode_SharedFileAvailable::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Check", _HELP("Check if available.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the shared file is associated with."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Id of the shared file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Exists", _HELP("True if the shared file is available on the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Checks to see if the specified shared file is available on the remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_SharedFileAvailable::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Check))
	{
		bool isAvailable = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string sharedId = GetPortString(pActInfo, eIn_SharedId);

		if (sharedId.length() > 0)
		{
			const size_t id = std::strtoull(sharedId.c_str(), nullptr, 10);

			if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
			{
				const CCachedShared& sharedCache = pRemoteStorageCache->GetOrCreateCache(id, platform);
				if (sharedCache.GetPtr() != nullptr)
				{
					isAvailable = true;
				}
				else
				{
					pRemoteStorageCache->ReleaseCache(sharedCache);
				}
			}
		}

		ActivateOutput(pActInfo, eOut_Available, isAvailable);
	}
}

void CFlowNode_ShareRemoteFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Share", _HELP("Share the file from remote storage.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to set."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to share the file.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP(
		"Attempts to share a file from remote storage. "
		"Use the generated identifier to pass to other users so they can download the shared file. "
		"OnFileShared event will be called on success containing the identifier to pass to other users."
	);

	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_ShareRemoteFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Share))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);
			if (auto pLocalFile = localCache.GetPtr())
			{
				isSuccess = pLocalFile->Share();
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_WriteDataFromFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Write", _HELP("Write data to a file via platform RemoteStorage.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the remote file belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the file.")),
		InputPortConfig<string>("Filepath", _HELP("Path to the file to copy to remote storage.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to write the file to the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP(
		"Attempts to write data from a local a file to remote storage. "
		"The path goes through the CryPak system. You can also use aliases such as %USER% or %ENGINEROOT% etc."
	);
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_WriteDataFromFile::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Write))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);
		const CryPathString path = GetPortString(pActInfo, eIn_FilePath);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);

			if (auto pRemoteFile = localCache.GetPtr())
			{
				isSuccess = pRemoteFile->WriteFromFile(path);
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

void CFlowNode_WriteData::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Write", _HELP("Write data to a file via platform RemoteStorage.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the remote file belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("Name of the file.")),
		InputPortConfig<string>("Data", _HELP("Data to write to the file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if we managed to write the file to the remote storage.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to write data to a file on remote storage.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_WriteData::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Write))
	{
		bool isSuccess = false;

		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
		const string name = GetPortString(pActInfo, eIn_Name);
		const string data = GetPortString(pActInfo, eIn_Data);

		if (IRemoteStorageCache* pRemoteStorageCache = CPluginHelper<IPlugin_GamePlatformNodes>::Get()->GetRemoteStorageCache())
		{
			const CCachedLocal& localCache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);

			if (auto pRemoteFile = localCache.GetPtr())
			{
				isSuccess = pRemoteFile->Write(data.c_str(), data.length());
			}
		}

		ActivateOutput(pActInfo, eOut_Success, isSuccess);
	}
}

REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:DeleteRemoteFile", CFlowNode_DeleteRemoteFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:DownloadSharedFile", CFlowNode_DownloadSharedFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:FileExists", CFlowNode_FileExists);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:GetCacheList", CFlowNode_GetCacheList);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:GetFileSize", CFlowNode_GetFileSize);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:GetSharedFileId", CFlowNode_GetSharedFileId);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:GetSharedFileSize", CFlowNode_GetSharedFileSize);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:IsEnabled", CFlowNode_IsEnabled);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ReadData", CFlowNode_ReadData);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ReadDataToFile", CFlowNode_ReadDataToFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ReadSharedData", CFlowNode_ReadSharedData);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ReadSharedDataToFile", CFlowNode_ReadSharedDataToFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ReleaseRemoteFile", CFlowNode_ReleaseRemoteFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ReleaseSharedFile", CFlowNode_ReleaseSharedFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:SharedFileAvailable", CFlowNode_SharedFileAvailable);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:ShareRemoteFile", CFlowNode_ShareRemoteFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:WriteDataFromFile", CFlowNode_WriteDataFromFile);
REGISTER_FLOW_NODE("GamePlatform:RemoteStorage:WriteData", CFlowNode_WriteData);

} // Cry::GamePlatform
