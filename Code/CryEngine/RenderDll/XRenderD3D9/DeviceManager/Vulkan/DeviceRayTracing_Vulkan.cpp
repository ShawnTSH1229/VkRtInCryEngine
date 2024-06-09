#include "..\DeviceObjects.h"

#include "Vulkan/API/VKInstance.hpp"
#include "Vulkan/API/VKBufferResource.hpp"
#include "Vulkan/API/VKImageResource.hpp"
#include "Vulkan/API/VKSampler.hpp"
#include "Vulkan/API/VKExtensions.hpp"

#include <Common/Renderer.h>
#include <CryCore/Assert/CryAssert.h>

#include "DeviceRayTracing_Vulkan.h"
#include "DeviceResourceSet_Vulkan.h"	
#include "DevicePSO_Vulkan.h"	
#include "DeviceBindless_Vulkan.h"

VkDeviceAddress InputStreamGetBufferDeviceAddress(const VkDevice* pVkDevice, const CDeviceInputStream* deviceInputStreaming)
{
	buffer_size_t offset;
	CBufferResource* const pActualBuffer = gcpRendD3D.m_DevBufMan.GetD3D(((SStreamInfo*)deviceInputStreaming)->hStream, &offset);
	VkBufferDeviceAddressInfo bufferDeviceAddressInfo = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
	bufferDeviceAddressInfo.buffer = pActualBuffer->GetHandle();
	return vkGetBufferDeviceAddress(*pVkDevice, &bufferDeviceAddressInfo) + offset;
}

static VkFormat GetVkVertexFormat(InputLayoutHandle vertexFormat)
{
	switch (vertexFormat)
	{
	case EDefaultInputLayouts::P3F:
		return VK_FORMAT_R32G32B32_SFLOAT;
	}
	CRY_ASSERT(false);
	return VK_FORMAT_UNDEFINED;
}

static void AddAccelerationStructureBuildBarrier(VkCommandBuffer CommandBuffer)
{
	VkMemoryBarrier Barrier = { VK_STRUCTURE_TYPE_MEMORY_BARRIER };
	Barrier.srcAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
	Barrier.dstAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
	
	// TODO: Revisit the compute stages here as we don't always need barrier to compute
	VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
	VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;

	vkCmdPipelineBarrier(CommandBuffer, srcStage, dstStage, 0, 1, &Barrier, 0, nullptr, 0, nullptr);
}

// Get build geometry info / build range info / build size info
static void GetBottomLevelAccelerationStructureBuildInfo(
	const VkDevice* pVkDevice,
	const std::vector<SRayTracingGeometryTriangle>& rtGeometryTriangles,
	const SRayTracingBottomLevelASCreateInfo::STriangleIndexInfo& sSTriangleIndexInfo,
	EBuildAccelerationStructureFlag eBuildFlag,
	EBuildAccelerationStructureMode eBuildMode,
	SVulkanRayTracingBLASBuildInfo& outvkRtBLASBuildInfo)
{
	VkDeviceAddress indexDeviceAddress = sSTriangleIndexInfo.m_sIndexStreaming ? InputStreamGetBufferDeviceAddress(pVkDevice, sSTriangleIndexInfo.m_sIndexStreaming) : 0;

	std::vector<uint32> maxPrimitiveCounts;
	for (uint32 index = 0; index < rtGeometryTriangles.size(); index++)
	{
		const SRayTracingGeometryTriangle& rayTracingGeometryTriangle = rtGeometryTriangles[index];
		VkDeviceAddress vertexDeviceAddress = InputStreamGetBufferDeviceAddress(pVkDevice, rayTracingGeometryTriangle.m_sTriangVertexleInfo.m_sVertexStreaming);

		VkAccelerationStructureGeometryKHR accelerationStructureGeometry = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR };

		if (rayTracingGeometryTriangle.bForceOpaque)
		{
			accelerationStructureGeometry.flags |= VK_GEOMETRY_OPAQUE_BIT_KHR;
		}

		accelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;

		accelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		accelerationStructureGeometry.geometry.triangles.vertexFormat = GetVkVertexFormat(rayTracingGeometryTriangle.m_sTriangVertexleInfo.m_hVertexPositionFormat);
		accelerationStructureGeometry.geometry.triangles.vertexData.deviceAddress = vertexDeviceAddress;
		accelerationStructureGeometry.geometry.triangles.vertexStride = ((SStreamInfo*)rayTracingGeometryTriangle.m_sTriangVertexleInfo.m_sVertexStreaming)->nStride;
		accelerationStructureGeometry.geometry.triangles.indexData.deviceAddress = indexDeviceAddress;

		if (sSTriangleIndexInfo.m_sIndexStreaming)
		{
			accelerationStructureGeometry.geometry.triangles.indexType = ((SStreamInfo*)sSTriangleIndexInfo.m_sIndexStreaming)->nStride == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
		}
		else
		{
			CRY_ASSERT(false);
		}

		outvkRtBLASBuildInfo.m_vkRtGeometryTrianglesInfo.push_back(accelerationStructureGeometry);

		VkAccelerationStructureBuildRangeInfoKHR RangeInfo = {};
		RangeInfo.primitiveCount = rayTracingGeometryTriangle.bEnable ? rayTracingGeometryTriangle.m_sRangeInfo.m_nPrimitiveCount : 0;
		RangeInfo.primitiveOffset = rayTracingGeometryTriangle.m_sRangeInfo.m_nPrimitiveOffset;
		RangeInfo.firstVertex = rayTracingGeometryTriangle.m_sRangeInfo.m_nFirstVertex;
		outvkRtBLASBuildInfo.m_vkAsBuildRangeInfo.push_back(RangeInfo);

		maxPrimitiveCounts.push_back(rayTracingGeometryTriangle.m_sTriangVertexleInfo.m_nMaxVertex);
	}

	outvkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	outvkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.flags = uint32(eBuildFlag & EBuildAccelerationStructureFlag::eBuild_PreferBuild) != (0) ? VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR : VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	outvkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.mode = uint32(eBuildMode & EBuildAccelerationStructureMode::eBuild) != 0 ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
	outvkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.geometryCount = outvkRtBLASBuildInfo.m_vkRtGeometryTrianglesInfo.size();
	outvkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.pGeometries = outvkRtBLASBuildInfo.m_vkRtGeometryTrianglesInfo.data();

	Extensions::KHR_acceleration_structure::vkGetAccelerationStructureBuildSizesKHR(*pVkDevice, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &outvkRtBLASBuildInfo.m_vkAsBuildGeometryInfo, maxPrimitiveCounts.data(), &outvkRtBLASBuildInfo.m_vkAsBuildSizeInfo);
}

