// Copyright 2015-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

struct SDocumentSettings
{
	SDocumentSettings()
		: bUseSelectionHelpers(true)
		, bShowInputParamTypes(true)
		, bFilterAvailableInputsByType(true)
	{}

	bool operator==(const SDocumentSettings& other) const;
	bool operator!=(const SDocumentSettings& other) const;

	bool bUseSelectionHelpers;
	bool bShowInputParamTypes;
	bool bFilterAvailableInputsByType;
};
