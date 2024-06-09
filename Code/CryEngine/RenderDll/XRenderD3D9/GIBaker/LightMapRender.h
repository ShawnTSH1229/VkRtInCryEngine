#pragma once
#include "GIBakerCommon.h"
#include "../GraphicsPipeline/Common/RayTracingRenderPass.h"

struct SBindlessIndex
{
	uint32 m_vbIndex;
	uint32 m_ibIndex;
};

class CLightMapRenderer
{
public:
	void RenderLightMap(std::vector<SAtlasBakeInformation>& atlasBakeInfomation);
	void InitScene(std::vector<SAtlasBakeInformation>& atlasBakeInfomation, SBakerConfig bakerConfig);
private:

	void CreateAndBuildBLAS(const std::vector<SAtlasBakeInformation>& atlasBakeInfomation);
	void CreateAndBuildTLAS(const std::vector<SAtlasBakeInformation>& atlasBakeInfomation);

	std::vector<SBindlessIndex> m_bindlessIndexArray;
	CGpuBuffer m_pBindlessIndexBuffer;
	SBakerConfig m_bakerConfig;

	std::vector<CRayTracingBottomLevelAccelerationStructurePtr> m_pBLAS;

	CConstantBufferPtr  m_pRayTracingCB;

	CGpuBuffer m_instanceBuffer;
	CRayTracingTopLevelAccelerationStructurePtr m_pRtTopLevelAS;

	std::vector<CRayTracingRenderPass> m_bindlessRayTracingRenderPasses;
};