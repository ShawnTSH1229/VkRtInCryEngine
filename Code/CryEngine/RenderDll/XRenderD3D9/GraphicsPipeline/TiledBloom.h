#pragma once

#include "Common/GraphicsPipelineStage.h"
#include "Common/FullscreenPass.h"
#include "Common/GraphicsPipeline.h"

class CTiledBloomStage : public CGraphicsPipelineStage
{
public:
	static const EGraphicsPipelineStage StageID = eStage_TiledBloom;

	CTiledBloomStage(CGraphicsPipeline& graphicsPipeline)
		: CGraphicsPipelineStage(graphicsPipeline)
		, m_pass1H(&graphicsPipeline)
		, m_pass1V(&graphicsPipeline)
		, m_pass2H(&graphicsPipeline)
		, m_pass2V(&graphicsPipeline) {}

	bool IsStageActive(EShaderRenderingFlags flags) const final
	{
		//return CRenderer::CV_r_HDRBloom == 0 && CRenderer::CV_r_PostProcess && CRenderer::CV_r_HDRTiledBloom == 1;
		return false;
	}

	void Execute();

private:
	CFullscreenPass m_pass1H;
	CFullscreenPass m_pass1V;
	CFullscreenPass m_pass2H;
	CFullscreenPass m_pass2V;
};

