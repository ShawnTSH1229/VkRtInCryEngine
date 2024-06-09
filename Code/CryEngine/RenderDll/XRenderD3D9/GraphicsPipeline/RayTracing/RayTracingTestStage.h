#pragma once
#include "../Common/GraphicsPipelineStage.h"
#include "../Common/FullscreenPass.h"
#include "../Common/ComputeRenderPass.h"
#include "../Common/GraphicsPipeline.h"
#include "../Common/RayTracingRenderPass.h"

class CRayTracingTestStage : public CGraphicsPipelineStage
{
public:
	static const EGraphicsPipelineStage StageID = eStage_RayTracingTest;

	CRayTracingTestStage(CGraphicsPipeline& graphicsPipeline)
		: CGraphicsPipelineStage(graphicsPipeline)
		, m_rayTracingRenderPass()
	{

	}

	void  Init()   final;
	void Execute();
	bool IsStageActive(EShaderRenderingFlags flags) const final
	{
		return true;
	}

	~CRayTracingTestStage();
	
	void CreateVbIb();
	void CreateAndBuildBLAS(CDeviceGraphicsCommandInterface* pCommandInterface);
	void CreateAndBuildTLAS(CDeviceGraphicsCommandInterface* pCommandInterface);
	void CreateRayAndResultBuffer();

private:

	//VB IB
	struct SRtVertex
	{
		Vec3 m_Pos;
	};

	struct SRayTracingRay
	{
		float m_origin[3];
		uint32 m_mask;
		float m_irection[3];
		float m_TFar;
	};

	using SRtIndex = uint16;

	std::vector<SRtVertex>m_RtVertices;
	std::vector<SRtIndex>m_RtIndices;

	buffer_handle_t m_pVB;
	buffer_handle_t m_pIB;

	SStreamInfo vertexStreamInfo;
	SStreamInfo indexStreamInfo;

	const CDeviceInputStream* m_pVertexInputSet = nullptr;
	const CDeviceInputStream* m_pIndexInputSet = nullptr;

	CRayTracingBottomLevelAccelerationStructurePtr m_pRtBottomLevelAS;
	CRayTracingTopLevelAccelerationStructurePtr m_pRtTopLevelAS;

	CGpuBuffer m_instanceBuffer;
	CGpuBuffer m_rayBuffer;
	CGpuBuffer m_resultBuffer;
	CRayTracingRenderPass m_rayTracingRenderPass;
};