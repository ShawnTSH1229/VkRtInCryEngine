// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include "EditorCommonAPI.h"

class EDITOR_COMMON_API CScopedRenderLock
{
public:
	CScopedRenderLock();
	~CScopedRenderLock();
	explicit operator bool();

	static void Lock();
	static void Unlock();

private:
	bool m_bOwnsLock;

	static int s_renderLock;
};
