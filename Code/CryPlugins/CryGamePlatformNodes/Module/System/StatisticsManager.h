// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>
#include <IStatisticsManager.h>

namespace Cry::GamePlatform
{

class CStatisticsManager
	: public IStatisticsManager
{

	struct SAchievementStorage
	{

		SAchievementStorage(const CryGUID& guid, const string& editorLabel, const string& label, const string& desc, const string& apiname, int apiid, int minProgress, int maxProgress, EGamePlatform platform)
			: m_guid(guid)
			, m_editorLabel(editorLabel)
			, m_label(label)
			, m_desc(desc)
			, m_apiName(apiname)
			, m_apiId(apiid)
			, m_minProgress(minProgress)
			, m_maxProgress(maxProgress)
			, m_platform(platform)
		{}

		bool operator==(const SAchievementStorage& rhs) const
		{
			return m_guid == rhs.m_guid;
		}

		bool operator!=(const SAchievementStorage& rhs) const
		{
			return !(*this == rhs);
		}

		bool operator<(const SAchievementStorage& rhs) const
		{
			return m_guid < rhs.m_guid;
		}

		friend bool operator<(const CryGUID& guid, const SAchievementStorage& rhs)
		{
			return guid < rhs.m_guid;
		}

		friend bool operator<(const SAchievementStorage& lhs, const CryGUID& guid)
		{
			return lhs.m_guid < guid;
		}

		const CryGUID       m_guid;
		const string        m_editorLabel;
		const string        m_label;
		const string        m_desc;
		const string        m_apiName;
		const int           m_apiId;
		const int           m_minProgress;
		const int           m_maxProgress;
		const EGamePlatform m_platform;

	};

	struct SStatisticStorage
	{

		SStatisticStorage(const CryGUID& guid, const string& editorLabel, const string& label, const string& desc, const string& apiName, EGamePlatform platform, EStatDataType statDataType)
			: m_guid(guid)
			, m_editorLabel(editorLabel)
			, m_label(label)
			, m_desc(desc)
			, m_apiName(apiName)
			, m_platform(platform)
			, m_statDataType(statDataType)
		{}

		bool operator==(const SStatisticStorage& rhs) const
		{
			return m_guid == rhs.m_guid;
		}

		bool operator!=(const SStatisticStorage& rhs) const
		{
			return !(*this == rhs);
		}

		bool operator<(const SStatisticStorage& rhs) const
		{
			return m_guid < rhs.m_guid;
		}

		friend bool operator<(const CryGUID& guid, const SStatisticStorage& rhs)
		{
			return guid < rhs.m_guid;
		}

		friend bool operator<(const SStatisticStorage& lhs, const CryGUID& guid)
		{
			return lhs.m_guid < guid;
		}

		const CryGUID       m_guid;
		const string        m_editorLabel;
		const string        m_label;
		const string        m_desc;
		const string        m_apiName;
		const EGamePlatform m_platform;
		const EStatDataType m_statDataType;

	};

public:

	CStatisticsManager();
	~CStatisticsManager();

	void                               Initialize();

	virtual SAchievementData           GetAchievementData(const CryGUID& guid) const override;
	virtual SAchievementData           GetAchievementData(EGamePlatform platform, int apiId) const override;
	virtual SAchievementData           GetAchievementData(EGamePlatform platform, const char* szApiName) const override;
	virtual DynArray<SAchievementData> GetAchievements() const override;
	virtual void                       Visit(AchievementVisitor f) const override;

	virtual SStatisticData             GetStatisticData(const CryGUID& guid) const override;
	virtual SStatisticData             GetStatisticData(EGamePlatform platform, const char* szApiName) const override;
	virtual DynArray<SStatisticData>   GetStatistics() const override;
	virtual void                       Visit(StatisticVisitor f) const override;

protected:

	void ParseAchievementFile();

private:

	inline void WriteAchievementData(const SAchievementStorage& in, SAchievementData& out) const;
	inline void WriteStatisticData(const SStatisticStorage& in, SStatisticData& out) const;

private:

	int m_statDlOnStartup;
	const char* m_achievementFile;
	std::set<SAchievementStorage, std::less<>> m_achievementList;
	std::set<SStatisticStorage, std::less<>> m_statisticList;

};

} // Cry::GamePlatform
