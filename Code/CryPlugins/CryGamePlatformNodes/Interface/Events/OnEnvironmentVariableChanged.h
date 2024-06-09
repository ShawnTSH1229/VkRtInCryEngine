// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "BaseContext.h"
#include <CrySchematyc/Utils/SharedString.h>

namespace Cry::GamePlatform
{

class COnEnvironmentVariableChanged : public CBaseEventContext
{

public:

	static constexpr EPlatformEvent EventType = EPlatformEvent::OnEnvironmentVariableChanged;

	COnEnvironmentVariableChanged()
		: CBaseEventContext(EGamePlatform::Main, EventType)
		, m_varName()
		, m_newValue()
	{}

	COnEnvironmentVariableChanged(EGamePlatform platform, const char* szVarName, const char* szNewValue)
		: CBaseEventContext(platform, EventType)
		, m_varName(szVarName)
		, m_newValue(szNewValue)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<COnEnvironmentVariableChanged>& typeDesc)
	{
		typeDesc.SetGUID("{401F0F7F-B1D9-40A3-8B2D-38C045E4A8EF}"_cry_guid);
		typeDesc.SetLabel("Service::OnEnvironmentVariableChanged");
		typeDesc.AddMember(&COnEnvironmentVariableChanged::m_varName, 'name', "name", "Name", nullptr, "");
		typeDesc.AddMember(&COnEnvironmentVariableChanged::m_newValue, 'val', "value", "Value", nullptr, "");
	}

	const Schematyc::CSharedString& GetName() const
	{
		return m_varName;
	}

	const Schematyc::CSharedString& GetValue() const
	{
		return m_newValue;
	}

private:

	Schematyc::CSharedString m_varName;
	Schematyc::CSharedString m_newValue;

};

} // Cry::GamePlatform