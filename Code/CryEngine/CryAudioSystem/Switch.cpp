// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "stdafx.h"
#include "Switch.h"
#include "SwitchState.h"

namespace CryAudio
{
//////////////////////////////////////////////////////////////////////////
CSwitch::~CSwitch()
{
	for (auto const& statePair : m_states)
	{
		delete statePair.second;
	}
}
} // namespace CryAudio
