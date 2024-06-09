// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <CrySerialization/Forward.h>

namespace FbxTool
{
struct SNode;
struct SMesh;
}

struct SNodeInfo
{
	const FbxTool::SNode* const pNode;

	void Serialize(Serialization::IArchive& ar);
};

struct SMeshInfo
{
	const FbxTool::SMesh* const pMesh;

	void Serialize(Serialization::IArchive& ar);
};
