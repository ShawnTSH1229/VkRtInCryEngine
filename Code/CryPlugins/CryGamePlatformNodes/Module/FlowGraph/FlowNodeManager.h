// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include "FlowNodeFactory.h"

namespace Cry::GamePlatform
{

class CFlowNodeManager
{

public:

	CFlowNodeManager();
	~CFlowNodeManager();

	void Initialize();

protected:

	void CreateDynamicNodes();
	void RegisterDynamicNodes();
	void UnregisterDynamicNodes();

private:

	CStatisticFlowNodeFactory_AutoArray m_UINodes;
};

} // Cry::GamePlatform