SRayTracingAccelerationStructSize CDeviceObjectFactory::GetRayTracingBottomLevelASSizeImpl(const SRayTracingBottomLevelASCreateInfo& rtBottomLevelCreateInfo)
{
	SVulkanRayTracingBLASBuildInfo vkRtBLASBuildInfo;
	GetBottomLevelAccelerationStructureBuildInfo(
		&GetDevice()->GetVkDevice(),
		rtBottomLevelCreateInfo.m_rtGeometryTriangles,
		rtBottomLevelCreateInfo.m_sSTriangleIndexInfo,
		rtBottomLevelCreateInfo.m_eBuildFlag, EBuildAccelerationStructureMode::eBuild, vkRtBLASBuildInfo);

	uint32 nAccelerationStructureAlignment = 256;
	uint32 nScratchAlignment = std::max<uint32>(nAccelerationStructureAlignment, GetDevice()->GetVulkanDeviceExtensionProperties().m_vkAccelerationStructureProperties.minAccelerationStructureScratchOffsetAlignment);

	SRayTracingAccelerationStructSize rtAccelerationStructSize;
	rtAccelerationStructSize.m_nAccelerationStructureSize = alignedValue(vkRtBLASBuildInfo.m_vkAsBuildSizeInfo.accelerationStructureSize, nAccelerationStructureAlignment);
	rtAccelerationStructSize.m_nBuildScratchSize = alignedValue(vkRtBLASBuildInfo.m_vkAsBuildSizeInfo.buildScratchSize, nScratchAlignment);
	rtAccelerationStructSize.m_nUpdateScratchSize = alignedValue(vkRtBLASBuildInfo.m_vkAsBuildSizeInfo.updateScratchSize, nScratchAlignment);
	return rtAccelerationStructSize;
}


CVulkanRayTracingBottomLevelAccelerationStructure::CVulkanRayTracingBottomLevelAccelerationStructure(const SRayTracingBottomLevelASCreateInfo& rtBottomLevelCreateInfo, CDevice* pDevice)
	:CRayTracingBottomLevelAccelerationStructure(rtBottomLevelCreateInfo)
	, m_pDevice(pDevice)
{
	m_sSizeInfo = GetDeviceObjectFactory().GetRayTracingBottomLevelASSize(rtBottomLevelCreateInfo);

	m_accelerationStructureBuffer.Create(1u, static_cast<uint32>(m_sSizeInfo.m_nAccelerationStructureSize), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_ACCELERATION_STRUCTURE, nullptr);

	VkAccelerationStructureCreateInfoKHR accelerationStructureCreateInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR };
	accelerationStructureCreateInfo.buffer = m_accelerationStructureBuffer.GetDevBuffer()->GetBuffer()->GetHandle();
	accelerationStructureCreateInfo.offset = 0;
	accelerationStructureCreateInfo.size = m_sSizeInfo.m_nAccelerationStructureSize;
	accelerationStructureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;

	Extensions::KHR_acceleration_structure::vkCreateAccelerationStructureKHR(m_pDevice->GetVkDevice(), &accelerationStructureCreateInfo, nullptr, &accelerationStructureHandle);

	VkAccelerationStructureDeviceAddressInfoKHR deviceAddressInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR };
	deviceAddressInfo.accelerationStructure = accelerationStructureHandle;
	accelerationStructureDeviceAddress = Extensions::KHR_acceleration_structure::vkGetAccelerationStructureDeviceAddressKHR(m_pDevice->GetVkDevice(), &deviceAddressInfo);
}

uint64 CVulkanRayTracingBottomLevelAccelerationStructure::GetAccelerationStructureAddress()
{
	return accelerationStructureDeviceAddress;
}

