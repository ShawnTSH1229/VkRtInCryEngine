// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowSystem.h>

namespace Cry::GamePlatform
{

static inline string CreateDisplayName(const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	char result[1024];
	cry_vsprintf(result, szFormat, args);
	va_end(args);
	return result;
}

// FlowNode factory class for Statistic/Achievement nodes
class CStatisticFlowNodeFactory : public IFlowNodeFactory
{

public:

	CStatisticFlowNodeFactory(const string& className, IFlowNodePtr pFlowNode)
		: m_className(className)
		, m_pFlowNode(pFlowNode)
	{
	}

	IFlowNodePtr Create(IFlowNode::SActivationInfo* pActInfo)
	{
		return m_pFlowNode->Clone(pActInfo);
	}

	void GetMemoryUsage(ICrySizer* s) const
	{
		SIZER_SUBCOMPONENT_NAME(s, "CStatisticFlowNodeFactory");
	}

	void Reset()
	{}

	const char* GetNodeTypeName() const
	{
		return m_className.c_str();
	}

private:

	const string m_className;
	IFlowNodePtr m_pFlowNode;

};

TYPEDEF_AUTOPTR(CStatisticFlowNodeFactory);
typedef CStatisticFlowNodeFactory_AutoPtr CStatisticFlowNodeFactoryPtr;

} // Cry::GamePlatform
