// Copyright 2013-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

struct SRenderContext;
struct SKeyEvent;
struct SMouseEvent;

class QViewportConsumer
{
public:
	virtual void OnViewportRender(const SRenderContext& rc) {}
	virtual void OnViewportKey(const SKeyEvent& ev)         {}
	virtual void OnViewportMouse(const SMouseEvent& ev)     {}
};
