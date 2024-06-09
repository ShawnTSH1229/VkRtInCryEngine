// Copyright 2015-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "CylinderTool.h"

namespace Designer
{
class ConeTool : public CylinderTool
{
public:

	ConeTool(EDesignerTool tool) : CylinderTool(tool)
	{
	}
	~ConeTool(){}

private:

	void UpdateShape(float fHeight) override;
};
}
