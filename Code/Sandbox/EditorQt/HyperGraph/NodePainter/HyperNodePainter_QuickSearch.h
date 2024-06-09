// Copyright 2011-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IHyperNodePainter.h"

class CHyperNodePainter_QuickSearch : public IHyperNodePainter
{
public:
	virtual void Paint(CHyperNode* pNode, CDisplayList* pList);
};
