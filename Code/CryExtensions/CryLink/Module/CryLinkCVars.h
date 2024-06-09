// Copyright 2015-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

namespace CryLinkService 
{
	class CVars
	{
	public:
		static CVars& GetInstance() { return m_instance; }

		void Register();
		void Unregister();

	private: 
		CVars();
		~CVars();

	public:
		ICVar* cryLinkAllowedClient;
		ICVar* cryLinkServicePassword;
		int32 cryLinkEditorServicePort;
		int32 cryLinkGameServicePort;

	private: 
		static CVars m_instance;

		bool m_bRegistered;
	};
}