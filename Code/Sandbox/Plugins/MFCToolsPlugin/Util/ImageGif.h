// Copyright 2004-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

class CImageEx;

class CImageGif
{
public:
	bool Load(const string& fileName, CImageEx& outImage);
};
