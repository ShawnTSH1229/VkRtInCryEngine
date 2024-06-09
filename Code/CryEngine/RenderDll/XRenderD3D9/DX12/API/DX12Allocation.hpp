//TanGram::DX12Optimization

#pragma once


#define MIN_PLACED_BUFFER_SIZE (64*1024)

namespace NCryDX12
{
    class CResourceHandle;
    
    enum class EGPUMemManageMethod
    {
        eAutoManage,
        //Automatic manage gpu memory; use placed resource
        eManuManage,
        //Manual manage gpu memory;use commit resource
    };

    struct SAllocatorConfig
    {
        //Automatic manage
        D3D12_HEAP_TYPE m_HeapType;
        D3D12_HEAP_FLAGS m_HeapFlags;

        //Manual manage
        D3D12_RESOURCE_STATES m_ResourceStates;
        D3D12_RESOURCE_FLAGS m_ResourceFlags;
    };

    class CDX12BuddyAllocator : public CDeviceObject
    {
    public:
        CDX12BuddyAllocator(CDevice* device);

        void Init(EGPUMemManageMethod memMangeMethod, SAllocatorConfig allocatorConfig, uint32 minBlockSize,
                  uint32 maxBlockSize);

        void Allocate(uint32 allocateSizeByte, uint32 alignment, CResourceHandle* resourceHandle);

        void DeAllocate();

    private:
        EGPUMemManageMethod m_GpuManageMethod;
        SAllocatorConfig m_AllocatorConfig;
        uint32 m_MinBlockSize;
        uint32 m_MaxBlockSize;
        uint32 m_MaxOrder;
        
        std::vector<std::set<uint32>> m_ResourceOffset;

        ID3D12Heap* pHeap;
        
        inline uint32 ConvertSizeToOrder(uint32 size)
        {
            uint32 blockNum = (size + m_MinBlockSize - 1) / m_MinBlockSize;
            return IntegerLog2_RoundUp(blockNum);
        }
    };
}
