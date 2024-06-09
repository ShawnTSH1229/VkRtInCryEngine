// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "StatisticsManager.h"

#include <GamePlatformHelper.h>
#include <CrySystem/ConsoleRegistration.h>
#include <CryGamePlatform/Interface/IPlatformStatistics.h>

#define READORCONTINUE_ACH_ATTRIBUTE(name, varname)                                                                      \
	if (!node->getAttr( # name, varname))                                                                                \
	{                                                                                                                    \
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Missing attribute '" # name "' in achievement #%d.", i); \
		continue;                                                                                                        \
	}

namespace Cry::GamePlatform
{

CStatisticsManager::CStatisticsManager()
	: m_statDlOnStartup(0)
	, m_achievementFile("")
	, m_achievementList({})
{
}

CStatisticsManager::~CStatisticsManager()
{
	m_achievementList.clear();
	m_statisticList.clear();

	if (gEnv->pConsole)
	{
		if (ICVar* pCVar = gEnv->pConsole->GetCVar("g_PlatformStatsFile"))
		{
			ConsoleRegistrationHelper::Unregister(pCVar);
		}
	}
}

void CStatisticsManager::Initialize()
{
	REGISTER_CVAR2("g_PlatformStatsFile", &m_achievementFile, "libs/config/statistics.xml", VF_REQUIRE_APP_RESTART, "Path to the platform statistic definitions file");

	ParseAchievementFile();
}

void CStatisticsManager::WriteAchievementData(const SAchievementStorage& in, SAchievementData& out) const
{
	out.guid = in.m_guid;
	out.szEditorLabel = in.m_editorLabel.c_str();
	out.szLabel = in.m_label.c_str();
	out.szDesc = in.m_desc.c_str();
	out.szApiName = in.m_apiName.c_str();
	out.apiId = in.m_apiId;
	out.minProgress = in.m_minProgress;
	out.maxProgress = in.m_maxProgress;
	out.platform = in.m_platform;
}

void CStatisticsManager::WriteStatisticData(const SStatisticStorage& in, SStatisticData& out) const
{
	out.guid = in.m_guid;
	out.szEditorLabel = in.m_editorLabel.c_str();
	out.szLabel = in.m_label.c_str();
	out.szDesc = in.m_desc.c_str();
	out.szApiName = in.m_apiName.c_str();
	out.platform = in.m_platform;
	out.statDataType = in.m_statDataType;
}

SAchievementData CStatisticsManager::GetAchievementData(const CryGUID& guid) const
{
	SAchievementData data;

	auto it = m_achievementList.find(guid);
	if (it != m_achievementList.end())
	{
		WriteAchievementData(*it, data);
	}

	return data;
}

SAchievementData CStatisticsManager::GetAchievementData(EGamePlatform platform, int apiId) const
{
	SAchievementData data;

	auto it = std::find_if(m_achievementList.cbegin(), m_achievementList.cend(), [&platform, &apiId](const SAchievementStorage& data) { return data.m_platform == platform && data.m_apiId == apiId; });
	if (it != m_achievementList.end())
	{
		WriteAchievementData(*it, data);
	}
	return data;
}

SAchievementData CStatisticsManager::GetAchievementData(EGamePlatform platform, const char* szApiName) const
{
	SAchievementData data;

	auto it = std::find_if(m_achievementList.cbegin(), m_achievementList.cend(), [&platform, &szApiName](const SAchievementStorage& data) { return data.m_platform == platform && stricmp(data.m_apiName.c_str(), szApiName) == 0; });
	if (it != m_achievementList.end())
	{
		WriteAchievementData(*it, data);
	}
	return data;
}

DynArray<SAchievementData> CStatisticsManager::GetAchievements() const
{
	DynArray<SAchievementData> resultset = {};
	std::for_each(m_achievementList.cbegin(), m_achievementList.cend(), [this, &resultset](const SAchievementStorage& storage)
	{
		SAchievementData data;
		WriteAchievementData(storage, data);
		resultset.emplace_back(data);
	});

	return resultset;
}

void CStatisticsManager::Visit(AchievementVisitor f) const
{
	for (const SAchievementStorage& achievement : m_achievementList)
	{
		SAchievementData data;
		WriteAchievementData(achievement, data);
		f(data);
	}
}

SStatisticData CStatisticsManager::GetStatisticData(const CryGUID& guid) const
{
	SStatisticData data;

	auto it = m_statisticList.find(guid);
	if (it != m_statisticList.end())
	{
		WriteStatisticData(*it, data);
	}
	return data;
}

SStatisticData CStatisticsManager::GetStatisticData(EGamePlatform platform, const char* szApiName) const
{
	SStatisticData data;

	auto it = std::find_if(m_statisticList.cbegin(), m_statisticList.cend(), [&platform, &szApiName](const SStatisticStorage& data) { return data.m_platform == platform && stricmp(data.m_apiName.c_str(), szApiName) == 0; });
	if (it != m_statisticList.end())
	{
		WriteStatisticData(*it, data);
	}
	return data;
}

DynArray<SStatisticData> CStatisticsManager::GetStatistics() const
{
	DynArray<SStatisticData> resultset = {};
	std::for_each(m_statisticList.cbegin(), m_statisticList.cend(), [this, &resultset](const SStatisticStorage& storage)
	{
		SStatisticData data;
		WriteStatisticData(storage, data);
		resultset.emplace_back(data);
	});

	return resultset;
}

void CStatisticsManager::Visit(StatisticVisitor f) const
{
	for (const SStatisticStorage& statistic : m_statisticList)
	{
		SStatisticData data;
		WriteStatisticData(statistic, data);
		f(data);
	}
}

void CStatisticsManager::ParseAchievementFile()
{
	if (gEnv->pCryPak->IsFileExist(m_achievementFile))
	{
		XmlNodeRef root = gEnv->pSystem->LoadXmlFromFile(m_achievementFile);
		const size_t numChildren = root->getChildCount();

		for (size_t i = 0; i < numChildren; i++)
		{
			XmlNodeRef node = root->getChild(i);
			if (node->isTag("Achievement"))
			{
				CryGUID guid;
				XmlString editorlabel;
				XmlString label;
				XmlString description;
				XmlString apiname;
				int apiid;
				int min;
				int max;
				XmlString platform;

				READORCONTINUE_ACH_ATTRIBUTE(guid, guid);
				READORCONTINUE_ACH_ATTRIBUTE(editorlabel, editorlabel);
				READORCONTINUE_ACH_ATTRIBUTE(label, label);
				READORCONTINUE_ACH_ATTRIBUTE(description, description);
				READORCONTINUE_ACH_ATTRIBUTE(apiname, apiname);
				READORCONTINUE_ACH_ATTRIBUTE(apiid, apiid);
				READORCONTINUE_ACH_ATTRIBUTE(min, min);
				READORCONTINUE_ACH_ATTRIBUTE(max, max);
				READORCONTINUE_ACH_ATTRIBUTE(platform, platform);

				const EGamePlatform ePlatform = Helper::GetServiceEnum(platform.c_str());

				if (ePlatform == EGamePlatform::None)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Unrecognized platform found '%s' for achievement '%s'.", platform.c_str(), apiname.c_str());
					continue;
				}

				m_achievementList.emplace(guid, editorlabel, label, description, apiname, apiid, min, max, ePlatform);
			}
			else if (node->isTag("Statistic"))
			{
				CryGUID guid;
				XmlString editorlabel;
				XmlString label;
				XmlString description;
				XmlString apiname;
				XmlString platform;
				XmlString type;

				READORCONTINUE_ACH_ATTRIBUTE(guid, guid);
				READORCONTINUE_ACH_ATTRIBUTE(editorlabel, editorlabel);
				READORCONTINUE_ACH_ATTRIBUTE(label, label);
				READORCONTINUE_ACH_ATTRIBUTE(description, description);
				READORCONTINUE_ACH_ATTRIBUTE(apiname, apiname);
				READORCONTINUE_ACH_ATTRIBUTE(platform, platform);
				READORCONTINUE_ACH_ATTRIBUTE(type, type);

				const EGamePlatform ePlatform = Helper::GetServiceEnum(platform.c_str());
				const EStatDataType eDataType = Helper::StatTypeFromName(type);

				if (ePlatform == EGamePlatform::None)
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Unrecognized platform found '%s' for statistic '%s'.", platform.c_str(), apiname.c_str());
					continue;
				}

				m_statisticList.emplace(guid, editorlabel, label, description, apiname, ePlatform, eDataType);
			}
			else
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Unexpected tag '%s' encountered in statistics file. Expected 'Achievement' or 'Statistic'.", node->getTag());
			}
		}

		return;
	}

	CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Statistics file '%s' could not be parsed. Statistics & Achievements will not be available.", m_achievementFile);
}

} // Cry::GamePlatform

#undef READORCONTINUE_ACH_ATTRIBUTE
