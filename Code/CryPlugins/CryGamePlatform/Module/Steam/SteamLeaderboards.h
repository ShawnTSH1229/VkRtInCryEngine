// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformLeaderboards.h"
#include "SteamTypes.h"
#include <steam/isteamuserstats.h>

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			class CLeaderboards final : public ILeaderboards
			{
			public:
				struct SQueuedEntryRequest
				{
					ELeaderboardDataRequest request;
					int minRange;
					int maxRange;
				};

				struct SQueuedUpdateRequest
				{
					ELeaderboardUploadScoreMethod method;
					int score;
					EScoreType scoreType;
					string name;
				};

				explicit CLeaderboards(CService& steamService);
				virtual ~CLeaderboards() = default;

				// ILeaderboards
				virtual void AddListener(IListener& listener, const char* szName) override { m_listeners.Add(&listener, szName); }
				virtual void RemoveListener(IListener& listener) override { m_listeners.Remove(&listener); }

				virtual void DownloadEntries(const LeaderboardIdentifier& leaderboardId, ERequest request = ERequest::Global, int minRange = 1, int maxRange = 10, bool bTimeBased = false) override;
				virtual void UpdateScore(const LeaderboardIdentifier& leaderboardId, int score, EScoreType scoreType, bool bForce = false) override;
				// ~ILeaderboards

			private:
				void FindOrCreateLeaderboard(const char* name, ELeaderboardSortMethod sortMethod = k_ELeaderboardSortMethodDescending, ELeaderboardDisplayType displayType = k_ELeaderboardDisplayTypeNumeric);
				void FindNextLeaderboardToUpdateScore();

				void OnFindLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure);
				CCallResult<CLeaderboards, LeaderboardFindResult_t> m_callResultFindLeaderboard;

				void OnEntriesDownloaded(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure);
				CCallResult<CLeaderboards, LeaderboardScoresDownloaded_t> m_callResultEntriesDownloaded;

			protected:
				CService& m_service;

				std::unique_ptr<SQueuedEntryRequest> m_pQueuedEntryRequest;
				std::vector<SQueuedUpdateRequest> m_queuedUpdateRequests;
				
				CListenerSet<IListener*> m_listeners = 1;
			};
		}
	}
}