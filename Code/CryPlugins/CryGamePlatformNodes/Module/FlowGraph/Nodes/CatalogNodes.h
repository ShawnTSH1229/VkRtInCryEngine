// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_BrowseCatalog : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Browse,
		eIn_Platform,
		eIn_SortOrder
	};

public:

	CFlowNode_BrowseCatalog(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_BrowseCatalog() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetLicenses : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Next,
		eIn_Platform
	};

	enum EOutputs
	{
		eOut_Remaining,
		eOut_ApplicationId
	};

public:

	CFlowNode_GetLicenses(SActivationInfo* pActInfo) : m_licenses(), m_curIndex(0) {}
	virtual ~CFlowNode_GetLicenses() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_GetLicenses(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	std::vector<string> m_licenses;
	int                 m_curIndex;

};

class QueryCatalogItemDetails : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Query,
		eIn_Platform,
		eIn_SystemContainerId
	};

public:

	QueryCatalogItemDetails(SActivationInfo* pActInfo) {}
	virtual ~QueryCatalogItemDetails() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
