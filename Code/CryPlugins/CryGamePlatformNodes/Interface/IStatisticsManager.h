// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Types/AchievementData.h"
#include "Types/StatisticData.h"

namespace Cry::GamePlatform
{

class IStatisticsManager
{

public:

	using AchievementVisitor = std::function<void(const SAchievementData&)>;
	using StatisticVisitor = std::function<void(const SStatisticData&)>;

public:

	virtual SAchievementData           GetAchievementData(const CryGUID& guid) const = 0;
	virtual SAchievementData           GetAchievementData(EGamePlatform platform, int apiId) const = 0;
	virtual SAchievementData           GetAchievementData(EGamePlatform platform, const char* apiName) const = 0;
	virtual DynArray<SAchievementData> GetAchievements() const = 0;
	virtual void                       Visit(AchievementVisitor f) const = 0;

	virtual SStatisticData             GetStatisticData(const CryGUID& guid) const = 0;
	virtual SStatisticData             GetStatisticData(EGamePlatform platform, const char* apiName) const = 0;
	virtual DynArray<SStatisticData>   GetStatistics() const = 0;
	virtual void                       Visit(StatisticVisitor f) const = 0;

};

} // Cry::GamePlatform
