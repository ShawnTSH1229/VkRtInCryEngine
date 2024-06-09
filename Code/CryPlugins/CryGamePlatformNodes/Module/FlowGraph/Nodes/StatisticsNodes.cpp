// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "StatisticsNodes.h"
#include <CryGamePlatform/Interface/IPlatformAchievement.h>
#include <CryGamePlatform/Interface/IPlatformStatistics.h>

namespace Cry::GamePlatform
{

void CFlowNode_DownloadStats::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Download", _HELP("Attempts to get current statistics.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the statistic data was updated, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to get current statistics.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_DownloadStats::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		bool success = false;
		if (auto pService = Helper::GetMainService())
		{
			if (IStatistics* pStatistics = pService->GetStatistics())
			{
				success = pStatistics->Download();
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Statistics not available for the current platform");
			}
		}

		ActivateOutput(pActInfo, eOut_Success, success);
	}
}

void CFlowNode_Achievement::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Get achievement details.")),
		InputPortConfig<bool>("SetAchieved", _HELP("True to set achievement as completed, False to reset the achievement.")),
		InputPortConfig<int>("SetProgress", _HELP("Set the progress of the achievement.")),
		InputPortConfig_Void("Reset", _HELP("Reset the progress of the achievement.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the action of the achievement was successful, False otherwise.")),
		OutputPortConfig<string>("GUID", _HELP("A unique global unique identifier for this statistic.")),
		OutputPortConfig<string>("EditorLabel", _HELP("A unique label used in the editor to identify the achievement.")),
		OutputPortConfig<string>("Label", _HELP("The human readable label given to the achievement.")),
		OutputPortConfig<string>("Description", _HELP("The human readable description of the achievement.")),
		OutputPortConfig<string>("ApiName", _HELP("The Api Name of the achievement if available.")),
		OutputPortConfig<int>("ApiId", _HELP("The Api Id of the achievement if available.")),
		OutputPortConfig<string>("Platform", _HELP("The platform this achievement belongs to as a string.")),
		OutputPortConfig<bool>("IsAchieved", _HELP("True if the achievement requirement was satisfied and the user rewarded.")),
		OutputPortConfig<int>("Progress", _HELP("If successful, contains the current details of the specified achievement.")),
		OutputPortConfig<int>("Min", _HELP("Minimum value of progress for this achievement.")),
		OutputPortConfig<int>("Max", _HELP("Maximum value of progress for this achievement.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Handles getting and setting achievement data.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_Achievement::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		bool success = true;

		if (IAchievement* pAchievement = GetPlatformAchievement())
		{
			if (IsPortActive(pActInfo, eIn_SetProgress))
			{
				success &= pAchievement->SetProgress(GetPortInt(pActInfo, eIn_SetProgress));
			}
			else if (event == eFE_Activate && IsPortActive(pActInfo, eIn_SetAchieved))
			{
				success &= pAchievement->Achieve();
			}
			else if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Reset))
			{
				success &= pAchievement->Reset();
			}

			ActivateOutput(pActInfo, eOut_GUID, m_achievement.guid.ToString());
			ActivateOutput(pActInfo, eOut_EditorLabel, string(m_achievement.szEditorLabel));
			ActivateOutput(pActInfo, eOut_Label, string(m_achievement.szLabel));
			ActivateOutput(pActInfo, eOut_Description, string(m_achievement.szDesc));
			ActivateOutput(pActInfo, eOut_ApiName, string(m_achievement.szApiName));
			ActivateOutput(pActInfo, eOut_ApiId, m_achievement.apiId);
			ActivateOutput(pActInfo, eOut_Platform, string(Helper::GetServiceName(m_achievement.platform)));
			ActivateOutput(pActInfo, eOut_IsAchieved, pAchievement->IsCompleted());
			ActivateOutput(pActInfo, eOut_Progress, pAchievement->GetProgress());
			ActivateOutput(pActInfo, eOut_MinProgress, m_achievement.minProgress);
			ActivateOutput(pActInfo, eOut_MaxProgress, m_achievement.maxProgress);
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not find achievement with the api name '%s' in platform '%s'.", m_achievement.szApiName, Helper::GetServiceName(m_achievement.platform));
			success = false;
		}

		ActivateOutput(pActInfo, eOut_Success, success);
	}
}

IAchievement* CFlowNode_Achievement::GetPlatformAchievement() const
{
	IAchievement* pAchievement = nullptr;

	SAchievementDetails details;
	details.minProgress = m_achievement.minProgress;
	details.maxProgress = m_achievement.maxProgress;

	if (IService* pService = Helper::GetService(m_achievement.platform))
	{
		if (IStatistics* pStatistics = pService->GetStatistics())
		{
			pAchievement = pStatistics->GetAchievement(m_achievement.szApiName, &details);

			if (!pAchievement)
				pAchievement = pStatistics->GetAchievement(m_achievement.apiId, &details);
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get statistics for platform '%s'.", Helper::GetServiceName(m_achievement.platform));
		}
	}

	return pAchievement;
}

void CFlowNode_UploadStats::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Upload", _HELP("Attempts to push current statistics.")),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<bool>("Success", _HELP("True if the statistic data was updated, False otherwise.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Attempts to push current statistics.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_UploadStats::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		bool success = false;
		if (auto pService = Helper::GetMainService())
		{
			if (IStatistics* pStatistics = pService->GetStatistics())
			{
				success = pStatistics->Upload();
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Statistics not available for the current platform");
			}
		}

		ActivateOutput(pActInfo, eOut_Success, success);
	}
}

REGISTER_FLOW_NODE("GamePlatform:Stats:Download", CFlowNode_DownloadStats);
REGISTER_FLOW_NODE("GamePlatform:Stats:Upload", CFlowNode_UploadStats);

} // Cry::GamePlatform