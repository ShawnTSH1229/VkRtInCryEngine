// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <IPlugin.h>
#include <IStatisticsManager.h>
#include "Nodes/StatisticsNodes.h"
#include "FlowNodeFactory.h"

namespace Cry::GamePlatform
{

inline void CreateStatisticNodes(CStatisticFlowNodeFactory_AutoArray& nodelist)
{
	if (IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
	{
		if (IStatisticsManager* pStats = pPlugin->GetStatisticsManager())
		{
			pStats->Visit([&nodelist](const SAchievementData& data)
			{
				// Create nodes
				CFlowNode_Achievement* pNode = new CFlowNode_Achievement(data);

				CStatisticFlowNodeFactory* pNodeFactory = new CStatisticFlowNodeFactory(
					CreateDisplayName("GamePlatform:Stats:%s:ACH_%s", Helper::GetServiceName(data.platform), data.szLabel),
					pNode);

				nodelist.push_back(pNodeFactory);
			});

			pStats->Visit([&nodelist](const SStatisticData& data)
			{
				const string displayName = CreateDisplayName("GamePlatform:Stats:%s:%s", Helper::GetServiceName(data.platform), data.szLabel);

				CStatisticFlowNodeFactory* pNodeFactory = nullptr;
				switch (data.statDataType)
				{
				case EStatDataType::Float:
					pNodeFactory = new CStatisticFlowNodeFactory(displayName, new CFlowNode_Statistic<float>(data));
					break;

				case EStatDataType::Integer:
					pNodeFactory = new CStatisticFlowNodeFactory(displayName, new CFlowNode_Statistic<int>(data));
					break;
				}

				if (pNodeFactory != nullptr)
				{
					nodelist.push_back(pNodeFactory);
				}
			});
		}
	}
}

} // Cry::GamePlatform
