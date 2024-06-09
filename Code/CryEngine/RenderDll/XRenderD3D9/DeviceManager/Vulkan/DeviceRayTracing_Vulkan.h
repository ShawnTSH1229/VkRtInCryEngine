#pragma once
#include <memory>
#include "../../DeviceManager/DeviceResources.h"
#include "../../DeviceManager/DevicePSO.h"
using namespace NCryVulkan;

struct SVulkanRayTracingBLASBuildInfo
{ 
	SVulkanRayTracingBLASBuildInfo()
	{
		m_vkAsBuildGeometryInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR };
		m_vkAsBuildSizeInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR };
	}
	std::vector<VkAccelerationStructureGeometryKHR>m_vkRtGeometryTrianglesInfo;
	
	std::vector<VkAccelerationStructureBuildRangeInfoKHR>m_vkAsBuildRangeInfo; // Build range info
	VkAccelerationStructureBuildGeometryInfoKHR m_vkAsBuildGeometryInfo; // Build geometry info
	VkAccelerationStructureBuildSizesInfoKHR m_vkAsBuildSizeInfo; // Build size info
};

class CVulkanRayTracingBottomLevelAccelerationStructure : public CRayTracingBottomLevelAccelerationStructure
{
public:
	CVulkanRayTracingBottomLevelAccelerationStructure(const SRayTracingBottomLevelASCreateInfo& rtBottomLevelCreateInfo, CDevice* pDevice);

	virtual uint64 GetAccelerationStructureAddress()override;

	VkAccelerationStructureKHR accelerationStructureHandle = nullptr;
	VkDeviceAddress accelerationStructureDeviceAddress;
	CGpuBuffer m_accelerationStructureBuffer;
private:
	CDevice* const  m_pDevice;
};

struct SVulkanRayTracingTLASBuildInfo
{
	SVulkanRayTracingTLASBuildInfo()
	{
		m_vkRtGeometryInstancesInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR };
		m_vkAsBuildGeometryInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR };
		m_vkAsBuildSizeInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR };
	}

	VkAccelerationStructureGeometryKHR m_vkRtGeometryInstancesInfo;
	VkAccelerationStructureBuildGeometryInfoKHR m_vkAsBuildGeometryInfo; // Build geometry info
	VkAccelerationStructureBuildSizesInfoKHR m_vkAsBuildSizeInfo; // Build size info
};




class CVulkanRayTracingTopLevelAccelerationStructure : public CRayTracingTopLevelAccelerationStructure
{
public:
	CVulkanRayTracingTopLevelAccelerationStructure(const SRayTracingTopLevelASCreateInfo& rtTopLevelCreateInfo, CDevice* pDevice);

	virtual CGpuBuffer* GetAccelerationStructureBuffer() { return &m_accelerationStructureBuffer; };

	VkAccelerationStructureKHR accelerationStructureHandle = nullptr;
	VkDeviceAddress accelerationStructureDeviceAddress;
	CGpuBuffer m_accelerationStructureBuffer;

private:
	CDevice* const  m_pDevice;
};

class CDeviceRayTracingSBT_Vulkan
{
	CDeviceRayTracingSBT_Vulkan(CDevice* pDevice)
		: m_pDevice(pDevice)
	{

	}



	CDevice* m_pDevice;
};

class CDeviceRayTracingPSO_Vulkan : public CDeviceRayTracingPSO
{
public:
	CDeviceRayTracingPSO_Vulkan(CDevice* pDevice)
		: m_pDevice(pDevice)
		, m_pipeline(VK_NULL_HANDLE)
	{}

	~CDeviceRayTracingPSO_Vulkan();

	virtual bool      Init(const CDeviceRayTracingPSODesc& desc) final;

	const VkPipeline& GetVkPipeline() const { return m_pipeline; }

	struct SRayTracingSBT
	{
		CGpuBuffer m_rtSBTBuffer;

		VkStridedDeviceAddressRegionKHR m_rayGenRegion;
		VkStridedDeviceAddressRegionKHR m_hitGroupRegion;
		VkStridedDeviceAddressRegionKHR m_rayMissRegion;
		VkStridedDeviceAddressRegionKHR m_callAbleRegion;
	};
	SRayTracingSBT m_sRayTracingSBT;

protected:
	CDevice* m_pDevice;
	VkPipeline m_pipeline;
};

typedef std::shared_ptr<CDeviceRayTracingPSO_Vulkan> CDeviceRayTracingPSO_VulkanPtr;