CRayTracingBottomLevelAccelerationStructurePtr CDeviceObjectFactory::CreateRayTracingBottomLevelASImpl(const SRayTracingBottomLevelASCreateInfo& rtBottomLevelCreateInfo)
{
	return std::make_shared<CVulkanRayTracingBottomLevelAccelerationStructure>(rtBottomLevelCreateInfo, GetDevice());
}

void CDeviceGraphicsCommandInterfaceImpl::BuildRayTracingBottomLevelASsImpl(std::vector<CRayTracingBottomLevelAccelerationStructurePtr>& rtBottomLevelASPtrs)
{
	uint64 nTotalScratchBufferSize = 0;
	for (auto& blas : rtBottomLevelASPtrs)
	{
		nTotalScratchBufferSize += blas->m_sSizeInfo.m_nBuildScratchSize;
	}

	CGpuBuffer scratchBuffer;
	scratchBuffer.Create(1u, static_cast<uint32>(nTotalScratchBufferSize), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, nullptr);

	std::vector<SVulkanRayTracingBLASBuildInfo> tempBuildInfos;
	std::vector<VkAccelerationStructureBuildGeometryInfoKHR> buildGeometryInfos;
	std::vector<VkAccelerationStructureBuildRangeInfoKHR*> buildRangeInfos;

	uint64 nScratchBufferOffset = 0;
	for (auto& blas : rtBottomLevelASPtrs)
	{
		tempBuildInfos.push_back(SVulkanRayTracingBLASBuildInfo());
		SVulkanRayTracingBLASBuildInfo& vkRtBLASBuildInfo = tempBuildInfos[tempBuildInfos.size() - 1];

		GetBottomLevelAccelerationStructureBuildInfo(
			&GetDevice()->GetVkDevice(),
			blas->m_sRtBlASCreateInfo.m_rtGeometryTriangles,
			blas->m_sRtBlASCreateInfo.m_sSTriangleIndexInfo,
			blas->m_sRtBlASCreateInfo.m_eBuildFlag, EBuildAccelerationStructureMode::eBuild, vkRtBLASBuildInfo);

		VkBufferDeviceAddressInfo bufferDeviceAddressInfo = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
		bufferDeviceAddressInfo.buffer = scratchBuffer.GetDevBuffer()->GetBuffer()->GetHandle();
		VkDeviceAddress scratchBufferAddress = vkGetBufferDeviceAddress(GetDevice()->GetVkDevice(), &bufferDeviceAddressInfo) + nScratchBufferOffset;
		nScratchBufferOffset += blas->m_sSizeInfo.m_nBuildScratchSize;

		CVulkanRayTracingBottomLevelAccelerationStructure* vkRtBLAS = static_cast<CVulkanRayTracingBottomLevelAccelerationStructure*>(blas.get());
		vkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.dstAccelerationStructure = vkRtBLAS->accelerationStructureHandle;
		vkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.srcAccelerationStructure = nullptr;
		vkRtBLASBuildInfo.m_vkAsBuildGeometryInfo.scratchData.deviceAddress = scratchBufferAddress;

		buildGeometryInfos.push_back(vkRtBLASBuildInfo.m_vkAsBuildGeometryInfo);
		buildRangeInfos.push_back(vkRtBLASBuildInfo.m_vkAsBuildRangeInfo.data());
	}

	VkCommandBuffer cmdBuffer = GetVKCommandList()->GetVkCommandList();
	Extensions::KHR_acceleration_structure::vkCmdBuildAccelerationStructuresKHR(cmdBuffer, rtBottomLevelASPtrs.size(), buildGeometryInfos.data(), buildRangeInfos.data());

	AddAccelerationStructureBuildBarrier(cmdBuffer);
	//GetVKCommandList()->Submit();

	//todo@ ->reset and ->begin submit at end of pass
	//GetVKCommandList()->Reset();
	//GetVKCommandList()->Begin();

	//todo@ bindless
}



static void GeTopLevelAccelerationStructureBuildInfo(const VkDevice vkDevice, const uint32 nInstance, const VkDeviceAddress instanceBufferAddress, EBuildAccelerationStructureFlag flag, SVulkanRayTracingTLASBuildInfo& outvkRtTLASBuildInfo)
{
	outvkRtTLASBuildInfo.m_vkRtGeometryInstancesInfo.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	outvkRtTLASBuildInfo.m_vkRtGeometryInstancesInfo.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	outvkRtTLASBuildInfo.m_vkRtGeometryInstancesInfo.geometry.instances.arrayOfPointers = VK_FALSE;
	outvkRtTLASBuildInfo.m_vkRtGeometryInstancesInfo.geometry.instances.data.deviceAddress = instanceBufferAddress;

	outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
	outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.flags = (flag == EBuildAccelerationStructureFlag::eBuild_PreferTrace) ? VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR : VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
	outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.geometryCount = 1;
	outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.pGeometries = &outvkRtTLASBuildInfo.m_vkRtGeometryInstancesInfo;

	Extensions::KHR_acceleration_structure::vkGetAccelerationStructureBuildSizesKHR(vkDevice, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo, &nInstance, &outvkRtTLASBuildInfo.m_vkAsBuildSizeInfo);
}

