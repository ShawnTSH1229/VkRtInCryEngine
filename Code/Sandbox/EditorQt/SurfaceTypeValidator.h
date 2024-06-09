// Copyright 2006-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

struct pe_params_part;

class CSurfaceTypeValidator
{
public:
	void Validate();

private:
	void GetUsedSubMaterials(pe_params_part* pPart, char usedSubMaterials[]);
};
