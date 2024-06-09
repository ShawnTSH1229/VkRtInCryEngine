// Copyright 2007-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

class CFacialAnimChannelInterpolator;

namespace FaceChannel
{
void GaussianBlurKeys(CFacialAnimChannelInterpolator* pSpline, float sigma);
void RemoveNoise(CFacialAnimChannelInterpolator* pSpline, float sigma, float threshold);
}
