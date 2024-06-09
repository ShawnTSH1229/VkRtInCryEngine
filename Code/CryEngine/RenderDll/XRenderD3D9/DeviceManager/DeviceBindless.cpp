#include "StdAfx.h"
#include <unordered_map>
#include "xxhash.h"
#include "DeviceObjects.h"
#include "DeviceCommandListCommon.h"
#include "Common/ReverseDepth.h"
#include "Common/Textures/TextureHelpers.h"
#include "../GraphicsPipeline/Common/GraphicsPipelineStateSet.h"

CDeviceBindlessDescriptorManager* CDeviceObjectFactory::GetDeviceBindlessDescriptorManager()
{
	return m_pDeviceBindlessDescriptorManager;
}

uint32 CDeviceObjectFactory::SetBindlessStorageBuffer(const CDeviceInputStream* DeviceStreaming, uint32 bindingIndex)
{
	return SetBindlessStorageBufferImpl(DeviceStreaming, bindingIndex);
}

void CDeviceObjectFactory::UnBindBindlessResource(uint32 descriptorIndex, uint32 unBindIndex)
{
	UnBindBindlessResourceImpl(descriptorIndex, unBindIndex);
}