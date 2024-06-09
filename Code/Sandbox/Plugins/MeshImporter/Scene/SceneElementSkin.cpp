// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "SceneElementSkin.h"
#include "SceneElementTypes.h"


CSceneElementSkin::CSceneElementSkin(CScene* pScene, int id)
	: CSceneElementCommon(pScene, id)
{
}

ESceneElementType CSceneElementSkin::GetType() const
{
	return ESceneElementType::Skin;
}
