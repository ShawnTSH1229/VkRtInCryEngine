#include "RayTracingRenderPass.h"

CRayTracingRenderPass::CRayTracingRenderPass()
	: m_dirtyMask(eDirty_All)
	, m_bCompiled(false)
	, m_resourceDesc()
	, m_needBindless(false)
	, m_maxPipelineRayRecursionDepth(1)
{
	m_pResourceSet = GetDeviceObjectFactory().CreateResourceSet(CDeviceResourceSet::EFlags_ForceSetAllState);
}

void CRayTracingRenderPass::SetTechnique(CShader* pShader, const CCryNameTSCRC& techName, uint64 rtMask)
{
	if (m_pShader != pShader)
	{
		m_dirtyMask |= (eDirty_Technique);
	}

	if (m_techniqueName != techName)
	{
		m_dirtyMask |= (eDirty_Technique);
	}

	if (m_rtMask != rtMask)
	{
		m_dirtyMask |= (eDirty_Technique);
	}

	m_pShader = pShader;
	m_techniqueName = techName;
	m_rtMask = rtMask;

	if (m_pPipelineState && ((!m_pPipelineState->IsValid()) || m_currentPsoUpdateCount != m_pPipelineState->GetUpdateCount()))
	{
		m_dirtyMask |= eDirty_Technique;
	}
}

void CRayTracingRenderPass::PrepareResourcesForUse(CDeviceCommandListRef RESTRICT_REFERENCE commandList)
{
	Compile();

	if (m_dirtyMask == eDirty_None)
	{
		CDeviceGraphicsCommandInterface* pComputeInterface = commandList.GetGraphicsInterface();

		// Prepare resources
		int bindSlot = 0;
		pComputeInterface->PrepareResourcesForUse(bindSlot++, m_pResourceSet.get());
	}
}

CRayTracingRenderPass::EDirtyFlags CRayTracingRenderPass::Compile()
{
	EDirtyFlags dirtyMask = m_dirtyMask | (EDirtyFlags)m_resourceDesc.GetDirtyFlags();
	m_resourceDesc.m_needBindless = m_needBindless;
	if ((dirtyMask != eDirty_None) || (m_currentPsoUpdateCount != m_pPipelineState->GetUpdateCount()))
	{
		EDirtyFlags revertMask = dirtyMask;

		if (dirtyMask & (eDirty_Resources))
		{
			if (!m_pResourceSet->Update(m_resourceDesc))
				return (EDirtyFlags)(m_dirtyMask |= revertMask);
		}

		if (dirtyMask & (eDirty_Technique | eDirty_ResourceLayout))
		{
			int bindSlot = 0;
			SDeviceResourceLayoutDesc resourceLayoutDesc;
			resourceLayoutDesc.m_needBindlessLayout = m_needBindless;
			resourceLayoutDesc.SetResourceSet(bindSlot++, m_resourceDesc);
			m_pResourceLayout = GetDeviceObjectFactory().CreateResourceLayout(resourceLayoutDesc);

			if (!m_pResourceLayout)
				return (EDirtyFlags)(m_dirtyMask |= revertMask);
		}

		if (dirtyMask & (eDirty_Technique | eDirty_ResourceLayout))
		{
			CDeviceRayTracingPSODesc psoDesc(m_pResourceLayout, m_pShader, m_techniqueName, m_rtMask, 0, m_maxPipelineRayRecursionDepth);
			m_pPipelineState = GetDeviceObjectFactory().CreateRayTracingPSO(psoDesc);

			if (!m_pPipelineState || !m_pPipelineState->IsValid())
			{
				return (EDirtyFlags)(m_dirtyMask |= revertMask);
			}
		}

		m_dirtyMask = dirtyMask = eDirty_None;
	}

	return dirtyMask;
}

void CRayTracingRenderPass::DispatchRayTracing(CDeviceCommandListRef RESTRICT_REFERENCE commandList, uint32 width, uint32 height)
{
	if (m_dirtyMask == eDirty_None)
	{
		int bindSlot = 0;
		CDeviceGraphicsCommandInterface* pCommandInterface = commandList.GetGraphicsInterface();
		pCommandInterface->SetRayTracingPipelineState(m_pPipelineState.get());
		pCommandInterface->SetRayTracingResourceLayout(m_pResourceLayout.get());
		pCommandInterface->SetRayTracingResources(bindSlot++, m_pResourceSet.get());
		pCommandInterface->DispatchRayTracing(width, height);
	}
}
