// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <Types/Leaderboard.h>
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

class COnLeaderboardEntryDownloaded : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnLeaderboardEntryDownloaded;

	COnLeaderboardEntryDownloaded()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_leaderboardId()
		, m_playerName("")
		, m_rank(0)
		, m_score(0)
		, m_scoreType(ILeaderboards::EScoreType::Unknown)
		, m_bCurrentUser(false)
	{}

	COnLeaderboardEntryDownloaded(EGamePlatform platform, const LeaderboardIdentifier& leaderboardId, const char* szPlayerName, int rank, int score, ILeaderboards::EScoreType scoreType, bool bCurrentUser)
		: CBaseEventContext(platform, EventType)
		, m_leaderboardId(leaderboardId)
		, m_playerName(szPlayerName)
		, m_rank(rank)
		, m_score(score)
		, m_scoreType(scoreType)
		, m_bCurrentUser(bCurrentUser)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnLeaderboardEntryDownloaded>& typeDesc)
	{
		typeDesc.SetGUID("{700ACE55-DA72-4CB6-BB7B-255784605579}"_cry_guid);
		typeDesc.SetLabel("Leaderboard::OnLeaderboardEntryDownloaded");
		typeDesc.AddMember(&COnLeaderboardEntryDownloaded::m_leaderboardId, 'lid', "leaderboardid", "Leaderboard Id", nullptr, LeaderboardIdentifier());
		typeDesc.AddMember(&COnLeaderboardEntryDownloaded::m_playerName, 'pnm', "playername", "Player Name", nullptr, "");
		typeDesc.AddMember(&COnLeaderboardEntryDownloaded::m_rank, 'rank', "rank", "Rank", nullptr, 0);
		typeDesc.AddMember(&COnLeaderboardEntryDownloaded::m_score, 'scor', "score", "Score", nullptr, 0);
		typeDesc.AddMember(&COnLeaderboardEntryDownloaded::m_scoreType, 'sctp', "scoretype", "Score Type", nullptr, ILeaderboards::EScoreType::Unknown);
		typeDesc.AddMember(&COnLeaderboardEntryDownloaded::m_bCurrentUser, 'cusr', "currentuser", "Is Current User", nullptr, false);
	}

	const LeaderboardIdentifier& GetIdentifier() const
	{
		return m_leaderboardId;
	}

	const Schematyc::CSharedString& GetPlayerName() const
	{
		return m_playerName;
	}

	int GetRank() const
	{
		return m_rank;
	}

	int GetScore() const
	{
		return m_score;
	}

	ILeaderboards::EScoreType GetScoreType() const
	{
		return m_scoreType;
	}

	bool IsCurrentUser() const
	{
		return m_bCurrentUser;
	}

private:

	LeaderboardIdentifier     m_leaderboardId;
	Schematyc::CSharedString  m_playerName;
	int                       m_rank;
	int                       m_score;
	ILeaderboards::EScoreType m_scoreType;
	bool                      m_bCurrentUser;

};

} // Cry::GamePlatform