SRayTracingAccelerationStructSize CDeviceObjectFactory::GetRayTracingTopLevelASSizeImpl(const SRayTracingTopLevelASCreateInfo& rtTopLevelCreateInfo)
{
	SVulkanRayTracingTLASBuildInfo outvkRtTLASBuildInfo;
	GeTopLevelAccelerationStructureBuildInfo(GetDevice()->GetVkDevice(), rtTopLevelCreateInfo.m_nInstanceTransform, 0, EBuildAccelerationStructureFlag::eBuild_PreferTrace, outvkRtTLASBuildInfo);

	SRayTracingAccelerationStructSize accelerationStructureSizeInfo;
	accelerationStructureSizeInfo.m_nAccelerationStructureSize = outvkRtTLASBuildInfo.m_vkAsBuildSizeInfo.accelerationStructureSize;
	accelerationStructureSizeInfo.m_nBuildScratchSize = outvkRtTLASBuildInfo.m_vkAsBuildSizeInfo.buildScratchSize;
	accelerationStructureSizeInfo.m_nUpdateScratchSize = outvkRtTLASBuildInfo.m_vkAsBuildSizeInfo.updateScratchSize;
	return accelerationStructureSizeInfo;
}


CVulkanRayTracingTopLevelAccelerationStructure::CVulkanRayTracingTopLevelAccelerationStructure(const SRayTracingTopLevelASCreateInfo& rtTopLevelCreateInfo, CDevice* pDevice)
	:CRayTracingTopLevelAccelerationStructure(rtTopLevelCreateInfo)
	, m_pDevice(pDevice)
{
	m_sSizeInfo = GetDeviceObjectFactory().GetRayTracingTopLevelASSize(rtTopLevelCreateInfo);
	m_accelerationStructureBuffer.Create(1u, static_cast<uint32>(m_sSizeInfo.m_nAccelerationStructureSize), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_ACCELERATION_STRUCTURE | CDeviceObjectFactory::USAGE_STRUCTURED, nullptr);

	VkAccelerationStructureCreateInfoKHR accelerationStructureCreateInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR };
	accelerationStructureCreateInfo.buffer = m_accelerationStructureBuffer.GetDevBuffer()->GetBuffer()->GetHandle();
	accelerationStructureCreateInfo.offset = 0;
	accelerationStructureCreateInfo.size = m_sSizeInfo.m_nAccelerationStructureSize;
	accelerationStructureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;

	Extensions::KHR_acceleration_structure::vkCreateAccelerationStructureKHR(m_pDevice->GetVkDevice(), &accelerationStructureCreateInfo, nullptr, &accelerationStructureHandle);

	//todo@ bindless accelerationStructureDeviceAddress
	
	VkAccelerationStructureDeviceAddressInfoKHR deviceAddressInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR };
	deviceAddressInfo.accelerationStructure = accelerationStructureHandle;
	accelerationStructureDeviceAddress = Extensions::KHR_acceleration_structure::vkGetAccelerationStructureDeviceAddressKHR(m_pDevice->GetVkDevice(), &deviceAddressInfo);

	m_accelerationStructureBuffer.GetDevBuffer()->GetBuffer()->TempSetTLASView(*(uint64*)(&accelerationStructureHandle));
}

CRayTracingTopLevelAccelerationStructurePtr CDeviceObjectFactory::CreateRayTracingTopLevelASImpl(const SRayTracingTopLevelASCreateInfo& rtTopLevelCreateInfo)
{
	return std::make_shared<CVulkanRayTracingTopLevelAccelerationStructure>(rtTopLevelCreateInfo, GetDevice());
}

void CDeviceGraphicsCommandInterfaceImpl::BuildRayTracingTopLevelASImpl(CRayTracingTopLevelAccelerationStructurePtr rtTopLevelASPtr, CGpuBuffer* instanceBuffer, uint32 offset)
{
	CGpuBuffer scratchBuffer;
	scratchBuffer.Create(1u, static_cast<uint32>(rtTopLevelASPtr->m_sSizeInfo.m_nBuildScratchSize), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, nullptr);

	CVulkanRayTracingTopLevelAccelerationStructure* rtTopLevelAccelerationStructure = static_cast<CVulkanRayTracingTopLevelAccelerationStructure*>(rtTopLevelASPtr.get());

	SVulkanRayTracingTLASBuildInfo outvkRtTLASBuildInfo;
	{
		VkBufferDeviceAddressInfo bufferDeviceAddressInfo = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
		bufferDeviceAddressInfo.buffer = instanceBuffer->GetDevBuffer()->GetBuffer()->GetHandle();
		VkDeviceAddress instanceBufferAddress = vkGetBufferDeviceAddress(GetDevice()->GetVkDevice(), &bufferDeviceAddressInfo) + offset;
		GeTopLevelAccelerationStructureBuildInfo(GetDevice()->GetVkDevice(), rtTopLevelASPtr->m_sRtTLASCreateInfo.m_nInstanceTransform, instanceBufferAddress, EBuildAccelerationStructureFlag::eBuild_PreferTrace, outvkRtTLASBuildInfo);
		outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.dstAccelerationStructure = rtTopLevelAccelerationStructure->accelerationStructureHandle;

		bufferDeviceAddressInfo.buffer = scratchBuffer.GetDevBuffer()->GetBuffer()->GetHandle();
		outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo.scratchData.deviceAddress = vkGetBufferDeviceAddress(GetDevice()->GetVkDevice(), &bufferDeviceAddressInfo);
	}

	VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo = outvkRtTLASBuildInfo.m_vkAsBuildGeometryInfo;
	VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo;
	buildRangeInfo.primitiveCount = rtTopLevelASPtr->m_sRtTLASCreateInfo.m_nInstanceTransform;
	buildRangeInfo.primitiveOffset = 0;
	buildRangeInfo.transformOffset = 0;
	buildRangeInfo.firstVertex = 0;

	VkAccelerationStructureBuildRangeInfoKHR* pBuildRangeInfo = &buildRangeInfo;

	VkCommandBuffer cmdBuffer = GetVKCommandList()->GetVkCommandList();
	AddAccelerationStructureBuildBarrier(cmdBuffer);
	Extensions::KHR_acceleration_structure::vkCmdBuildAccelerationStructuresKHR(cmdBuffer, 1, &buildGeometryInfo, &pBuildRangeInfo);
	AddAccelerationStructureBuildBarrier(cmdBuffer);
	//GetVKCommandList()->Submit();
}


