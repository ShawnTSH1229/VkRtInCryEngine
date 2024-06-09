// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "../../DeviceManager/DeviceResourceSet.h"

using namespace NCryVulkan;

static NCryVulkan::CDevice* GetDevice()
{
	return GetDeviceObjectFactory().GetVKDevice();
}

static NCryVulkan::CCommandScheduler* GetScheduler()
{
	return GetDeviceObjectFactory().GetVKScheduler();
}



class CDeviceResourceSet_Vulkan : public CDeviceResourceSet
{
public:
	CDeviceResourceSet_Vulkan(CDevice* pDevice, CDeviceResourceSet::EFlags flags)
		: CDeviceResourceSet(flags)
		, m_pDevice(pDevice)
		, m_descriptorSetLayout(VK_NULL_HANDLE)
		, m_pDescriptorSet(nullptr)
	{}

	~CDeviceResourceSet_Vulkan();

	virtual bool UpdateImpl(const CDeviceResourceSetDesc& desc, CDeviceResourceSetDesc::EDirtyFlags dirtyFlags);

	VkDescriptorSetLayout GetVkDescriptorSetLayout() const { VK_ASSERT(m_descriptorSetLayout != VK_NULL_HANDLE, "Set not built"); return m_descriptorSetLayout; }
	VkDescriptorSet       GetVKDescriptorSet()       const { return m_descriptorSetHandle; }

	static VkDescriptorSetLayout CreateLayout(const VectorMap<SResourceBindPoint, SResourceBinding>& resources, bool needBindless = false);


	// Calls specified functor with signature "void (CMemoryResource* pResource, bool bWritable)" for all resources used by the resource-set.
	template<typename TFunctor>
	void EnumerateInUseResources(const TFunctor& functor) const
	{
		for (const std::pair<_smart_ptr<CMemoryResource>, bool>& pair : m_inUseResources)
		{
			CMemoryResource* const pResource = pair.first.get();
			const bool bWritable = pair.second;
			functor(pResource, bWritable);
		}
	}

protected:

	bool FillDescriptors(const CDeviceResourceSetDesc& desc, const SDescriptorSet* descriptorSet);
	void ReleaseDescriptors();

	static VkDescriptorType GetDescriptorType(SResourceBindPoint bindPoint)
	{
		switch (bindPoint.slotType)
		{
		case SResourceBindPoint::ESlotType::ConstantBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		case SResourceBindPoint::ESlotType::Sampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		case SResourceBindPoint::ESlotType::TextureAndBuffer:
		{
			if (uint8(bindPoint.flags & SResourceBindPoint::EFlags::IsTexture))
			{
				return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			}
			//TanGram:VKRT:BEGIN
			else if (uint8(bindPoint.flags & SResourceBindPoint::EFlags::IsAccelerationStructured))
			{
				return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
			}
			//TanGram:VKRT:END
			else
			{
				return uint8(bindPoint.flags & SResourceBindPoint::EFlags::IsStructured)
					? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER        // StructuredBuffer<*> : register(t*)
					: VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER; // Buffer<*> : register(t*)
			}

		}

		case SResourceBindPoint::ESlotType::UnorderedAccessView:
		{
			if (uint8(bindPoint.flags & SResourceBindPoint::EFlags::IsTexture))
			{
				return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			}
			else
			{
				return uint8(bindPoint.flags & SResourceBindPoint::EFlags::IsStructured)
					? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER        // RWStructuredBuffer<*> : register(u*)
					: VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER; // RWBuffer<*> : register(u*)
			}
		}
		}

		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}

	static bool IsTexelBuffer(VkDescriptorType descriptorType)
	{
		return descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER || descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	}

	CDevice* const                                    m_pDevice;
	CAutoHandle<VkDescriptorSetLayout>                m_descriptorSetLayout;
	VkDescriptorSet                                   m_descriptorSetHandle;


	std::vector<std::pair<_smart_ptr<CMemoryResource>, bool>> m_inUseResources; // Required to keep resources alive while in resource-set.

	SDescriptorSet*      m_pDescriptorSet;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TanGram:VSM:BEGIN
class CDeviceResourceIndirectLayout_Vulkan : public CDeviceResourceIndirectLayout
{
public:
	CDeviceResourceIndirectLayout_Vulkan(CDevice* pDevice)
		: m_pDevice(pDevice)
	{

	}
	bool Init(const SDeviceResourceIndirectLayoutDesc& desc);

	const VkIndirectCommandsLayoutNV& GetVkIndirectCmdLayout() const { return indirectCmdsLayout; }

private:
	CDevice* m_pDevice;
	VkIndirectCommandsLayoutNV indirectCmdsLayout;
};
//TanGram:VSM:END

class CDeviceResourceLayout_Vulkan : public CDeviceResourceLayout
{
public:
	CDeviceResourceLayout_Vulkan(CDevice* pDevice, UsedBindSlotSet requiredResourceBindings)
		: CDeviceResourceLayout(requiredResourceBindings)
		, m_pDevice(pDevice)
		, m_pipelineLayout(VK_NULL_HANDLE)
		, m_hash(0)
		, m_needBindless(false)
	{}

	~CDeviceResourceLayout_Vulkan();

	bool                    Init(const SDeviceResourceLayoutDesc& desc);

	const VkPipelineLayout&   GetVkPipelineLayout() const { return m_pipelineLayout; }
	uint64                    GetHash()             const { return m_hash; }
	const std::vector<uint8>& GetEncodedLayout()    const { return *GetDeviceObjectFactory().LookupResourceLayoutEncoding(m_hash); }

	bool m_needBindless;

protected:
	std::vector<uint8>        EncodeDescriptorSet(const VectorMap<SResourceBindPoint, SResourceBinding>& resources);

	CDevice*                           m_pDevice;
	VkPipelineLayout                   m_pipelineLayout;

	uint64                             m_hash;
};

