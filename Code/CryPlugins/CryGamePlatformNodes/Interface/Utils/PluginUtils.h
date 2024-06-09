// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/ICryPlugin.h>

namespace Cry::GamePlatform
{

// Helper to get plugin instance only if available
template<class PLUGIN>
class CPluginHelper : public IPluginManager::IEventListener
{
	static_assert(std::is_convertible<PLUGIN&, Cry::IEnginePlugin&>::value, "Template argument must be an IEnginePlugin");

	friend class CListener;
	class CListener : public IPluginManager::IEventListener
	{

	public:

		~CListener()
		{
			if (s_isListenerRegistered)
			{
				if (IPluginManager* pPluginManager = gEnv->pSystem->GetIPluginManager())
				{
					pPluginManager->RemoveEventListener<PLUGIN>(this);
				}
			}
		}

		virtual void OnPluginEvent(const CryClassID& pluginClassId, EEvent event) override
		{
			IPluginManager* pPluginManager = gEnv->pSystem->GetIPluginManager();

			if (event == EEvent::Initialized)
			{
				CPluginHelper::s_pPlugin = pPluginManager->QueryPlugin<PLUGIN>();
			}
			else
			{
				CPluginHelper::s_pPlugin = nullptr;
			}
		}

	};

public:

	static PLUGIN* Get()
	{
		if (!s_isListenerRegistered)
		{
			if (IPluginManager* pPluginManager = gEnv->pSystem->GetIPluginManager())
			{
				s_pPlugin = pPluginManager->QueryPlugin<PLUGIN>();

				pPluginManager->RegisterEventListener<PLUGIN>(&s_listener);

				s_isListenerRegistered = true;
			}
		}

		return s_pPlugin;
	}

private:

	static PLUGIN*   s_pPlugin;
	static bool      s_isListenerRegistered;
	static CListener s_listener;
};

template<class PLUGIN> PLUGIN* CPluginHelper<PLUGIN>::s_pPlugin = nullptr;
template<class PLUGIN> bool CPluginHelper<PLUGIN>::s_isListenerRegistered = false ;
template<class PLUGIN> typename CPluginHelper<PLUGIN>::CListener CPluginHelper<PLUGIN>::s_listener = typename CPluginHelper<PLUGIN>::CListener();

} // Cry::GamePlatform