CDeviceRayTracingPSOPtr CDeviceObjectFactory::CreateRayTracingPSOImpl(const CDeviceRayTracingPSODesc& psoDesc) const
{
	CDeviceRayTracingPSO_VulkanPtr pDeiveRayTracingPSO = std::make_shared<CDeviceRayTracingPSO_Vulkan>(GetDevice());
	pDeiveRayTracingPSO->Init(psoDesc);
	return pDeiveRayTracingPSO;
}

CDeviceRayTracingPSO_Vulkan::~CDeviceRayTracingPSO_Vulkan()
{
	m_pDevice->DeferDestruction(m_pipeline);
}

EShaderStage SDeviceObjectHelpers::GetRayTracingShaderInstanceInfo(THwRTShaderInfo& result, ::CShader* pShader, const CCryNameTSCRC& technique, uint64 rtFlags, uint32 mdFlags, const UPipelineState pipelineState[eHWSC_RayEnd - eHWSC_RayStart], EVertexModifier mdvFlags)
{
	if (SShaderTechnique* pShaderTechnique = pShader->mfFindTechnique(technique))
	{
		if (pShaderTechnique->m_Passes.empty())
			return EShaderStage_None;

		SShaderPass& shaderPass = pShaderTechnique->m_Passes[0];

		typedef std::array<std::vector<CHWShader*>*, eHWSC_RayEnd - eHWSC_RayStart> TRTShaderGroups;
		TRTShaderGroups rayTracingShaderGroups;
		rayTracingShaderGroups[0] = &shaderPass.m_RGShaders;
		rayTracingShaderGroups[1] = &shaderPass.m_HGShaders;
		rayTracingShaderGroups[2] = &shaderPass.m_RMShaders;

		for (uint32 index = 0; index < (eHWSC_RayEnd - eHWSC_RayStart); index++)
		{
			if (result[index] == nullptr)
			{
				CRY_ASSERT(0);
			}
			else
			{
				result[index]->resize(rayTracingShaderGroups[index]->size());
			}
		}

		EShaderStage validShaderStages = EShaderStage_None;
		for (uint32 shaderGroupIndex = 0; shaderGroupIndex < (eHWSC_RayEnd - eHWSC_RayStart); shaderGroupIndex++)
		{
			for (uint32 shaderIndex = 0; shaderIndex < rayTracingShaderGroups[shaderGroupIndex]->size(); shaderIndex++)
			{
				CHWShader_D3D* pHWShaderD3D = reinterpret_cast<CHWShader_D3D*>((*(rayTracingShaderGroups[shaderGroupIndex]))[shaderIndex]);
				(*result[shaderGroupIndex])[shaderIndex].pHwShader = pHWShaderD3D;
				(*result[shaderGroupIndex])[shaderIndex].technique = technique;

				if (pHWShaderD3D)
				{
					SShaderCombIdent Ident;
					Ident.m_LightMask = 0;
					Ident.m_RTMask = (rtFlags & pHWShaderD3D->m_nMaskAnd_RT) | pHWShaderD3D->m_nMaskOr_RT;
					Ident.m_MDMask = mdFlags & 0xFFFFFFFF;
					Ident.m_MDVMask = (mdvFlags | CParserBin::m_nPlatform);
					Ident.m_GLMask = pHWShaderD3D->m_nMaskGenShader;
					Ident.m_pipelineState = pipelineState[shaderGroupIndex];//@todo fixme

					//Ident.m_MDMask = mdFlags & (shaderStage != eHWSC_Pixel ? 0xFFFFFFFF : ~HWMD_TEXCOORD_FLAG_MASK);
					//Ident.m_MDVMask = ((shaderStage != eHWSC_Pixel) ? mdvFlags : 0) | CParserBin::m_nPlatform;
					//Ident.m_pipelineState = pipelineState ? pipelineState[shaderStage] : UPipelineState();

					bool isShaderValid = false;
					if (auto pInstance = pHWShaderD3D->mfGetInstance(Ident, 0))
					{
						if (pHWShaderD3D->CheckActivation(pShader, pInstance, 0))
						{
							if (pInstance->m_Handle.m_pShader->m_bDisabled)
							{
								(*result[shaderGroupIndex])[shaderIndex].pHwShader = nullptr;
							}
							else
							{
								(*result[shaderGroupIndex])[shaderIndex].pHwShaderInstance = pInstance;
								(*result[shaderGroupIndex])[shaderIndex].pDeviceShader = pInstance->m_Handle.m_pShader->GetHandle();

								validShaderStages |= EShaderStage_RayGen;
							}

							isShaderValid = true;
						}
					}

					if (!isShaderValid)
					{
						return EShaderStage_None;
					}
				}
			}
		}
		return validShaderStages;
	}

	return EShaderStage_None;
}



