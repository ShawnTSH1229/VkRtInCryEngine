// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySchematyc/Reflection/TypeDesc.h>

namespace Cry::GamePlatform
{

// If you add an enum here, make sure you update the string list below
enum class EPlatformEvent : uint32
{
	// General service events
	OnAccountAdded,
	OnAccountNameChanged,
	OnAccountRemoved,
	OnAuthTokenReceived,
	OnAvatarImageLoaded,
	OnCatalogItemsReceived,
	OnCatalogReceived,
	OnEnvironmentVariableChanged,
	OnGetSteamAuthTicketResponse,
	OnInventoryItemConsumed,
	OnInventoryReceived,
	OnLicensesChanged,
	OnLogInStateChanged,
	OnMicroTransactionAuthorizationResponse,
	OnNetworkStatusChanged,
	OnOverlayActivated,
	OnPersonaStateChanged,
	OnPlatformServiceError,
	OnShutdown,
	OnUserPermissionChanged,

	// Leaderboard events
	OnLeaderboardEntryDownloaded,

	// Matchmaking events
	OnLobbyQueryComplete,
	OnCreatedLobby,
	OnJoinedLobby,
	OnLobbyJoinFailed,
	OnPreJoinLobby,

	// Lobby events
	OnPlayerEntered,
	OnPlayerLeft,
	OnPlayerDisconnected,
	OnPlayerKicked,
	OnPlayerBanned,
	OnLeave,
	OnGameCreated,
	OnChatMessage,
	OnLobbyDataUpdate,
	OnUserDataUpdate,

	// Remote Storage events
	OnFileShared,
	OnDownloadedSharedFile,

	// User Generated Content events
	OnContentCreated,

	// Custom Events
	OnTextFiltered,
	OnCheckMultiplayerAccess,

	Last = OnCheckMultiplayerAccess, // <- Must be last
	Count = Last+1
};

class CPlatformEventFlags
{

// False positive with GCC < 4.8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

	static constexpr std::array<const char*, static_cast<size_t>(EPlatformEvent::Count)> s_events =
	{
		"OnAccountAdded",
		"OnAccountNameChanged",
		"OnAccountRemoved",
		"OnAuthTokenReceived",
		"OnAvatarImageLoaded",
		"OnCatalogItemsReceived",
		"OnCatalogReceived",
		"OnChatMessage",
		"OnCheckMultiplayerAccess",
		"OnContentCreated",
		"OnCreatedLobby",
		"OnDownloadedSharedFile",
		"OnEnvironmentVariableChanged",
		"OnFileShared",
		"OnGameCreated",
		"OnGetSteamAuthTicketResponse",
		"OnInventoryItemConsumed",
		"OnInventoryReceived",
		"OnJoinedLobby",
		"OnLeaderboardEntryDownloaded",
		"OnLeave",
		"OnLicensesChanged",
		"OnLobbyDataUpdate",
		"OnLobbyJoinFailed",
		"OnLobbyQueryComplete",
		"OnLogInStateChanged",
		"OnMicroTransactionAuthorizationResponse",
		"OnNetworkStatusChanged",
		"OnOverlayActivated",
		"OnPersonaStateChanged",
		"OnPlatformServiceError",
		"OnPlayerBanned",
		"OnPlayerDisconnected",
		"OnPlayerEntered",
		"OnPlayerKicked",
		"OnPlayerLeft",
		"OnPreJoinLobby",
		"OnShutdown",
		"OnTextFiltered",
		"OnUserDataUpdate",
		"OnUserPermissionChanged",
	};

#pragma GCC diagnostic pop

	static_assert(s_events.size() == static_cast<size_t>(EPlatformEvent::Count), "Event string table size mis-match.");

public:

	CPlatformEventFlags() = default;

	static void ReflectType(Schematyc::CTypeDesc<CPlatformEventFlags>& typeDesc)
	{
		typeDesc.SetGUID("{BCB29876-955E-4BB1-AD33-297C226A1658}"_cry_guid);
		typeDesc.SetLabel("PlatformEventFlags");
		typeDesc.SetDescription("Flags used to determine which events will be received.");
	}

	void Serialize(Serialization::IArchive& ar)
	{
		const bool prevAll = m_flags.all();
		const bool prevNone = !m_flags.any();

		for (size_t index = 0; index < s_events.size(); index++)
		{
			bool value = Check(index);
			ar(value, s_events[index], s_events[index]);

			if (ar.isInput())
			{
				if (value) Set(index);
				else Unset(index);
			}
		}

		if (ar.isEdit())
		{
			if (ar.isInput())
			{
				bool all;
				bool none;

				ar(all, "All", "All");
				ar(none, "None", "None");

				if (all && !prevAll) m_flags.set();
				else if (none && !prevNone) m_flags.reset();
			}
			else if(ar.isOutput())
			{
				bool all = m_flags.all();
				bool none = m_flags.all() ? false : !m_flags.any();

				ar(all, "All", "All");
				ar(none, "None", "None");
			}
		}
	}

	bool Any() const
	{
		return m_flags.any();
	}

	bool All() const
	{
		return m_flags.all();
	}

	bool CheckAny(const CPlatformEventFlags& checkFlags) const
	{
		return (m_flags & checkFlags.m_flags).any();
	}

	bool CheckAll(const CPlatformEventFlags& checkFlags) const
	{
		return (m_flags & checkFlags.m_flags) == checkFlags.m_flags;
	}

	bool Check(size_t index) const
	{
		return m_flags.test(index);
	}

	size_t NumFlags() const
	{
		return m_flags.size();
	}

	size_t NumSet() const
	{
		return m_flags.count();
	}

	void Add(const CPlatformEventFlags& other)
	{
		m_flags &= other.m_flags;
	}

	void Remove(const CPlatformEventFlags& other)
	{
		m_flags &= ~other.m_flags;
	}

	void Set(size_t index)
	{
		if (index < m_flags.size())
			m_flags.set(index);
	}

	void Unset(size_t index)
	{
		if (index < m_flags.size())
			m_flags.reset(index);
	}

	void SetAll()
	{
		m_flags.set();
	}

	void Reset()
	{
		m_flags.reset();
	}

	bool operator==(const CPlatformEventFlags& other) const
	{
		return m_flags == other.m_flags;
	}

	bool operator!=(const CPlatformEventFlags& other) const
	{
		return m_flags != other.m_flags;
	}

private:

	std::bitset<static_cast<size_t>(EPlatformEvent::Count)> m_flags;

};

} // Cry::GamePlatform