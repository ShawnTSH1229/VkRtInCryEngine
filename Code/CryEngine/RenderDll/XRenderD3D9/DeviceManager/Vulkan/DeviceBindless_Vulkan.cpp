#include "Vulkan/API/VKInstance.hpp"
#include "Vulkan/API/VKBufferResource.hpp"
#include "Vulkan/API/VKImageResource.hpp"
#include "Vulkan/API/VKSampler.hpp"
#include "Vulkan/API/VKExtensions.hpp"

#include <Common/Renderer.h>
#include <CryCore/Assert/CryAssert.h>

#include "DeviceResourceSet_Vulkan.h"	
#include "DevicePSO_Vulkan.h"	
#include "DeviceBindless_Vulkan.h"


CDeviceBindlessDescriptorManager* CDeviceObjectFactory::InitBindlessDescriptorManagerImpl()
{
	CDeviceBindlessDescriptorManager_Vulkan* pDeviceBindlessDescriptorManager = new CDeviceBindlessDescriptorManager_Vulkan();
	pDeviceBindlessDescriptorManager->Init();
	return pDeviceBindlessDescriptorManager;
}

static inline VkDescriptorType ConvertToDescriptorType(uint32 index)
{
	switch (index)
	{
	case EBindlessDescriptorBindingType::e_bdstUniform_0:    
	case EBindlessDescriptorBindingType::e_bdstUniform_1:    
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	case EBindlessDescriptorBindingType::e_bdstStorage_0:
	case EBindlessDescriptorBindingType::e_bdstStorage_1:
	case EBindlessDescriptorBindingType::e_bdstStorage_2:
	case EBindlessDescriptorBindingType::e_bdstStorage_3:
		return  VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	default: CRY_ASSERT(false);
	}
	return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

void CDeviceBindlessDescriptorManager_Vulkan::CreateBindlessDescriptorPool()
{
	// allocate descriptor pool
	const uint32 setCount = 65535;
	const uint32 sampledImageCount = 32 * 65536;
	const uint32 storageImageCount = 1 * 65536;
	const uint32 uniformBufferCount = 1 * 65536;
	const uint32 uniformBufferDynamicCount = 4 * 65536;
	const uint32 storageBufferCount = 1 * 65536;
	const uint32 uniformTexelBufferCount = 8192;
	const uint32 storageTexelBufferCount = 8192;
	const uint32 samplerCount = 2 * 65536;
	const uint32 accelerationStructureCount = 32 * 1024;

	VkDescriptorPoolSize poolSizes[9];

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	poolSizes[0].descriptorCount = sampledImageCount;

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	poolSizes[1].descriptorCount = storageImageCount;

	poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[2].descriptorCount = uniformBufferCount;

	poolSizes[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[3].descriptorCount = uniformBufferDynamicCount;

	poolSizes[4].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	poolSizes[4].descriptorCount = storageBufferCount;

	poolSizes[5].type = VK_DESCRIPTOR_TYPE_SAMPLER;
	poolSizes[5].descriptorCount = samplerCount;

	poolSizes[6].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	poolSizes[6].descriptorCount = uniformTexelBufferCount;

	poolSizes[7].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	poolSizes[7].descriptorCount = storageTexelBufferCount;

	poolSizes[8].type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
	poolSizes[8].descriptorCount = accelerationStructureCount;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = nullptr;

	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;//TanGram:BINDLESS

	descriptorPoolCreateInfo.maxSets = setCount;
	descriptorPoolCreateInfo.poolSizeCount = CRY_ARRAY_COUNT(poolSizes);
	descriptorPoolCreateInfo.pPoolSizes = poolSizes;

	CRY_ASSERT(vkCreateDescriptorPool(GetDevice()->GetVkDevice(), &descriptorPoolCreateInfo, nullptr, &m_bindlessDescriptorPool) == VK_SUCCESS);
}

void CDeviceBindlessDescriptorManager_Vulkan::UpdatePendingDescriptorSets()
{
	uint32 setWritesSize = m_descriptorSetWrites.size();
	if (m_isUpdateDescriptor == false && setWritesSize > 0)
	{
		vkUpdateDescriptorSets(
			GetDevice()->GetVkDevice(),
			static_cast<uint32_t>(setWritesSize),
			m_descriptorSetWrites.data(), 0, nullptr);
		m_descriptorSetWrites.clear();
		m_isUpdateDescriptor = true;
	}
}

void CDeviceBindlessDescriptorManager_Vulkan::Init()
{
	CreateBindlessDescriptorPool();

	constexpr uint32 bindlessDescriptorCounrPerType = 1024;

	for (uint32 indexStorage = 0; indexStorage < EBindlessDescriptorBindingType::e_bdstUniformNum + EBindlessDescriptorBindingType::e_bdstStorageNum; indexStorage++)
	{
		m_BufferBindingState[indexStorage].m_freeIndexArray.resize(1024);
		for (uint32 index = 0; index < m_BufferBindingState[indexStorage].m_freeIndexArray.size(); index++)
		{
			m_BufferBindingState[indexStorage].m_freeIndexArray[index] = index + 1;
		}
		m_BufferBindingState[indexStorage].m_currentFreeIndex = 0;
	}

	std::vector<VkDescriptorBindingFlags> bindingFlags;
	std::vector< VkDescriptorSetLayoutBinding>descriptorSetLayoutBindings;

	for (uint32 index = 0; index < EBindlessDescriptorBindingType::e_bdbtNum; index++)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
		descriptorSetLayoutBinding.binding = index;
		descriptorSetLayoutBinding.descriptorType = ConvertToDescriptorType(index);
		descriptorSetLayoutBinding.descriptorCount = bindlessDescriptorCounrPerType;
		descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);

		bindingFlags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT);
	}

	VkDescriptorSetLayoutBindingFlagsCreateInfo descriptorSetLayoutBindingFlagsCreateInfo;
	descriptorSetLayoutBindingFlagsCreateInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO };
	descriptorSetLayoutBindingFlagsCreateInfo.bindingCount = EBindlessDescriptorBindingType::e_bdbtNum;
	descriptorSetLayoutBindingFlagsCreateInfo.pBindingFlags = bindingFlags.data();

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings.data();
	descriptorSetLayoutCreateInfo.bindingCount = EBindlessDescriptorBindingType::e_bdbtNum;
	descriptorSetLayoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
	//descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.pNext = &descriptorSetLayoutBindingFlagsCreateInfo;

	CRY_ASSERT(vkCreateDescriptorSetLayout(GetDevice()->GetVkDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_bindlessDescriptorSetLayout) == VK_SUCCESS);

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	descriptorSetAllocateInfo.descriptorPool = m_bindlessDescriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &m_bindlessDescriptorSetLayout;

	VkDescriptorSetVariableDescriptorCountAllocateInfoEXT descriptorSetVariableDescriptorCountAllocateInfoEXT;
	descriptorSetVariableDescriptorCountAllocateInfoEXT = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT };
	uint32 maxAllocatableCount = bindlessDescriptorCounrPerType - 1;
	descriptorSetVariableDescriptorCountAllocateInfoEXT.descriptorSetCount = 1;
	// This number is the max allocatable count
	descriptorSetVariableDescriptorCountAllocateInfoEXT.pDescriptorCounts = &maxAllocatableCount;
	descriptorSetAllocateInfo.pNext = &descriptorSetVariableDescriptorCountAllocateInfoEXT;


	CRY_ASSERT(vkAllocateDescriptorSets(GetDevice()->GetVkDevice(), &descriptorSetAllocateInfo, &m_bindlessDescriptorSet) == VK_SUCCESS);
}

