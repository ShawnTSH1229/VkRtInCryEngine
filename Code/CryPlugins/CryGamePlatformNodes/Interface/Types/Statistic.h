// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <IPlugin.h>
#include <IStatisticsManager.h>
#include <GamePlatformHelper.h>
#include <Types/Platform.h>
#include <Utils/PluginUtils.h>

#include <CrySchematyc/SerializationUtils/SerializationQuickSearch.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/ResourceTypes.h>

#include <CryGamePlatform/Interface/IPlatformAchievement.h>

namespace Cry::GamePlatform
{

class CStatistic
{

public:

	CStatistic()
		: m_statisticData()
	{}

	CStatistic(const CryGUID& guid)
		: m_statisticData()
	{
		if (auto pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
		{
			if (IStatisticsManager* pStatisticsManager = pPlugin->GetStatisticsManager())
			{
				m_statisticData = pStatisticsManager->GetStatisticData(guid);
			}
		}
	}

	static inline void ReflectType(Schematyc::CTypeDesc<CStatistic>& typeDesc)
	{
		typeDesc.SetGUID("{B2DF2504-FC31-4F50-B624-6CEF071FFB6D}"_cry_guid);
		typeDesc.SetLabel("Statistic");
		typeDesc.SetDescription("A statistic relating to an achievement for a specific platform");
	}

	const SStatisticData& GetStatisticData() const
	{
		return m_statisticData;
	}

	inline void Serialize(Serialization::IArchive& archive)
	{
		Schematyc::SerializationUtils::CScopedQuickSearchConfig<CryGUID> quickSearchConfig(archive, "Statistic", "::");

		if (auto pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
		{
			IStatisticsManager* pStatisticsManager = pPlugin->GetStatisticsManager();

			if (archive.isEdit() && pStatisticsManager)
			{
				auto visitor = [&quickSearchConfig](const SStatisticData& data)
				{
					const string platformName = Helper::GetServiceName(data.platform);

					string label;
					label.Format("%s::%s", platformName.c_str(), data.szLabel);

					quickSearchConfig.AddOption(data.szEditorLabel, data.guid, label, "A platform specific statistic identifier.");
				};

				pStatisticsManager->Visit(visitor);
			}

			Schematyc::SerializationUtils::SQuickSearchTypeWrapper<CryGUID> wrappedGUID;
			if (archive.isInput())
			{
				CRY_ASSERT(pStatisticsManager != nullptr, "Could not get statistics.");
				archive(wrappedGUID, "statisticId", "Statistic Id");

				if (pStatisticsManager)
				{
					m_statisticData = pStatisticsManager->GetStatisticData(wrappedGUID.value);
				}
			}
			else if (archive.isOutput())
			{
				if (!m_statisticData.guid.IsNull())
				{
					wrappedGUID.value = m_statisticData.guid;
				}

				archive(wrappedGUID, "statisticId", "Statistic Id");
			}
		}
	}

	bool operator==(const CStatistic& other) const
	{
		if (m_statisticData.guid.IsNull() || other.m_statisticData.guid.IsNull())
			return false;

		return m_statisticData.guid == other.m_statisticData.guid;
	}

	bool operator!=(const CStatistic& other) const
	{
		return !(*this == other);
	}

private:

	SStatisticData m_statisticData;

};

} // Cry::GamePlatform