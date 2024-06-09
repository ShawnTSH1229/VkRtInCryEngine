// Copyright 2015-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Tools/BaseTool.h"

namespace Designer
{
class ExportTool : public BaseTool
{
public:

	ExportTool(EDesignerTool tool) : BaseTool(tool)
	{
	}

	void ExportToCgf();
	void ExportToGrp();
	void ExportToObj();

	void Serialize(Serialization::IArchive& ar);
};
}