CDeviceBindlessDescriptorManager_Vulkan::CDeviceBindlessDescriptorManager_Vulkan()
{

}

uint32 CDeviceObjectFactory::SetBindlessStorageBufferImpl(const CDeviceInputStream* DeviceStreaming, uint32 bindingIndex)
{
	CRY_ASSERT(bindingIndex >= 2 && bindingIndex <= 5);

	CDeviceBindlessDescriptorManager_Vulkan* pDeviceBindlessDescriptorManager = static_cast<CDeviceBindlessDescriptorManager_Vulkan*>(m_pDeviceBindlessDescriptorManager);
	
	buffer_size_t offset;
	CBufferResource* const pActualBuffer = gcpRendD3D.m_DevBufMan.GetD3D(((SStreamInfo*)DeviceStreaming)->hStream, &offset);
	VkBuffer buffer = pActualBuffer->GetHandle();

	SBufferBindingState& storageufferBindingState = pDeviceBindlessDescriptorManager->m_BufferBindingState[bindingIndex];

	uint32 currentFreeIndex = storageufferBindingState.m_currentFreeIndex;
	storageufferBindingState.m_currentFreeIndex = storageufferBindingState.m_freeIndexArray[storageufferBindingState.m_currentFreeIndex];

	uint32 bufferSize = storageufferBindingState.m_buffers.size();
	if (currentFreeIndex >= bufferSize)
	{
		storageufferBindingState.m_buffers.resize(alignedValue(currentFreeIndex + 1, 1024));
		storageufferBindingState.m_bufferInfos.resize(alignedValue(currentFreeIndex + 1, 1024));
	}
	storageufferBindingState.m_buffers[currentFreeIndex] = buffer;
	
	
	VkDescriptorBufferInfo descriptorBufferInfo;
	//range is the size in bytes that is used for this descriptor update, or VK_WHOLE_SIZE to use the range from offset to the end of the buffer.
	descriptorBufferInfo.range = VK_WHOLE_SIZE;
	descriptorBufferInfo.buffer = storageufferBindingState.m_buffers[currentFreeIndex];
	descriptorBufferInfo.offset = offset;
	storageufferBindingState.m_bufferInfos[currentFreeIndex] = descriptorBufferInfo;

	VkWriteDescriptorSet writeDescriptorSet = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
	writeDescriptorSet.dstSet = pDeviceBindlessDescriptorManager->m_bindlessDescriptorSet;
	writeDescriptorSet.dstBinding = bindingIndex;
	writeDescriptorSet.dstArrayElement = currentFreeIndex;
	writeDescriptorSet.descriptorType = ConvertToDescriptorType(bindingIndex);
	writeDescriptorSet.pBufferInfo = &storageufferBindingState.m_bufferInfos[currentFreeIndex];
	writeDescriptorSet.descriptorCount = 1;

	pDeviceBindlessDescriptorManager->m_descriptorSetWrites.emplace_back(writeDescriptorSet);
	pDeviceBindlessDescriptorManager->m_isUpdateDescriptor = false;

	return currentFreeIndex;
}

void CDeviceObjectFactory::UnBindBindlessResourceImpl(uint32 descriptorIndex ,uint32 unBindIndex)
{
	CDeviceBindlessDescriptorManager_Vulkan* pDeviceBindlessDescriptorManager = static_cast<CDeviceBindlessDescriptorManager_Vulkan*>(m_pDeviceBindlessDescriptorManager);
	SBufferBindingState& bufferBindingState = pDeviceBindlessDescriptorManager->m_BufferBindingState[unBindIndex];
	bufferBindingState.m_buffers[descriptorIndex] = VkBuffer();
	bufferBindingState.m_bufferInfos[descriptorIndex] = VkDescriptorBufferInfo();
	bufferBindingState.m_freeIndexArray[descriptorIndex] = bufferBindingState.m_currentFreeIndex;
	bufferBindingState.m_currentFreeIndex = descriptorIndex;

	//todo@remove buffer
}