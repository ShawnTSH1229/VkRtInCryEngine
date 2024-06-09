// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include "CryString/CryString.h"

enum class EConflictResolution
{
    None,
    Their,
    Ours
};

using SVersionControlFileConflictStatus = std::pair<string, EConflictResolution>;
