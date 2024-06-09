// Copyright 2013-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "../SequencerDopeSheet.h"

class CMannDopeSheet : public CSequencerDopeSheet
{
	DECLARE_DYNAMIC(CMannDopeSheet)

public:
	bool IsDraggingTime() const
	{
		//--- Dammit
		return m_mouseMode == 4;
	}

	float GetTime() const
	{
		return m_currentTime;
	}
};
