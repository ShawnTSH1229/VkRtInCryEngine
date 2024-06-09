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

class CAchievement
{

public:

	CAchievement()
		: m_achievementData()
	{}

	CAchievement(const CryGUID& guid)
		: m_achievementData()
	{
		if (auto pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
		{
			if (IStatisticsManager* pStatisticsManager = pPlugin->GetStatisticsManager())
			{
				m_achievementData = pStatisticsManager->GetAchievementData(guid);
			}
		}
	}

	static inline void ReflectType(Schematyc::CTypeDesc<CAchievement>& typeDesc)
	{
		typeDesc.SetGUID("{0869B857-C082-4FAA-88AC-2E39CFDD39D0}"_cry_guid);
		typeDesc.SetLabel("Achievement");
		typeDesc.SetDescription("An achievement for a specific platform");
	}

	const SAchievementData& GetAchievementData() const
	{
		return m_achievementData;
	}

	inline void Serialize(Serialization::IArchive& archive)
	{
		Schematyc::SerializationUtils::CScopedQuickSearchConfig<CryGUID> quickSearchConfig(archive, "Achievement", "::");

		if (auto pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
		{
			IStatisticsManager* pStatisticsManager = pPlugin->GetStatisticsManager();

			if (archive.isEdit() && pStatisticsManager)
			{
				auto visitor = [&quickSearchConfig](const SAchievementData& data)
				{
					const string platformName = Helper::GetServiceName(data.platform);

					string label;
					label.Format("%s::%s", platformName.c_str(), data.szEditorLabel);

					quickSearchConfig.AddOption(data.szEditorLabel, data.guid, label, "A platform specific achievement identifier.");
				};

				pStatisticsManager->Visit(visitor);
			}

			Schematyc::SerializationUtils::SQuickSearchTypeWrapper<CryGUID> wrappedGUID;
			if (archive.isInput())
			{
				CRY_ASSERT(pStatisticsManager != nullptr, "Could not get statistics.");
				archive(wrappedGUID, "achievementId", "Achievement Id");

				if (pStatisticsManager)
				{
					m_achievementData = pStatisticsManager->GetAchievementData(wrappedGUID.value);
				}
			}
			else if (archive.isOutput())
			{
				if (!m_achievementData.guid.IsNull())
				{
					wrappedGUID.value = m_achievementData.guid;
				}

				archive(wrappedGUID, "achievementId", "Achievement Id");
			}
		}
	}

	bool operator==(const CAchievement& other) const
	{
		if (m_achievementData.guid.IsNull() || other.m_achievementData.guid.IsNull())
			return false;

		return m_achievementData.guid == other.m_achievementData.guid;
	}

	bool operator!=(const CAchievement& other) const
	{
		return !(*this == other);
	}

private:

	SAchievementData m_achievementData;

};

} // Cry::GamePlatform