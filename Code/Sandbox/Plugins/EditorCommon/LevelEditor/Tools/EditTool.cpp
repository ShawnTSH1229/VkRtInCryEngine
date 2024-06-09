// Copyright 2004-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "EditTool.h"
#include "LevelEditor/LevelEditorSharedState.h"
#include <IEditor.h>

IMPLEMENT_DYNAMIC(CEditTool, CObject);

CEditTool::CEditTool()
	: m_nRefCount(0)
{
}

void CEditTool::Abort()
{
	GetIEditor()->GetLevelEditorSharedState()->SetEditTool(nullptr);
}