bool CDeviceRayTracingPSO_Vulkan::Init(const CDeviceRayTracingPSODesc& psoDesc)
{
	m_isValid = false;
	m_updateCount++;

	if (psoDesc.m_pShader == nullptr)
		return false;

	std::vector<SDeviceObjectHelpers::SShaderInstanceInfo> rayGenShaderInfos;
	std::vector<SDeviceObjectHelpers::SShaderInstanceInfo> rayHitGroupShaderInfos;
	std::vector<SDeviceObjectHelpers::SShaderInstanceInfo> rayMissShaderInfos;

	SDeviceObjectHelpers::THwRTShaderInfo rayTracingShaderInfo;
	rayTracingShaderInfo[0] = &rayGenShaderInfos;
	rayTracingShaderInfo[1] = &rayHitGroupShaderInfos;
	rayTracingShaderInfo[2] = &rayMissShaderInfos;

	uint64 resourceLayoutHash = reinterpret_cast<CDeviceResourceLayout_Vulkan*>(psoDesc.m_pResourceLayout.get())->GetHash();
	UPipelineState customPipelineState[] = { resourceLayoutHash, resourceLayoutHash, resourceLayoutHash};

	EShaderStage validShaderStage = SDeviceObjectHelpers::GetRayTracingShaderInstanceInfo(rayTracingShaderInfo, psoDesc.m_pShader, psoDesc.m_technique, psoDesc.m_ShaderFlags_RT, psoDesc.m_ShaderFlags_MD, customPipelineState,MDV_NONE);
	CRY_ASSERT(validShaderStage != EShaderStage_None);

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	std::vector<VkRayTracingShaderGroupCreateInfoKHR> shaderGroups;
	std::vector<string> entryPointName;

	for (auto& rayGenInfo : rayGenShaderInfos)
	{
		entryPointName.push_back(rayGenInfo.pHwShader->m_EntryFunc);
		
		VkPipelineShaderStageCreateInfo shaderStage = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		shaderStage.module = reinterpret_cast<NCryVulkan::CShader*>(rayGenInfo.pDeviceShader)->GetVulkanShader();
		shaderStage.stage = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		shaderStage.pName = entryPointName.back().data();
		shaderStages.push_back(shaderStage);

		VkRayTracingShaderGroupCreateInfoKHR shaderGroup = { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR };
		shaderGroup.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
		shaderGroup.generalShader = shaderStages.size() - 1;//store general shader index
		shaderGroup.closestHitShader = VK_SHADER_UNUSED_KHR;
		shaderGroup.anyHitShader = VK_SHADER_UNUSED_KHR;
		shaderGroup.intersectionShader = VK_SHADER_UNUSED_KHR;
		shaderGroups.push_back(shaderGroup);
	}

	for (auto& rayMissnInfo : rayMissShaderInfos)
	{
		entryPointName.push_back(rayMissnInfo.pHwShader->m_EntryFunc);

		VkPipelineShaderStageCreateInfo shaderStage = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		shaderStage.module = reinterpret_cast<NCryVulkan::CShader*>(rayMissnInfo.pDeviceShader)->GetVulkanShader();
		shaderStage.stage = VK_SHADER_STAGE_MISS_BIT_KHR;
		shaderStage.pName = entryPointName.back().data();
		shaderStages.push_back(shaderStage);

		VkRayTracingShaderGroupCreateInfoKHR shaderGroup = { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR };
		shaderGroup.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
		shaderGroup.generalShader = shaderStages.size() - 1;//store general shader index
		shaderGroup.closestHitShader = VK_SHADER_UNUSED_KHR;
		shaderGroup.anyHitShader = VK_SHADER_UNUSED_KHR;
		shaderGroup.intersectionShader = VK_SHADER_UNUSED_KHR;
		shaderGroups.push_back(shaderGroup);
	}


	for (auto& hitGroupInfo : rayHitGroupShaderInfos)
	{
		entryPointName.push_back(hitGroupInfo.pHwShader->m_EntryFunc);

		VkPipelineShaderStageCreateInfo shaderStage = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		shaderStage.module = reinterpret_cast<NCryVulkan::CShader*>(hitGroupInfo.pDeviceShader)->GetVulkanShader();
		shaderStage.stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		shaderStage.pName = entryPointName.back().data();
		shaderStages.push_back(shaderStage);

		VkRayTracingShaderGroupCreateInfoKHR shaderGroup = { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR };
		shaderGroup.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
		shaderGroup.generalShader = VK_SHADER_UNUSED_KHR;//store general shader index
		shaderGroup.closestHitShader = shaderStages.size() - 1;
		shaderGroup.anyHitShader = VK_SHADER_UNUSED_KHR;
		shaderGroup.intersectionShader = VK_SHADER_UNUSED_KHR;
		shaderGroups.push_back(shaderGroup);
	}

	VkRayTracingPipelineCreateInfoKHR rayTracingPipelineCreateInfo = { VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR };
	rayTracingPipelineCreateInfo.stageCount = shaderStages.size();
	rayTracingPipelineCreateInfo.pStages = shaderStages.data();
	rayTracingPipelineCreateInfo.groupCount = shaderGroups.size();
	rayTracingPipelineCreateInfo.pGroups = shaderGroups.data();
	rayTracingPipelineCreateInfo.maxPipelineRayRecursionDepth = psoDesc.m_maxPipelineRayRecursionDepth;//todo:fixme
	rayTracingPipelineCreateInfo.layout = static_cast<CDeviceResourceLayout_Vulkan*>(psoDesc.m_pResourceLayout.get())->GetVkPipelineLayout();
	//rayTracingPipelineCreateInfo.flags = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
	CRY_VERIFY(Extensions::KHR_ray_tracing_pipeline::vkCreateRayTracingPipelinesKHR(GetDevice()->GetVkDevice() , {}, {}, 1, &rayTracingPipelineCreateInfo, nullptr, &m_pipeline) == VK_SUCCESS);
	
	{
		//get handles
		uint32 nRayGenCount = rayGenShaderInfos.size();
		uint32 nRayMissCount = rayMissShaderInfos.size();
		uint32 nHitGroupCount = rayHitGroupShaderInfos.size();
		uint32 nHandleCount = nRayGenCount + nRayMissCount + nHitGroupCount;
		
		VkPhysicalDeviceRayTracingPipelinePropertiesKHR vkRayTracingPipelinePropertie = GetDevice()->GetVulkanDeviceExtensionProperties().m_vkRayTracingPipelineProperties;
		uint32 nHandleSize = vkRayTracingPipelinePropertie.shaderGroupHandleSize;
		assert(nHandleSize < 1452816845);

		uint32 nHandleDataSize = nHandleCount * nHandleSize;
		std::vector<uint8> handleData;
		handleData.resize(nHandleDataSize);

		CRY_VERIFY(Extensions::KHR_ray_tracing_pipeline::vkGetRayTracingShaderGroupHandlesKHR(GetDevice()->GetVkDevice(), m_pipeline,0, nHandleCount, nHandleDataSize, handleData.data()) == VK_SUCCESS);
		
		//file sbt buffer
		uint32 nHandleAlign = alignedValue(nHandleSize, GetDevice()->GetVulkanDeviceExtensionProperties().m_vkRayTracingPipelineProperties.shaderGroupHandleAlignment);
		uint32 nBaseAlign = GetDevice()->GetVulkanDeviceExtensionProperties().m_vkRayTracingPipelineProperties.shaderGroupBaseAlignment;

		m_sRayTracingSBT.m_rayGenRegion.stride = alignedValue(nHandleAlign, nBaseAlign);
		m_sRayTracingSBT.m_rayGenRegion.size = m_sRayTracingSBT.m_rayGenRegion.stride;// The size member of pRayGenShaderBindingTable must be equal to its stride member

		m_sRayTracingSBT.m_rayMissRegion.stride = nHandleAlign;
		m_sRayTracingSBT.m_rayMissRegion.size = alignedValue(nRayMissCount * nHandleAlign, nBaseAlign);

		m_sRayTracingSBT.m_hitGroupRegion.stride = nHandleAlign;
		m_sRayTracingSBT.m_hitGroupRegion.size = alignedValue(nHitGroupCount * nHandleAlign, nBaseAlign);

		m_sRayTracingSBT.m_callAbleRegion.stride = nHandleAlign;
		m_sRayTracingSBT.m_callAbleRegion.size = 0;

		VkDeviceSize sbtSize = m_sRayTracingSBT.m_rayGenRegion.size + m_sRayTracingSBT.m_rayMissRegion.size + m_sRayTracingSBT.m_hitGroupRegion.size;

		auto getHandle = [&](int i) { return handleData.data() + i * nHandleSize; };

		std::vector<uint8> pSBTBuffer;
		pSBTBuffer.resize(static_cast<buffer_size_t>(sbtSize));
		uint8* pData = nullptr;
		uint32 handleIdx = 0;

		{
			pData = pSBTBuffer.data();
			for (uint32 c = 0; c < nRayGenCount; c++)
			{
				memcpy(pData, getHandle(handleIdx), nHandleSize);
				pData += m_sRayTracingSBT.m_rayGenRegion.stride;
				handleIdx++;
			}
		}

		{
			pData = pSBTBuffer.data() + m_sRayTracingSBT.m_rayGenRegion.size;
			for (uint32 c = 0; c < nRayMissCount; c++)
			{
				memcpy(pData, getHandle(handleIdx), nHandleSize);
				pData += m_sRayTracingSBT.m_rayMissRegion.stride;
				handleIdx++;
			}
		}

		{
			pData = pSBTBuffer.data() + m_sRayTracingSBT.m_rayGenRegion.size + m_sRayTracingSBT.m_rayMissRegion.size;
			for (uint32 c = 0; c < nHitGroupCount; c++)
			{
				memcpy(pData, getHandle(handleIdx), nHandleSize);
				pData += m_sRayTracingSBT.m_hitGroupRegion.stride;
				handleIdx++;
			}
		}


		m_sRayTracingSBT.m_rtSBTBuffer.Create(1u, static_cast<buffer_size_t>(sbtSize), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_SHADER_BINDING_TABLE, pSBTBuffer.data());
		
		VkBufferDeviceAddressInfo bufferDeviceAddressInfo = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
		bufferDeviceAddressInfo.buffer = m_sRayTracingSBT.m_rtSBTBuffer.GetDevBuffer()->GetBuffer()->GetHandle();
		VkDeviceAddress sbtAddress = vkGetBufferDeviceAddress(GetDevice()->GetVkDevice(), &bufferDeviceAddressInfo);
		
		m_sRayTracingSBT.m_rayGenRegion.deviceAddress = sbtAddress;
		m_sRayTracingSBT.m_rayMissRegion.deviceAddress = sbtAddress + m_sRayTracingSBT.m_rayGenRegion.size;
		m_sRayTracingSBT.m_hitGroupRegion.deviceAddress = sbtAddress + m_sRayTracingSBT.m_rayGenRegion.size + m_sRayTracingSBT.m_rayMissRegion.size;
		m_sRayTracingSBT.m_callAbleRegion.deviceAddress = sbtAddress + m_sRayTracingSBT.m_rayGenRegion.size + m_sRayTracingSBT.m_rayMissRegion.size + m_sRayTracingSBT.m_hitGroupRegion.size;
	}

	m_isValid = true;

	return validShaderStage != EShaderStage_None;
}

