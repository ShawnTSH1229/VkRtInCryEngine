#pragma once
#include "../../DeviceManager/DeviceResources.h"

enum EBindlessDescriptorBindingType
{
	e_bdstUniform_0 = 0,
	e_bdstUniform_1,
	e_bdstStorage_0,
	e_bdstStorage_1,
	e_bdstStorage_2,
	e_bdstStorage_3,
	e_bdbtNum,

	e_bdstUniformNum = e_bdstUniform_1 - e_bdstUniform_0 + 1,
	e_bdstStorageNum = e_bdstStorage_3 - e_bdstStorage_0 + 1,
};

DEFINE_ENUM_FLAG_OPERATORS(EBindlessDescriptorBindingType);

struct SBufferBindingState
{
	uint32 m_currentFreeIndex;
	std::vector<uint32>m_freeIndexArray;
	std::vector<VkBuffer> m_buffers;
	std::vector<VkDescriptorBufferInfo>m_bufferInfos;
};

class CDeviceBindlessDescriptorManager_Vulkan : public CDeviceBindlessDescriptorManager
{
public:
	CDeviceBindlessDescriptorManager_Vulkan();
	void Init();
	void UpdatePendingDescriptorSets();

	bool m_isUpdateDescriptor = false;
	VkDescriptorSetLayout m_bindlessDescriptorSetLayout;
	VkDescriptorSet m_bindlessDescriptorSet;
	VkDescriptorPool m_bindlessDescriptorPool;

	std::vector<VkWriteDescriptorSet> m_descriptorSetWrites;

	SBufferBindingState m_BufferBindingState[EBindlessDescriptorBindingType::e_bdstUniformNum + EBindlessDescriptorBindingType::e_bdstStorageNum];

private:

	void CreateBindlessDescriptorPool();
	
};