// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "VKBase.hpp"
namespace NCryVulkan
{
	namespace Extensions
	{
		class CVulkanDeviceExtension
		{
		public:
			CVulkanDeviceExtension(const char* inVkExtensionName)
				:vkExtensionName(inVkExtensionName) {}

			const char* GetExtensionName()
			{
				return vkExtensionName;
			}
		private:
			const char* vkExtensionName;
		};

		class CVulkanDeviceExtensionWithFeature : public CVulkanDeviceExtension
		{
		public:
			CVulkanDeviceExtensionWithFeature(const char* inVkExtensionName)
				:CVulkanDeviceExtension(inVkExtensionName) {}

			virtual void WritePhysicalDeviceFeatures(VkPhysicalDeviceFeatures2KHR& PhysicalDeviceFeatures2) {};
			virtual void GetPhysicalDeviceProperties(VkPhysicalDeviceProperties2KHR& PhysicalDeviceProperties2, CDevice* pDevice) {};
			virtual void EnablePhysicalDeviceFeatures(VkDeviceCreateInfo& DeviceInfo) {};
		};

		std::vector<CVulkanDeviceExtensionWithFeature*>& GetVulkanDeviceExtensionWithFeatureList();

		namespace EXT_debug_marker
		{
			extern bool                              IsSupported;

			extern PFN_vkDebugMarkerSetObjectTagEXT  SetObjectTag;
			extern PFN_vkDebugMarkerSetObjectNameEXT SetObjectName;
			extern PFN_vkCmdDebugMarkerBeginEXT      CmdDebugMarkerBegin;
			extern PFN_vkCmdDebugMarkerEndEXT        CmdDebugMarkerEnd;
			extern PFN_vkCmdDebugMarkerInsertEXT     CmdDebugMarkerInsert;
		}

		namespace EXT_rasterization_order
		{
			extern bool                              IsSupported;
		}

		//TanGram:VSM:BEGIN
		namespace EXT_device_generated_commands
		{
			extern bool									IsSupported;
			extern PFN_vkCreateIndirectCommandsLayoutNV CmdCreateIndirectCommandsLayout;
			extern PFN_vkGetGeneratedCommandsMemoryRequirementsNV CmdGetGeneratedCommandsMemoryRequirements;
			extern PFN_vkCmdExecuteGeneratedCommandsNV  CmdExecuteGeneratedCommands;
		}
		//TanGram:VSM:END

		//TanGram:VKRT:BEGIN
		namespace KHR_acceleration_structure
		{
			extern  bool                              IsSupported;

			//typedef void (VKAPI_PTR* PFN_vkGetAccelerationStructureBuildSizesKHR)(VkDevice  device, VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
			extern  PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;

			//typedef VkResult(VKAPI_PTR* PFN_vkCreateAccelerationStructureKHR)(VkDevice  device, const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure);
			extern PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;

			//typedef VkDeviceAddress (VKAPI_PTR *PFN_vkGetAccelerationStructureDeviceAddressKHR)(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
			extern PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;

			//typedef void (VKAPI_PTR* PFN_vkCmdBuildAccelerationStructuresKHR)(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
			extern PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
		}

		namespace KHR_ray_tracing_pipeline
		{
			extern  bool                              IsSupported;

			//typedef VkResult(VKAPI_PTR* PFN_vkCreateRayTracingPipelinesKHR)(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
			extern	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;

			//typedef VkResult(VKAPI_PTR* PFN_vkGetRayTracingShaderGroupHandlesKHR)(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
			extern PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;

			//typedef void (VKAPI_PTR* PFN_vkCmdTraceRaysKHR)(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
			extern PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
		}

		//TanGram:VKRT:END

		void Init(CDevice* pDevice, const std::vector<const char*>& loadedExtensions);

		typedef                  std::unordered_map<uintptr_t, std::string>   DeviceObjPointerNameMap;
		static                   std::map<VkDevice, DeviceObjPointerNameMap> debugMarkerObjectNameMap;
		VkResult                 SetObjectName              (VkDevice device, uintptr_t objectPtr, VkDebugMarkerObjectNameInfoEXT* pNameInfo);
		DeviceObjPointerNameMap& GetDeviceObjPointerNameMap (VkDevice device);
		std::string              GetObjectName              (VkDevice device, uintptr_t objectPtr);
		void                     ClearDebugName             (VkDevice device, uintptr_t objectPtr);
	}
}