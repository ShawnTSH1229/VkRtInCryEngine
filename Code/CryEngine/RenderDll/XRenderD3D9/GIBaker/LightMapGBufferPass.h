#pragma once
#include "../GraphicsPipeline/Common/GraphicsPipelineStage.h"
#include "../GraphicsPipeline/Common/GraphicsPipelineStateSet.h"
#include "../GraphicsPipeline/Common/SceneRenderPass.h"
#include "../GraphicsPipeline/Common/FullscreenPass.h"
#include "../GraphicsPipeline/Common/UtilityPasses.h"
#include "GIBakerCommon.h"

class CLightMapGBufferGenerator
{
public:
	void Init(uint32 constantBufferNum, SBakerConfig bakerConfig, std::vector<SAtlasBakeInformation>& atlasBakeInfomation);

	void GenerateLightMapGBuffer(std::vector<SAtlasBakeInformation>& atlasBakeInfomation);
	void ReleaseResource();
private:
	void GenerateGraphicsPSO();
	
	std::vector<CConstantBufferPtr>constantBuffers;

	CDeviceGraphicsPSOPtr m_graphicsPSO;
	CDeviceResourceLayoutPtr m_pResourceLayout;

	std::vector<SAtlasBakeInformation> m_atlasBakeInformation;
	
	CPrimitiveRenderPass         m_lightMapGBufferPass;

	std::vector<SCompiledRenderPrimitive> primitives;

	bool bPsoGenerated = false;

	SBakerConfig m_bakerConfig;
};

