// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "FlowNodeManager.h"
#include "CreateStatisticNodes.h"

namespace Cry::GamePlatform
{

CFlowNodeManager::CFlowNodeManager()
	: m_UINodes()
{
}

CFlowNodeManager::~CFlowNodeManager()
{
	UnregisterDynamicNodes();
	m_UINodes.clear();
}

void CFlowNodeManager::Initialize()
{
	CreateDynamicNodes();
	RegisterDynamicNodes();
}

void CFlowNodeManager::CreateDynamicNodes()
{
	CreateStatisticNodes(m_UINodes);
}

void CFlowNodeManager::RegisterDynamicNodes()
{
	for (auto pNodeFactory : m_UINodes)
	{
		gEnv->pFlowSystem->RegisterType(pNodeFactory->GetNodeTypeName(), pNodeFactory);
	}
}

void CFlowNodeManager::UnregisterDynamicNodes()
{
	if (gEnv->pFlowSystem != nullptr)
	{
		for (auto pNodeFactory : m_UINodes)
		{
			gEnv->pFlowSystem->UnregisterType(pNodeFactory->GetNodeTypeName());
		}
	}
}

} // Cry::GamePlatform
