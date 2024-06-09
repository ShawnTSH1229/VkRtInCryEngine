// Copyright 2015-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Tools/BaseTool.h"
#include "Util/SpotManager.h"
#include "Core/Model.h"

namespace Designer
{
class RemoveTool : public BaseTool, public SpotManager
{
public:

	RemoveTool(EDesignerTool tool) : BaseTool(tool)
	{
	}

	void        Enter();

	static bool RemoveSelectedElements(MainContext& mc, bool bEraseMirrored);

private:

	bool RemoveSelectedElements();
};
}
