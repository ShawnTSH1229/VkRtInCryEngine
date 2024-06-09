// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include "EditorCommonAPI.h"

//! This class is responsible for initializing VCS.
class EDITOR_COMMON_API CVersionControlInitializer
{
public:
	static void ActivateListeners();

	static void DeactivateListeners();

	static void Initialize();
};
