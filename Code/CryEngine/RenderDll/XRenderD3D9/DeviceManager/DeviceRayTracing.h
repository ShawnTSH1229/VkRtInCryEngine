#pragma once

#if (CRY_RENDERER_DIRECT3D >= 120)
#elif (CRY_RENDERER_DIRECT3D >= 110)
#if defined(USE_NV_API)
#include "D3D11/DeviceResources_D3D11_NVAPI.h"
#endif
#endif
#include <CryRenderer/RenderElements/RendElement.h>
#include <memory>
class CGpuBuffer;

// Enum specifying the type of build operation to perform
enum class EBuildAccelerationStructureMode : uint32
{
	eBuild = 1 << 1, 												// Specifies that the destination acceleration structure will be built using the specified geometries

	//unsupported currently
	eUpdate = 1 << 2,											// Specifies that the destination acceleration structure will be built using data in a source acceleration structure, updated by the specified geometries
};
DEFINE_ENUM_FLAG_OPERATORS(EBuildAccelerationStructureMode);

// Bitmask specifying additional parameters for acceleration structure builds
enum class EBuildAccelerationStructureFlag : uint32
{
	eBuild_None = 0,
	eBuild_PreferTrace = 1 << 2,						// Indicates that the given acceleration structure build should prioritize trace performance over build time
	eBuild_PreferBuild = 1 << 3,						// Indicates that the given acceleration structure build should prioritize build time over trace performance
};
DEFINE_ENUM_FLAG_OPERATORS(EBuildAccelerationStructureFlag);

struct SRayTracingAccelerationStructSize
{
public:
	uint64 m_nAccelerationStructureSize = 0;			// The size in bytes required in a AccelerationStructure for a build or update operation
	uint64 m_nUpdateScratchSize = 0;					// The size in bytes required in a scratch buffer for an update operation.
	uint64 m_nBuildScratchSize = 0;						// The size in bytes required in a scratch buffer for a build operation.
};

struct SRayTracingGeometryTriangle
{
	struct STriangleVertexInfo
	{
		const CDeviceInputStream* m_sVertexStreaming;
		InputLayoutHandle m_hVertexPositionFormat = EDefaultInputLayouts::P3F;		// The VkFormat of each vertex element
		uint32 m_nMaxVertex = 0;											// The highest index of a vertex that will be addressed by a build command using this structure.
	}m_sTriangVertexleInfo;													// Triangle info: Structure specifying a triangle geometry in a bottom-level acceleration structure

	struct SRangeInfo
	{
		uint32 m_nPrimitiveCount = 0;										// The number of primitives for a corresponding acceleration structure geometry
		uint32 m_nPrimitiveOffset = 0;										// An offset in bytes into the memory where primitive data is defined.
		uint32 m_nFirstVertex = 0;											// The index of the first vertex to build from for triangle geometry
	}m_sRangeInfo;															// Range info: structure specifying build offsets and counts for acceleration structure builds

	bool bForceOpaque = true;											// Set this to false turns off any-hit shader
	bool bEnable = true;												// Whether this geometry triangle will be enabled
};

struct SRayTracingBottomLevelASCreateInfo
{
	std::vector<SRayTracingGeometryTriangle>m_rtGeometryTriangles;

	struct STriangleIndexInfo
	{
		const CDeviceInputStream* m_sIndexStreaming;
		uint32 m_nIndexBufferOffset = 0;
	}m_sSTriangleIndexInfo;

	EBuildAccelerationStructureFlag m_eBuildFlag = EBuildAccelerationStructureFlag::eBuild_None;
};

class CRayTracingBottomLevelAccelerationStructure
{
public:
	CRayTracingBottomLevelAccelerationStructure(const SRayTracingBottomLevelASCreateInfo& sRtBlASCreateInfo)
		:m_sRtBlASCreateInfo(sRtBlASCreateInfo)
	{}

	virtual uint64 GetAccelerationStructureAddress() { return 0; };

	SRayTracingBottomLevelASCreateInfo m_sRtBlASCreateInfo;
	SRayTracingAccelerationStructSize m_sSizeInfo;
};

typedef std::shared_ptr<CRayTracingBottomLevelAccelerationStructure> CRayTracingBottomLevelAccelerationStructurePtr;

// Top level acceleration structure
struct SAccelerationStructureInstanceData
{
	Matrix34 m_aTransform;									// A transformation to be applied to the acceleration structure
	uint32 m_nInstanceCustomIndex : 24;							// A 24-bit user-specified index value accessible to ray shaders in the InstanceCustomIndexKHR built-in
	uint32 m_nMask : 8;											// An 8-bit visibility mask for the geometry. The instance may only be hit if Cull Mask & instance.mask != 0
	uint32 m_instanceShaderBindingTableRecordOffset : 24;		// A 24-bit offset used in calculating the hit shader binding table index
	uint32 m_flags : 8;											// An 8-bit mask values to apply to this instance
	uint64 accelerationStructureReference;						// The device address of the acceleration structure
};

struct SRayTracingInstanceTransform
{
	Matrix34 m_aTransform;
};

struct SAccelerationStructureInstanceInfo
{
	CRayTracingBottomLevelAccelerationStructurePtr m_blas;
	std::vector<SRayTracingInstanceTransform> m_transformPerInstance;
	std::vector<uint32> m_nCustomIndex;
	uint32 m_defaultIndex = 0;
	uint8 m_rayMask = 0xFF;
};

struct SRayTracingTopLevelASCreateInfo
{
	std::vector<CRayTracingBottomLevelAccelerationStructurePtr> m_aBLASPerInstance;
	std::vector<uint32> m_nPreGeometryNumSum;

	uint32 m_nInstanceTransform = 0;
	uint32 m_nTotalGeometry = 0;
	uint32 m_nHitShaderNumPerTriangle = 1;
	uint32 m_nMissShaderNum = 1;
};

class CRayTracingTopLevelAccelerationStructure
{
public:
	CRayTracingTopLevelAccelerationStructure(const SRayTracingTopLevelASCreateInfo& sRtTLASCreateInfo)
		:m_sRtTLASCreateInfo(sRtTLASCreateInfo)
	{}
	
	virtual CGpuBuffer* GetAccelerationStructureBuffer() { return nullptr; };

	SRayTracingTopLevelASCreateInfo m_sRtTLASCreateInfo;
	SRayTracingAccelerationStructSize m_sSizeInfo;
};

typedef std::shared_ptr<CRayTracingTopLevelAccelerationStructure> CRayTracingTopLevelAccelerationStructurePtr;





