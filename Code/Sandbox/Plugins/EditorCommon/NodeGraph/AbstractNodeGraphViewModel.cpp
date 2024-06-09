// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "stdafx.h"
#include "AbstractNodeGraphViewModel.h"

namespace CryGraphEditor {

CNodeGraphViewModel::~CNodeGraphViewModel()
{
	SignalDestruction();
}

} // namespace CryGraphEditor
