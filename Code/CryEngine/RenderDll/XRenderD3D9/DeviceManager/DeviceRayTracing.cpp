#include "StdAfx.h"
#include <unordered_map>
#include "xxhash.h"
#include "DeviceObjects.h"
#include "DeviceCommandListCommon.h"
#include "Common/ReverseDepth.h"
#include "Common/Textures/TextureHelpers.h"
#include "../GraphicsPipeline/Common/GraphicsPipelineStateSet.h"

CRayTracingBottomLevelAccelerationStructurePtr CDeviceObjectFactory::CreateRayTracingBottomLevelAS(const SRayTracingBottomLevelASCreateInfo& rtBottomLevelCreateInfo)
{
	return CreateRayTracingBottomLevelASImpl(rtBottomLevelCreateInfo);
}
CRayTracingTopLevelAccelerationStructurePtr CDeviceObjectFactory::CreateRayTracingTopLevelAS(const SRayTracingTopLevelASCreateInfo& rtTopLevelCreateInfo)
{
	return CreateRayTracingTopLevelASImpl(rtTopLevelCreateInfo);
}
SRayTracingAccelerationStructSize CDeviceObjectFactory::GetRayTracingBottomLevelASSize(const SRayTracingBottomLevelASCreateInfo& rtBottomLevelCreateInfo)
{
	return GetRayTracingBottomLevelASSizeImpl(rtBottomLevelCreateInfo);
}

SRayTracingAccelerationStructSize CDeviceObjectFactory::GetRayTracingTopLevelASSize(const SRayTracingTopLevelASCreateInfo& rtTopLevelCreateInfo)
{
	return GetRayTracingTopLevelASSizeImpl(rtTopLevelCreateInfo);
}

void CDeviceGraphicsCommandInterface::BuildRayTracingBottomLevelASs(std::vector<CRayTracingBottomLevelAccelerationStructurePtr>& rtBottomLevelASPtrs)
{
	BuildRayTracingBottomLevelASsImpl(rtBottomLevelASPtrs);
}

void CDeviceGraphicsCommandInterface::BuildRayTracingTopLevelAS(CRayTracingTopLevelAccelerationStructurePtr rtTopLevelASPtr, CGpuBuffer* instanceBuffer, uint32 offset)
{
	BuildRayTracingTopLevelASImpl(rtTopLevelASPtr, instanceBuffer, offset);
}

CDeviceRayTracingPSODesc::CDeviceRayTracingPSODesc(const CDeviceRayTracingPSODesc& other)
{
	*this = other;
}

CDeviceRayTracingPSODesc::CDeviceRayTracingPSODesc(CDeviceResourceLayoutPtr pResourceLayout,::CShader* pShader, const CCryNameTSCRC& technique, uint64 rtFlags, uint32 mdFlags,uint32 maxPipelineRayRecursionDepth)
{
	memset(this, 0, sizeof(CDeviceRayTracingPSODesc));
	m_pShader = pShader;
	m_technique = technique;
	m_ShaderFlags_RT = rtFlags;
	m_ShaderFlags_MD = mdFlags;
	m_pResourceLayout = pResourceLayout;
	m_maxPipelineRayRecursionDepth = maxPipelineRayRecursionDepth;
}

CDeviceRayTracingPSODesc& CDeviceRayTracingPSODesc::operator=(const CDeviceRayTracingPSODesc& other)
{
	// increment ref counts
	m_pResourceLayout = other.m_pResourceLayout;
	m_pShader = other.m_pShader;
	memcpy(this, &other, sizeof(CDeviceRayTracingPSODesc));

	return *this;
}

bool CDeviceRayTracingPSODesc::operator==(const CDeviceRayTracingPSODesc& other) const
{
	return memcmp(this, &other, sizeof(CDeviceRayTracingPSODesc)) == 0;
}

uint64 CDeviceRayTracingPSODesc::GetHash() const
{
	uint64 key = XXH64(this, sizeof(CDeviceRayTracingPSODesc), 0);
	return key;
}

CDeviceRayTracingPSOPtr CDeviceObjectFactory::CreateRayTracingPSO(const CDeviceRayTracingPSODesc& psoDesc)
{
	CDeviceRayTracingPSOPtr pPso;

	auto it = m_RayTracingPsoCache.find(psoDesc);
	if (it != m_RayTracingPsoCache.end())
	{
		pPso = it->second;
	}
	else
	{
		pPso = CreateRayTracingPSOImpl(psoDesc);
		m_RayTracingPsoCache.emplace(psoDesc, pPso);

		if (!pPso->IsValid())
			m_InvalidRayTracingPsos.emplace(psoDesc, pPso);
	}

	pPso->SetLastUseFrame(gRenDev->GetRenderFrameID());
	return pPso;
}

void CDeviceGraphicsCommandInterface::SetRayTracingPipelineState(const CDeviceRayTracingPSO* deviceRayTracingPSO)
{
	if (m_raytracingState.pPipelineState.Set(deviceRayTracingPSO))
	{
		SetRayTracingPipelineStateImpl(deviceRayTracingPSO);
	}
}

void CDeviceGraphicsCommandInterface::SetRayTracingResourceLayout(const CDeviceResourceLayout* pResourceLayout)
{
	if (m_raytracingState.pResourceLayout.Set(pResourceLayout))
	{
		// If a root signature is changed on a command list, all previous root signature bindings
		// become stale and all newly expected bindings must be set before Draw/Dispatch
		ZeroArray(m_raytracingState.pResourceSets);

		m_raytracingState.requiredResourceBindings = pResourceLayout->GetRequiredResourceBindings();
		m_raytracingState.validResourceBindings = 0;  // invalidate all resource bindings

		m_raytracingState.custom.pendingBindings.Reset();
		//SetResourceLayoutImpl(pResourceLayout);

#if defined(ENABLE_PROFILING_CODE)
		++m_profilingStats.numLayoutSwitches;
#endif
	}
}

void CDeviceGraphicsCommandInterface::SetRayTracingResources(uint32 bindSlot, const CDeviceResourceSet* pResources)
{
	if (m_raytracingState.pResourceSets[bindSlot].Set(pResources))
	{
		m_raytracingState.validResourceBindings[bindSlot] = pResources->IsValid();

		SetRayTracingResourcesImpl(bindSlot, pResources);

#if defined(ENABLE_PROFILING_CODE)
		++m_profilingStats.numResourceSetSwitches;
#endif
	}
}

void CDeviceGraphicsCommandInterface::DispatchRayTracing(uint32 width, uint32 height)
{
	DispatchRayTracingImpl(width, height);
}




