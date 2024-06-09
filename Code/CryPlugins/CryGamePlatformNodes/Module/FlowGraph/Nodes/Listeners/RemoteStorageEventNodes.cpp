// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "RemoteStorageEventNodes.h"

namespace Cry::GamePlatform
{

void CFlowNode_OnContentCreated::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("UniqueId", _HELP("A unique identifier for the created content.")),
			OutputPortConfig<bool>("RequiresAgreement", _HELP("True if the user is required to accept a legal agreement upon submission.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more user generated content creation requests were replied to since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnContentCreated::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string id;
	if (IUserGeneratedContent* pContent = event.GetUserGeneratedContent().GetPtr())
	{
		id = ToString(pContent->GetIdentifier());
	}

	ActivateOutput(pActInfo, eOut_UniqueId, id);
	ActivateOutput(pActInfo, eOut_RequireAccept, event.IsLegalRequired());
}

void CFlowNode_OnDownloadedSharedFile::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("SharedFileId", _HELP("The unique identifier of the shared file.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more shared files were downloaded since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnDownloadedSharedFile::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string sharedId = ToString(event.GetSharedFile().GetSharedFileId());
	ActivateOutput(pActInfo, eOut_SharedFileId, sharedId);
}

void CFlowNode_OnFileShared::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	if (ConfigureBaseParameters())
	{
		m_outputs.insert(m_outputs.end(), {
			OutputPortConfig<string>("Name", _HELP("The name of the remote file that was shared.")),
			OutputPortConfig<string>("SharedFileId", _HELP("The unique identifier of the shared file to pass to other users.")),
			{ 0 }
		});
	}

	nodeConfig.pInputPorts = s_listenerInputConfiguration.data();
	nodeConfig.pOutputPorts = m_outputs.data();
	nodeConfig.sDescription = "Fired if one or more remote files were shared since the last frame.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_OnFileShared::HandleEvent(SActivationInfo* pActInfo, const Event& event)
{
	string sharedId = ToString(event.GetSharedFile().GetSharedFileId());
	ActivateOutput(pActInfo, eOut_Name, string(event.GetRemoteFile().GetName().c_str()));
	ActivateOutput(pActInfo, eOut_SharedFileId, sharedId);
}

void CFlowNode_StartStopRemoteFileListener::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Start", _HELP("Start listening to the remote storage events.")),
		InputPortConfig_Void("Stop", _HELP("Stop listening to the remote storage events.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("Name", _HELP("The name of the remote file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig_Void("OnStarted", _HELP("Triggered when the Start node was activated.")),
		OutputPortConfig_Void("OnStopped", _HELP("Triggered when the Stop node was activated.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Starts or stops remote storage event listeners for specific files. Useful for listening to OnFileShared events.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_StartStopRemoteFileListener::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		const bool isStart = IsPortActive(pActInfo, eIn_Start);
		const bool isStop = IsPortActive(pActInfo, eIn_Stop);

		if (isStart || isStop)
		{
			const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
			const string name = GetPortString(pActInfo, eIn_Name);

			IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get();
			IRemoteStorageCache* pRemoteStorageCache = pPlugin->GetRemoteStorageCache();
			IPlatformEventManager* pEventManager = pPlugin->GetPlatformEventManager();

			if (pRemoteStorageCache && pEventManager)
			{
				const CCachedLocal& cache = pRemoteStorageCache->GetOrCreateCache(name.c_str(), platform);

				CRY_ASSERT(cache.GetPtr(), "[Game Platform Nodes] Trying to manage a Remote File listener on a non-existent Remote File.");
				if (cache.GetPtr() != nullptr)
				{
					if (IsPortActive(pActInfo, eIn_Start))
					{
						pEventManager->StartListening(cache);
						ActivateOutput(pActInfo, eOut_OnStarted, true);
					}
					else if (IsPortActive(pActInfo, eIn_Stop))
					{
						pEventManager->StopListening(cache);
						ActivateOutput(pActInfo, eOut_OnStopped, true);
					}
				}
			}
		}
	}
}

void CFlowNode_StartStopSharedFileListener::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInputs[] = {
		InputPortConfig_Void("Start", _HELP("Start listening to the remote storage events.")),
		InputPortConfig_Void("Stop", _HELP("Stop listening to the remote storage events.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform the account belongs to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<string>("SharedFileId", _HELP("The unique identifier of the shared file.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutputs[] = {
		OutputPortConfig_Void("OnStarted", _HELP("Triggered when the Start node was activated.")),
		OutputPortConfig_Void("OnStopped", _HELP("Triggered when the Stop node was activated.")),
		{ 0 }
	};

	nodeConfig.pInputPorts = pInputs;
	nodeConfig.pOutputPorts = pOutputs;
	nodeConfig.sDescription = "Starts or stops remote storage event listeners for specific shared files. Useful for listening to OnDownloadedShareFile events.";
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_StartStopSharedFileListener::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		const bool isStart = IsPortActive(pActInfo, eIn_Start);
		const bool isStop = IsPortActive(pActInfo, eIn_Stop);

		if (isStart || isStop)
		{
			const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));
			const uint64 id = std::strtoull(GetPortString(pActInfo, eIn_SharedFileId).c_str(), 0, 10);

			IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get();
			IRemoteStorageCache* pRemoteStorageCache = pPlugin->GetRemoteStorageCache();
			IPlatformEventManager* pEventManager = pPlugin->GetPlatformEventManager();

			if (pRemoteStorageCache && pEventManager)
			{
				const CCachedShared& cache = pRemoteStorageCache->GetOrCreateCache(id, platform);

				CRY_ASSERT(cache.GetPtr(), "[Game Platform Nodes] Trying to manage a Shared File listener on a non-existent Shared File.");
				if (cache.GetPtr() != nullptr)
				{
					if (IsPortActive(pActInfo, eIn_Start))
					{
						pEventManager->StartListening(cache);
						ActivateOutput(pActInfo, eOut_OnStarted, true);
					}
					else if (IsPortActive(pActInfo, eIn_Stop))
					{
						pEventManager->StopListening(cache);
						ActivateOutput(pActInfo, eOut_OnStopped, true);
					}
				}
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Listener:RemoteStorage:OnContentCreated", CFlowNode_OnContentCreated);
REGISTER_FLOW_NODE("GamePlatform:Listener:RemoteStorage:OnDownloadedSharedFile", CFlowNode_OnDownloadedSharedFile);
REGISTER_FLOW_NODE("GamePlatform:Listener:RemoteStorage:OnFileShared", CFlowNode_OnFileShared);
REGISTER_FLOW_NODE("GamePlatform:Listener:RemoteStorage:StartStopRemoteFileListener", CFlowNode_StartStopRemoteFileListener);
REGISTER_FLOW_NODE("GamePlatform:Listener:RemoteStorage:StartStopSharedFileListener", CFlowNode_StartStopSharedFileListener);

} // Cry::GamePlatform