void CDeviceGraphicsCommandInterfaceImpl::SetRayTracingPipelineStateImpl(const CDeviceRayTracingPSO* pDevicePSO)
{
	const CDeviceRayTracingPSO_Vulkan* pVkDevicePSO = reinterpret_cast<const CDeviceRayTracingPSO_Vulkan*>(pDevicePSO);

	vkCmdBindPipeline(GetVKCommandList()->GetVkCommandList(), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, pVkDevicePSO->GetVkPipeline());
	//m_raytracingState.pPipelineState = nullptr;
}

void CDeviceGraphicsCommandInterfaceImpl::SetRayTracingResourcesImpl(uint32 bindSlot, const CDeviceResourceSet* pResources)
{
	const CDeviceResourceSet_Vulkan* pVkResources = reinterpret_cast<const CDeviceResourceSet_Vulkan*>(pResources);
	CRY_ASSERT(pVkResources->GetVKDescriptorSet() != VK_NULL_HANDLE);

	m_raytracingState.custom.pendingBindings.AppendDescriptorSet(bindSlot, pVkResources->GetVKDescriptorSet(), nullptr);
}

void CDeviceGraphicsCommandInterfaceImpl::DispatchRayTracingImpl(uint32 width, uint32 height)
{
	const CDeviceResourceLayout_Vulkan* pVkLayout = reinterpret_cast<const CDeviceResourceLayout_Vulkan*>(m_raytracingState.pResourceLayout.cachedValue);
	const CDeviceRayTracingPSO_Vulkan* pVkPipeline = reinterpret_cast<const CDeviceRayTracingPSO_Vulkan*>(m_raytracingState.pPipelineState.cachedValue);

	ApplyPendingBindings(GetVKCommandList()->GetVkCommandList(), pVkLayout->GetVkPipelineLayout(), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, m_raytracingState.custom.pendingBindings);
	CDeviceBindlessDescriptorManager_Vulkan* pDeviceBindlessDescriptorManager = static_cast<CDeviceBindlessDescriptorManager_Vulkan*>(GetDeviceObjectFactory().GetDeviceBindlessDescriptorManager());
	pDeviceBindlessDescriptorManager->UpdatePendingDescriptorSets();

	if (pVkLayout->m_needBindless)
	{
		vkCmdBindDescriptorSets(
			GetVKCommandList()->GetVkCommandList(),
			VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
			pVkLayout->GetVkPipelineLayout(),
			1, /*TODO:FixMe*/
			1,
			&pDeviceBindlessDescriptorManager->m_bindlessDescriptorSet, 0, nullptr);
	}

	//m_raytracingState.custom.pendingBindings.Reset();
	m_raytracingState.pPipelineState = nullptr;

	GetVKCommandList()->PendingResourceBarriers();
	Extensions::KHR_ray_tracing_pipeline::vkCmdTraceRaysKHR(GetVKCommandList()->GetVkCommandList(), 
		&pVkPipeline->m_sRayTracingSBT.m_rayGenRegion, 
		&pVkPipeline->m_sRayTracingSBT.m_rayMissRegion, 
		&pVkPipeline->m_sRayTracingSBT.m_hitGroupRegion, 
		&pVkPipeline->m_sRayTracingSBT.m_callAbleRegion,
		width, height, 1);
	GetVKCommandList()->m_nCommands += CLCOUNT_DISPATCH;

	m_raytracingState.pPipelineState = nullptr;
}