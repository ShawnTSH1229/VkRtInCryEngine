// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "MarkupVolume.h"

namespace MNM
{

void SMarkupVolume::Swap(SMarkupVolume& other)
{
	BoundingVolume::Swap(other);

	std::swap(areaAnnotation, other.areaAnnotation);
	std::swap(bStoreTriangles, other.bStoreTriangles);
	std::swap(bExpandByAgentRadius, other.bExpandByAgentRadius);
}

}
