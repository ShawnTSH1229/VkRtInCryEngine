// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Events/OnContentCreated.h>
#include <Events/OnDownloadedSharedFile.h>
#include <Events/OnFileShared.h>
#include <FlowGraph/Nodes/Listeners/ListenerBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_OnContentCreated
	: public CFlowListenerBaseNode<COnContentCreated>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_UniqueId = eOut_CustomOutput,
		eOut_RequireAccept
	};

public:

	CFlowNode_OnContentCreated(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnContentCreated() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnContentCreated(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnDownloadedSharedFile
	: public CFlowListenerBaseNode<COnDownloadedSharedFile>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_SharedFileId = eOut_CustomOutput
	};

public:

	CFlowNode_OnDownloadedSharedFile(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnDownloadedSharedFile() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnDownloadedSharedFile(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_OnFileShared
	: public CFlowListenerBaseNode<COnFileShared>
{

	using Base = CFlowListenerBaseNode<Event>;

protected:

	enum EOutputPorts
	{
		eOut_Name = eOut_CustomOutput,
		eOut_SharedFileId
	};

public:

	CFlowNode_OnFileShared(SActivationInfo* pActInfo) : Base(pActInfo) {}
	virtual ~CFlowNode_OnFileShared() override {}

	// IFlowBaseNode
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_OnFileShared(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	// ~IFlowBaseNode

protected:

	// CFlowListenerBaseNode
	virtual void               HandleEvent(SActivationInfo* pActInfo, const Event& event) override;
	// ~CFlowListenerBaseNode

};

class CFlowNode_StartStopRemoteFileListener
	: public CFlowBaseNode<eNCT_Instanced>
{

protected:

	enum EInputPorts
	{
		eIn_Start,
		eIn_Stop,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputPorts
	{
		eOut_OnStarted,
		eOut_OnStopped
	};

public:

	CFlowNode_StartStopRemoteFileListener(SActivationInfo*) {}
	~CFlowNode_StartStopRemoteFileListener() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_StartStopRemoteFileListener(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

class CFlowNode_StartStopSharedFileListener
	: public CFlowBaseNode<eNCT_Instanced>
{

protected:

	enum EInputPorts
	{
		eIn_Start,
		eIn_Stop,
		eIn_Platform,
		eIn_SharedFileId
	};

	enum EOutputPorts
	{
		eOut_OnStarted,
		eOut_OnStopped
	};

public:

	CFlowNode_StartStopSharedFileListener(SActivationInfo*) {}
	~CFlowNode_StartStopSharedFileListener() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_StartStopSharedFileListener(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };
	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;
};

} // Cry::GamePlatform