// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_CreateContent : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Create,
		eIn_Platform,
		eIn_ContentType,
		eIn_Title,
		eIn_Description,
		eIn_Visibility,
		eIn_Tags,
		eIn_FolderPath,
		eIn_PreviewPath
	};

public:

	CFlowNode_CreateContent(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_CreateContent() override {}

	virtual void GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:
	DynArray<string> ParseTagList(const string& tags) const;

};

} // Cry::GamePlatform
