// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Events/BaseContext.h>
#include <Types/Lobby.h>
#include <Types/RemoteStorage.h>

namespace Cry::GamePlatform
{

struct IPlatformEventListener;

class IPlatformEventManager
{

public:

	virtual ~IPlatformEventManager() {}

	virtual void AddListener(EGamePlatform platform, IPlatformEventListener* pListener) = 0;
	virtual void RemoveListener(EGamePlatform platform, IPlatformEventListener* pListener) = 0;

	virtual void StartListening(const LobbyIdentifier& lobbyId) = 0;
	virtual void StopListening(const LobbyIdentifier& lobbyId) = 0;

	virtual void StartListening(const CCachedLocal& localCache) = 0;
	virtual void StopListening(const CCachedLocal& localCache) = 0;

	virtual void StartListening(const CCachedShared& sharedCache) = 0;
	virtual void StopListening(const CCachedShared& sharedCache) = 0;

	virtual void HandleEvent(const CBaseEventContext& context) = 0;

};

struct IPlatformEventListener
{

	virtual void                OnPlatformEvent(const CBaseEventContext& context) = 0;
	virtual CPlatformEventFlags GetPlatformEventMask() const = 0;

};

} // Cry::GamePlatform