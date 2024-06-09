#pragma once
#include "RenderPassBase.h"
#include "Common/Shaders/CShader.h"

class CRayTracingRenderPass : public CRenderPassBase
{
public:

	enum EDirtyFlags : uint32
	{
		eDirty_ResourceLayout = BIT(0),
		eDirty_Resources = BIT(1),
		eDirty_Technique = BIT(2),

		eDirty_None = 0,
		eDirty_All = eDirty_Technique | eDirty_Resources | eDirty_ResourceLayout,
	};


	CRayTracingRenderPass();

	void SetTechnique(CShader* pShader, const CCryNameTSCRC& techName, uint64 rtMask);
	void PrepareResourcesForUse(CDeviceCommandListRef RESTRICT_REFERENCE commandList);
	void DispatchRayTracing(CDeviceCommandListRef RESTRICT_REFERENCE commandList,uint32 width,uint32 height);

	void SetMaxPipelineRayRecursionDepth(uint32 depth);

	void SetNeedBindless(bool needBindless);
	void SetBuffer(uint32 slot, CGpuBuffer* pBuffer);
	void SetTexture(uint32 slot, CTexture* pTexture, ResourceViewHandle resourceViewID = EDefaultResourceViews::Default);
	void SetOutputUAV(uint32 slot, CTexture* pTexture, ResourceViewHandle resourceViewID = EDefaultResourceViews::UnorderedAccess, ::EShaderStage shaderStages = EShaderStage_RayTracing);
	void SetOutputUAV(uint32 slot, CGpuBuffer* pBuffer, ResourceViewHandle resourceViewID = EDefaultResourceViews::UnorderedAccess, ::EShaderStage shaderStages = EShaderStage_RayTracing);
	void SetConstantBuffer(uint32 slot, CConstantBuffer* pConstantBuffer);

private:

	EDirtyFlags Compile();
private:
	EDirtyFlags              m_dirtyMask;
	CShader*				 m_pShader;
	CCryNameTSCRC            m_techniqueName;
	uint64                   m_rtMask;
	CDeviceResourceSetDesc   m_resourceDesc;
	CDeviceResourceSetPtr    m_pResourceSet;
	CDeviceResourceLayoutPtr m_pResourceLayout;
	CDeviceRayTracingPSOPtr  m_pPipelineState;
	int                      m_currentPsoUpdateCount;

	bool                     m_bCompiled;
	bool					 m_needBindless;

	uint32					 m_maxPipelineRayRecursionDepth;
};

DEFINE_ENUM_FLAG_OPERATORS(CRayTracingRenderPass::EDirtyFlags);

inline void CRayTracingRenderPass::SetMaxPipelineRayRecursionDepth(uint32 depth)
{
	m_maxPipelineRayRecursionDepth = depth;
}

inline void CRayTracingRenderPass::SetNeedBindless(bool needBindless)
{
	m_needBindless = needBindless;
}

inline void CRayTracingRenderPass::SetTexture(uint32 slot, CTexture* pTexture, ResourceViewHandle resourceViewID)
{
	m_resourceDesc.SetTexture(slot, pTexture, resourceViewID, EShaderStage_RayTracing);
}

inline void CRayTracingRenderPass::SetBuffer(uint32 slot, CGpuBuffer* pBuffer)
{
	m_resourceDesc.SetBuffer(slot, pBuffer, EDefaultResourceViews::Default, EShaderStage_RayTracing);
}

inline void CRayTracingRenderPass::SetOutputUAV(uint32 slot, CTexture* pTexture, ResourceViewHandle resourceViewID, ::EShaderStage shaderStages)
{
	m_resourceDesc.SetTexture(slot, pTexture, resourceViewID, shaderStages);
}

inline void CRayTracingRenderPass::SetOutputUAV(uint32 slot, CGpuBuffer* pBuffer, ResourceViewHandle resourceViewID, ::EShaderStage shaderStages)
{
	m_resourceDesc.SetBuffer(slot, pBuffer, resourceViewID, shaderStages);
}

inline void CRayTracingRenderPass::SetConstantBuffer(uint32 slot, CConstantBuffer* pConstantBuffer)
{
	m_resourceDesc.SetConstantBuffer(slot, pConstantBuffer, EShaderStage_RayTracing);
}