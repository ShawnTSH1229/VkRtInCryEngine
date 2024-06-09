#pragma once
#include "../Common/GraphicsPipelineStage.h"
#include "../Common/FullscreenPass.h"
#include "../Common/ComputeRenderPass.h"
#include "../Common/GraphicsPipeline.h"
#include "../Common/RayTracingRenderPass.h"

struct SObjVertex
{
	SObjVertex() = default;

	SObjVertex(const Vec3& pos, const Vec3& normal)
		: m_pos(pos)
		, m_normal(normal)
	{
	}

	Vec3 m_pos;
	Vec3 m_normal;
};


using ObjVertexBuffer = std::vector<SObjVertex>;
using ObjIndexBuffer = std::vector<vtx_idx>;

struct SObjectGeometry
{
	ObjVertexBuffer m_objectVB;
	ObjIndexBuffer m_objectIB;

	buffer_handle_t m_pObjectVB;
	buffer_handle_t m_pObjectIB;

	const CDeviceInputStream* m_pObjectVertexInputSet = nullptr;
	const CDeviceInputStream* m_pObjectIndexInputSet = nullptr;

	uint32 m_vbBindlessIndex = 0;
	uint32 m_ibBindlessIndex = 0;

	std::vector<SRayTracingInstanceTransform> m_rayTracingTransforms;
	CRayTracingBottomLevelAccelerationStructurePtr m_pRtBottomLevelAS;

	~SObjectGeometry();
};

struct SBindlessIndex
{
	uint32 m_vbIndex;
	uint32 m_ibIndex;
};

struct SRayCameraMatrix
{
	Matrix44A m_viewProj;
	Matrix44A m_viewInverse;
	Matrix44A m_projInverse;
	Vec4 m_lightDirection;
	Vec4 m_padding0;
	Vec4 m_padding1;
	Vec4 m_padding2;
};

class CBindlessRayTracingTestStage : public CGraphicsPipelineStage
{
public:
	static const EGraphicsPipelineStage StageID = eStage_BindlessRayTracingTest;

	CBindlessRayTracingTestStage(CGraphicsPipeline& graphicsPipeline)
		: CGraphicsPipelineStage(graphicsPipeline)
		, m_bindlessRayTracingRenderPass()
	{

	}
	void Init()   final;
	void Execute(CTexture* RayTracingResultTexture);
	bool IsStageActive(EShaderRenderingFlags flags) const final
	{
		return true;
	}

private:

	void CreateVBAndIB();
	void CreateAndBuildBLAS(CDeviceGraphicsCommandInterface* pCommandInterface);
	void CreateAndBuildTLAS(CDeviceGraphicsCommandInterface* pCommandInterface);
	void CreateUniformBuffer();

private:
	static constexpr uint32 m_nObjectNum = 3;

	SObjectGeometry m_objectSphere;
	SObjectGeometry m_objectCylinder;
	SObjectGeometry m_objectPlane;

	SObjectGeometry* m_objectGeometry[m_nObjectNum];

	CRayTracingTopLevelAccelerationStructurePtr m_pRtTopLevelAS;

	CGpuBuffer m_instanceBuffer;
	CGpuBuffer m_pBindlessIndexBuffer;

	CRayTracingRenderPass m_bindlessRayTracingRenderPass;

	CConstantBufferPtr  m_pRayTracingCB;
	
	bool bInit = false;
};



