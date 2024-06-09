// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <GamePlatformHelper.h>
#include <Types/AchievementData.h>
#include <Types/StatisticData.h>
#include <CryFlowGraph/IFlowBaseNode.h>
#include <CryGamePlatform/Interface/IPlatformStatistics.h>

namespace Cry::GamePlatform
{

class CFlowNode_DownloadStats : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Download,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_DownloadStats(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_DownloadStats() override {}

	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_Achievement : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Get,
		eIn_SetAchieved,
		eIn_SetProgress,
		eIn_Reset,
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_GUID,
		eOut_EditorLabel,
		eOut_Label,
		eOut_Description,
		eOut_ApiName,
		eOut_ApiId,
		eOut_Platform,
		eOut_IsAchieved,
		eOut_Progress,
		eOut_MinProgress,
		eOut_MaxProgress,
	};

public:

	CFlowNode_Achievement(const SAchievementData& achievement) : m_achievement(achievement) {}
	~CFlowNode_Achievement() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_Achievement(m_achievement); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

protected:

	IAchievement* GetPlatformAchievement() const;

private:

	const SAchievementData m_achievement;

};

template<typename TDataType, typename = std::enable_if_t<std::is_same<TDataType, float>::value || std::is_same<TDataType, int>::value>>
class CFlowNode_Statistic : public CFlowBaseNode<eNCT_Instanced>
{
	enum EInputs
	{
		eIn_Get,
		eIn_SetValue,
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_GUID,
		eOut_EditorLabel,
		eOut_Label,
		eOut_Description,
		eOut_ApiName,
		eOut_Platform,
		eOut_DataType,
		eOut_Value,
	};

public:

	CFlowNode_Statistic(const SStatisticData& statistic)
		: m_statistic(statistic) {}
	virtual ~CFlowNode_Statistic() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return IFlowNodePtr(new CFlowNode_Statistic(m_statistic)); }
	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); }

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override
	{
		static const SInputPortConfig pInConfig[] =
		{
			InputPortConfig_Void("Get", _HELP("Get statistic value.")),
			InputPortConfig<TDataType>("SetValue", _HELP("Set statistic value.")),
			{ 0 }
	};

		static const SOutputPortConfig pOutConfig[] =
		{
			OutputPortConfig<bool>("Success", _HELP("True if the action was successful, False otherwise.")),
			OutputPortConfig<string>("GUID", _HELP("A unique global unique identifier for this statistic.")),
			OutputPortConfig<string>("EditorLabel", _HELP("The human readable label given to the achievement.")),
			OutputPortConfig<string>("Label", _HELP("The human readable label given to the achievement.")),
			OutputPortConfig<string>("Description", _HELP("The human readable label given to the achievement.")),
			OutputPortConfig<string>("ApiName", _HELP("The Api Name of the achievement if available.")),
			OutputPortConfig<string>("Platform", _HELP("The platform this achievement belongs to as a string.")),
			OutputPortConfig<string>("DataType", _HELP("The data type assigned to this statistic (Float or Integer).")),
			OutputPortConfig<TDataType>("Value", _HELP("Value of the statistic.")),
			{ 0 }
	};

		nodeConfig.sDescription = _HELP("Handles getting and setting the statistic.");
		nodeConfig.pInputPorts = pInConfig;
		nodeConfig.pOutputPorts = pOutConfig;
		nodeConfig.SetCategory(EFLN_ADVANCED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		if (event == eFE_Activate && (IsPortActive(pActInfo, eIn_Get) || IsPortActive(pActInfo, eIn_SetValue)))
		{
			bool success = false;
			TDataType value = TDataType();

			if (IService* pService = Helper::GetService(m_statistic.platform))
			{
				IStatistics* pStatistics = pService->GetStatistics();

				if (pStatistics != nullptr)
				{
					if (IsPortActive(pActInfo, eIn_Get))
					{
						success = pStatistics->Get(m_statistic.szApiName, value);
					}
					else if (IsPortActive(pActInfo, eIn_SetValue))
					{
						success = pActInfo->pInputPorts[eIn_SetValue].GetValueWithConversion(value);
						success &= pStatistics->Set(m_statistic.szApiName, value);
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Could not get statistics for platform '%s'.", Helper::GetServiceName(m_statistic.platform));
				}
			}

			ActivateOutput(pActInfo, eOut_Success, success);
			ActivateOutput(pActInfo, eOut_GUID, m_statistic.guid.ToString());
			ActivateOutput(pActInfo, eOut_EditorLabel, string(m_statistic.szEditorLabel));
			ActivateOutput(pActInfo, eOut_Label, string(m_statistic.szLabel));
			ActivateOutput(pActInfo, eOut_Description, string(m_statistic.szDesc));
			ActivateOutput(pActInfo, eOut_ApiName, string(m_statistic.szApiName));
			ActivateOutput(pActInfo, eOut_Platform, string(Helper::GetServiceName(m_statistic.platform)));
			ActivateOutput(pActInfo, eOut_DataType, string(Helper::StatTypeToName(m_statistic.statDataType)));
			ActivateOutput(pActInfo, eOut_Value, value);
		}
	}

private:

	const SStatisticData m_statistic;

};

class CFlowNode_UploadStats : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Upload,
	};

	enum EOutputs
	{
		eOut_Success,
	};

public:

	CFlowNode_UploadStats(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_UploadStats() override {}

	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
