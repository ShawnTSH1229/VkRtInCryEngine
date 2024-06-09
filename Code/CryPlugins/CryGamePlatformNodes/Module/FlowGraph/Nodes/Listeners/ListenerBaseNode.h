// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <IPlugin.h>
#include <IPlatformEventManager.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryFlowGraph/IFlowBaseNode.h>
#include <queue>

namespace Cry::GamePlatform
{

static const std::vector<SInputPortConfig> s_listenerInputConfiguration = {
	InputPortConfig_Void("GetNextEvent", _HELP("Gets the next event data. If there is no event data queued only the EventsLeft output will fire with a value of 0.")),
	InputPortConfig<int>("Platform", 0, _HELP("The platform this node is listening to."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
	{ 0 }
};

template<class LISTENER_TYPE>
class CFlowListenerBaseNode
	: public CFlowBaseNode<eNCT_Instanced>
	, public IPlatformEventListener
{

protected:

	using Event = LISTENER_TYPE;

	enum EInputPorts
	{
		eIn_GetNext,
		eIn_Platform
	};

	enum EOutputPortsBase
	{
		eOut_NumEventsLeft,
		eOut_CustomOutput
	};

public:

	CFlowListenerBaseNode(SActivationInfo*)
		: m_platform(EGamePlatform::None)
		, m_queue()
		, m_isRegistered(false)
	{
		m_outputs.reserve(3);
	}

	virtual ~CFlowListenerBaseNode()
	{
		if (m_isRegistered)
		{
			if (IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get())
			{
				if (IPlatformEventManager* pManager = pPlugin->GetPlatformEventManager())
				{
					pManager->RemoveListener(m_platform, this);
				}
			}
		}
	}

	// Returns true if the parameter were configured here (not configured before)
	bool ConfigureBaseParameters()
	{
		static bool isConfigured = false;

		if (isConfigured)
			return false;

		m_outputs = {
			OutputPortConfig<int>("EventsLeft", _HELP("The number of queued events left in this node."))
		};

		isConfigured = true;

		return true;
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		if (event == eFE_Initialize)
		{
			IPlugin_GamePlatformNodes* pPlugin = CPluginHelper<IPlugin_GamePlatformNodes>::Get();
			IPlatformEventManager* pManager = pPlugin->GetPlatformEventManager();

			const EGamePlatform oldPlatform = m_platform;
			m_platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (m_isRegistered && m_platform != oldPlatform)
			{
				pManager->RemoveListener(oldPlatform, this);
				m_isRegistered = false;
			}

			if (!m_isRegistered)
			{
				pManager->AddListener(m_platform, this);
				m_isRegistered = true;
			}

			pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, true);
		}
		else if (event == eFE_Activate && IsPortActive(pActInfo, eIn_GetNext))
		{
			TryEvent(pActInfo);
		}
		else if (event == eFE_Update)
		{
			TryEvent(pActInfo);
		}
	};

	// IPlatformEventListener
	virtual void OnPlatformEvent(const CBaseEventContext& context) override
	{
		CRY_ASSERT(Helper::IsMatchingService(context.GetPlatform(), m_platform), "Event platform (%d) does not match node platform (%d)", context.GetPlatform(), m_platform);
		m_queue.emplace(static_cast<const Event&>(context));
	}

	virtual CPlatformEventFlags GetPlatformEventMask() const override 
	{ 
		static CPlatformEventFlags flags;
		if (!flags.Any())
			flags.Set(static_cast<size_t>(Event::EventType));

		return flags; 
	}
	// ~IPlatformEventListener

protected:

	virtual void HandleEvent(SActivationInfo* pActInfo, const Event& event) = 0;

private:

	void TryEvent(SActivationInfo* pActInfo)
	{
		if (m_queue.size())
		{
			auto event = m_queue.front();
			HandleEvent(pActInfo, event);
			m_queue.pop();
		}
	}

protected:

	EGamePlatform m_platform;
	bool m_isRegistered;

	// Minimum size of 3: Default output + custom output + empty output
	static inline std::vector<SOutputPortConfig> m_outputs{};

private:

	std::queue<Event> m_queue;
};

} // Cry::GamePlatform
