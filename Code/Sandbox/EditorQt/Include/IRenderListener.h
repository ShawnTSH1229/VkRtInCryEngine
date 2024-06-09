// Copyright 2009-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

struct SDisplayContext;

struct IRenderListener
{
	virtual void Render(SDisplayContext& rDisplayContext) = 0;
	virtual ~IRenderListener() {}
};