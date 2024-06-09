// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Events/BaseContext.h>
#include <IPlatformEventManager.h>
#include <Types/RemoteStorage.h>
#include <CryEntitySystem/IEntityComponent.h>

namespace Cry::GamePlatform
{

class CPlatformSignalComponent
	: public IEntityComponent
	, public IPlatformEventListener
{

public:

	CPlatformSignalComponent() = default;
	virtual ~CPlatformSignalComponent() override;

	static inline void ReflectType(Schematyc::CTypeDesc<CPlatformSignalComponent>& typeDesc)
	{
		typeDesc.SetGUID("{6D18520C-60C8-4F8E-A82F-C14772467ECF}"_cry_guid);
		typeDesc.SetLabel("Platform Signal Receiver");
		typeDesc.SetEditorCategory("Game Platform");
		typeDesc.SetComponentFlags({ IEntityComponent::EFlags::HideFromInspector });
		typeDesc.AddMember(&CPlatformSignalComponent::m_platform, 'plat', "platform", "Platform", "Platform of which to register event listeners for", EGamePlatform::Main);
		typeDesc.AddMember(&CPlatformSignalComponent::m_events, 'evts', "events", "Events", "Events to handle via this component", CPlatformEventFlags());
	}

	// IPlatformEventListener
	virtual void OnPlatformEvent(const CBaseEventContext& context) override;
	virtual CPlatformEventFlags GetPlatformEventMask() const override;
	// ~IPlatformEventListener

	// Schematyc Nodes
	void StartLobbyListener(const LobbyIdentifier& account);
	void StopLobbyListener(const LobbyIdentifier& account);
	void StartRemoteFileListener(const CCachedLocal& localCache);
	void StopRemoteFileListener(const CCachedLocal& localCache);
	void StartSharedFileListener(const CCachedShared& sharedCache);
	void StopSharedFileListener(const CCachedShared& sharedCache);
	// ~Schematyc Nodes

	void SetEnabled(const bool enable);
	void SetPlatform(EGamePlatform platform);
	void SetPlatformEventMask(const CPlatformEventFlags& events);

protected:

	// IEntityComponent
	virtual void            Initialize() override;
	virtual EntityEventMask GetEventMask() const override;
	virtual void            ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent
	void UnregisterListeners();

private:

	bool                m_isEnabled = true;
	EGamePlatform       m_platform = EGamePlatform::Main;
	CPlatformEventFlags m_events{};

	std::set<LobbyIdentifier> m_registeredLobbyListeners;
	std::set<CCachedLocal>    m_registeredRemoteFileListeners;
	std::set<CCachedShared>   m_registeredSharedFileListeners;
};

} // Cry::GamePlatform