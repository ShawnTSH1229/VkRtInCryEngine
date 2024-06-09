// Copyright 2010-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Nodes/TrackViewAnimNode.h"

class CLayerNodeAnimator : public IAnimNodeAnimator
{
public:
	void Animate(CTrackViewAnimNode* pNode, const SAnimContext& ac);

protected:
	virtual ~CLayerNodeAnimator() {}
};
