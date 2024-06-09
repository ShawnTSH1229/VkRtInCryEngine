// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CryGamePlatform/Interface/IPlatformLeaderboards.h>

namespace Cry::GamePlatform
{

inline void ReflectType(Schematyc::CTypeDesc<LeaderboardIdentifier>& typeDesc)
{
	typeDesc.SetGUID("{9E38E96D-B6CA-4F38-9B00-5F0FA3B5F5DB}"_cry_guid);
	typeDesc.SetLabel("Leaderboard");
	typeDesc.SetDescription("An identifier for a platform-specific leaderboard");
}

inline void ReflectType(Schematyc::CTypeDesc<ILeaderboards::ERequest>& typeDesc)
{
	typeDesc.SetGUID("{41EEFDF8-D3E1-413A-B2E8-7BF372246B62}"_cry_guid);
	typeDesc.SetLabel("RequestType");
	typeDesc.SetDescription("Type of leaderboard request");
	typeDesc.SetDefaultValue(ILeaderboards::ERequest::Global);
	typeDesc.AddConstant(ILeaderboards::ERequest::Global, "global", "Global");
	typeDesc.AddConstant(ILeaderboards::ERequest::GlobalRelativeToUser, "globalrelative", "GlobalRelativeToUser");
	typeDesc.AddConstant(ILeaderboards::ERequest::Friends, "friends", "Friends");
	typeDesc.AddConstant(ILeaderboards::ERequest::Users, "users", "Users");
}

inline void ReflectType(Schematyc::CTypeDesc<ILeaderboards::EScoreType>& typeDesc)
{
	typeDesc.SetGUID("{4766E5A8-BAAC-4352-8838-51B4F058697D}"_cry_guid);
	typeDesc.SetLabel("ScoreType");
	typeDesc.SetDescription("Determines the type of score that we are uploading to a leaderboard");
	typeDesc.SetDefaultValue(ILeaderboards::EScoreType::Unknown);
	typeDesc.AddConstant(ILeaderboards::EScoreType::Score, "score", "Score");
	typeDesc.AddConstant(ILeaderboards::EScoreType::Time, "time", "Time");
	typeDesc.AddConstant(ILeaderboards::EScoreType::Unknown, "unknown", "Unknown");
}

} // Cry::GamePlatform