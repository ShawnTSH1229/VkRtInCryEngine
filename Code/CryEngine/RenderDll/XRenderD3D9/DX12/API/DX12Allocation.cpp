//TanGram::DX12Optimization
#include "StdAfx.h"
#include "DX12/CryDX12.hpp"
#include "DX12Allocation.hpp"


namespace NCryDX12
{
    CDX12BuddyAllocator::CDX12BuddyAllocator(CDevice* device)
        : CDeviceObject(device)
    {
    }

    void CDX12BuddyAllocator::Init(EGPUMemManageMethod memMangeMethod, SAllocatorConfig allocatorConfig,
        uint32 minBlockSize, uint32 maxBlockSize)
    {
        m_GpuManageMethod = memMangeMethod;
        m_AllocatorConfig = allocatorConfig;
        m_MinBlockSize = minBlockSize;
        m_MaxBlockSize = maxBlockSize;

        m_MaxOrder = ConvertSizeToOrder(maxBlockSize);//e.g.:maxBlockSize = 8 => order = 3. 0 1 2 3
        
        m_ResourceOffset.clear();
        m_ResourceOffset.resize(m_MaxOrder + 1);
        m_ResourceOffset[m_MaxOrder].insert(0);

        if(m_GpuManageMethod == EGPUMemManageMethod::eAutoManage)//use placed resource
        {
            D3D12_HEAP_PROPERTIES heapProperties;
            heapProperties.Type = m_AllocatorConfig.m_HeapType;
            heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            heapProperties.CreationNodeMask = 0;
            heapProperties.VisibleNodeMask = 0;

            D3D12_HEAP_DESC heapDesc;
            heapDesc.SizeInBytes = m_MaxBlockSize;
            heapDesc.Properties = heapProperties;
            heapDesc.Alignment = MIN_PLACED_BUFFER_SIZE;
            heapDesc.Flags = m_AllocatorConfig.m_HeapFlags;
            GetDevice()->GetD3D12Device()->CreateHeap(&heapDesc, IID_GFX_ARGS(&pHeap));
        }
        else//use commit resource
        {
            
        }
    }

    void CDX12BuddyAllocator::Allocate(uint32 allocateSizeByte, uint32 alignment, CResourceHandle* resourceHandle)
    {
        
    }

    void CDX12BuddyAllocator::DeAllocate()
    {
        
    }
}

