// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <queue>
#include <CryFlowGraph/IFlowBaseNode.h>

namespace Cry::GamePlatform
{

class CFlowNode_DeleteRemoteFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Delete,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_DeleteRemoteFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_DeleteRemoteFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_DownloadSharedFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Download,
		eIn_Platform,
		eIn_SharedFileId,
		eIn_DownloadPriority
	};

	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_DownloadSharedFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_DownloadSharedFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_FileExists : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_Exists
	};

public:

	CFlowNode_FileExists(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_FileExists() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetCacheList : public CFlowBaseNode<eNCT_Instanced>
{

	enum EInputs
	{
		eIn_Get,
		eIn_GetNext,
		eIn_Platform,
		eIn_CacheType
	};

	enum EOutputs
	{
		eOut_Remaining,
		eOut_NameOrId
	};

public:

	CFlowNode_GetCacheList(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetCacheList() override {}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override { return new CFlowNode_GetCacheList(pActInfo); };
	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

private:

	std::queue<string> m_cacheList;

};

class CFlowNode_GetFileSize : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_FileSize
	};

public:

	CFlowNode_GetFileSize(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetFileSize() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetSharedFileId : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_SharedId
	};

public:

	CFlowNode_GetSharedFileId(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetSharedFileId() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_GetSharedFileSize : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Get,
		eIn_Platform,
		eIn_SharedId
	};

	enum EOutputs
	{
		eOut_Success,
		eOut_FileSize
	};

public:

	CFlowNode_GetSharedFileSize(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_GetSharedFileSize() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_IsEnabled : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform
	};
	enum EOutputs
	{
		eOut_IsEnabled
	};

public:

	CFlowNode_IsEnabled(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_IsEnabled() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ReadData : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Read,
		eIn_Platform,
		eIn_Name
	};
	enum EOutputs
	{
		eOut_Success,
		eOut_Data
	};

public:

	CFlowNode_ReadData(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReadData() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ReadDataToFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Read,
		eIn_Platform,
		eIn_Name,
		eIn_FilePath
	};
	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_ReadDataToFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReadDataToFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};


class CFlowNode_ReadSharedData : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Read,
		eIn_Platform,
		eIn_SharedId
	};
	enum EOutputs
	{
		eOut_Success,
		eOut_Data
	};

public:

	CFlowNode_ReadSharedData(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReadSharedData() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ReadSharedDataToFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Read,
		eIn_Platform,
		eIn_SharedId,
		eIn_FilePath
	};
	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_ReadSharedDataToFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReadSharedDataToFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ReleaseRemoteFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Release,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputs
	{
		eOut_OnReleased
	};

public:

	CFlowNode_ReleaseRemoteFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReleaseRemoteFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ReleaseSharedFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Release,
		eIn_Platform,
		eIn_SharedId
	};

	enum EOutputs
	{
		eOut_OnReleased
	};

public:

	CFlowNode_ReleaseSharedFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ReleaseSharedFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_SharedFileAvailable : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Check,
		eIn_Platform,
		eIn_SharedId
	};

	enum EOutputs
	{
		eOut_Available
	};

public:

	CFlowNode_SharedFileAvailable(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_SharedFileAvailable() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_ShareRemoteFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Share,
		eIn_Platform,
		eIn_Name
	};

	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_ShareRemoteFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_ShareRemoteFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_WriteDataFromFile : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Write,
		eIn_Platform,
		eIn_Name,
		eIn_FilePath
	};
	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_WriteDataFromFile(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_WriteDataFromFile() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

class CFlowNode_WriteData : public CFlowBaseNode<eNCT_Singleton>
{

	enum EInputs
	{
		eIn_Write,
		eIn_Platform,
		eIn_Name,
		eIn_Data
	};

	enum EOutputs
	{
		eOut_Success
	};

public:

	CFlowNode_WriteData(SActivationInfo* pActInfo) {}
	virtual ~CFlowNode_WriteData() override {}

	virtual void         GetMemoryUsage(ICrySizer* s) const override { s->Add(*this); };

	virtual void         GetConfiguration(SFlowNodeConfig& nodeConfig) override;
	virtual void         ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override;

};

} // Cry::GamePlatform
