// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformLobby.h"
#include "SteamTypes.h"

namespace Cry
{
	namespace GamePlatform
	{
		namespace Steam
		{
			class CUserLobby
				: public IUserLobby
				, public ISystemEventListener
			{
			public:
				CUserLobby(CService& steamService, const LobbyIdentifier& lobbyId);
				virtual ~CUserLobby();

				// IUserLobby
				virtual void AddListener(IListener& listener, const char* szName) override { m_listeners.Add(&listener, szName); }
				virtual void RemoveListener(IListener& listener) override { m_listeners.Remove(&listener); }

				virtual bool HostServer(const char* szLevel, bool isLocal) override;

				virtual int GetMemberLimit() const override;
				virtual int GetNumMembers() const override;
				virtual AccountIdentifier GetMemberAtIndex(int index) const override;

				virtual bool IsInServer() const override { return m_serverIP != 0; }
				virtual void Leave() override;

				virtual AccountIdentifier GetOwnerId() const override;
				virtual LobbyIdentifier GetIdentifier() const override { return CreateLobbyIdentifier(m_steamLobbyId); }

				virtual bool SendChatMessage(const char* message) const override;

				virtual void ShowInviteDialog() const override;
				virtual bool InvitePlayers(const DynArray<AccountIdentifier>& invitees) override;

				virtual bool SetData(const char* key, const char* value) override;
				virtual const char* GetData(const char* key) const override;
				virtual const char* GetMemberData(const AccountIdentifier& userId, const char* szKey) override;
				virtual void SetMemberData(const char* szKey, const char* szValue) override;
				// ~IUserLobby

				// ISystemEventListener
				virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
				// ~ISystemEventListener

				void ConnectToServer(uint32 ip, uint16 port, IServer::Identifier serverId);
				
			protected:
				STEAM_CALLBACK(CUserLobby, OnLobbyChatUpdate, LobbyChatUpdate_t, m_callbackChatDataUpdate);
				STEAM_CALLBACK(CUserLobby, OnLobbyDataUpdate, LobbyDataUpdate_t, m_callbackDataUpdate);

				STEAM_CALLBACK(CUserLobby, OnLobbyChatMessage, LobbyChatMsg_t, m_callbackChatMessage);

				STEAM_CALLBACK(CUserLobby, OnLobbyGameCreated, LobbyGameCreated_t, m_callbackGameCreated);

				CService& m_service;

				CListenerSet<IListener*> m_listeners = 1;
				CSteamID m_steamLobbyId;

				uint32 m_serverIP;
				uint16 m_serverPort;

				IServer::Identifier m_serverId;
			};
		}
	}
}