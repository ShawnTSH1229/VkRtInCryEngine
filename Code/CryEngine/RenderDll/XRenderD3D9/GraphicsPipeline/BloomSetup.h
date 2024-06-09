//TanGram: TiledBloom
#pragma once
#include "Common/GraphicsPipelineStage.h"
#include "Common/FullscreenPass.h"
#include "Common/ComputeRenderPass.h"
#include "Common\GraphicsPipeline.h"

struct STypedBloomSetupConstants
{
	Vec4 TexSize;
	Vec4i BufferSize;
};


class CBloomSetupStage : public CGraphicsPipelineStage
{
public:
	static const EGraphicsPipelineStage StageID = eStage_BloomSetup;

	CBloomSetupStage(CGraphicsPipeline& graphicsPipeline)
		: CGraphicsPipelineStage(graphicsPipeline)
		, m_passBloomSetup(&graphicsPipeline)
		, m_passBloomTileIndexGen1(&graphicsPipeline, CComputeRenderPass::eFlags_ReflectConstantBuffersFromShader)
		, m_passBloomTileIndexGen2(&graphicsPipeline, CComputeRenderPass::eFlags_ReflectConstantBuffersFromShader)
		, m_pass1H(&graphicsPipeline, CComputeRenderPass::eFlags_ReflectConstantBuffersFromShader)
		, m_pass1V(&graphicsPipeline, CComputeRenderPass::eFlags_ReflectConstantBuffersFromShader)
		, m_pass2H(&graphicsPipeline, CComputeRenderPass::eFlags_ReflectConstantBuffersFromShader)
		, m_pass2V(&graphicsPipeline, CComputeRenderPass::eFlags_ReflectConstantBuffersFromShader)
	{
		m_passBloomTileIndexGen[0] = &m_passBloomTileIndexGen1;
		m_passBloomTileIndexGen[1] = &m_passBloomTileIndexGen2;

		m_passBloom[0][0] = &m_pass1H;
		m_passBloom[0][1] = &m_pass1V;
		m_passBloom[1][0] = &m_pass2H;
		m_passBloom[1][1] = &m_pass2V;
	}

	~CBloomSetupStage();

	bool IsStageActive(EShaderRenderingFlags flags) const final
	{
		//return CRenderer::CV_r_PostProcess;
		return false;
	}
	
	void Resize(int renderWidth, int renderHeight) final;

	void Execute(CTexture* pSrcRT, CTexture* pTiledBloomDestRT, CTexture* pTexBloomOutRT);

	void InitBuffer();
private:

	gpu::CTypedConstantBuffer<STypedBloomSetupConstants>  m_parameters;

	CComputeRenderPass m_passBloomSetup;

	CComputeRenderPass* m_passBloomTileIndexGen[2];
	CComputeRenderPass* m_passBloom[2][2];


	CGpuBuffer m_tileBloomMaskBuffer[2];
	CGpuBuffer m_tileIndexBuffer[2];
	CGpuBuffer m_dispatchIndirectCount[2];

	bool bBufferInit = false;
	int width = 0;
	int height = 0;

	Vec4 textureSize;

	Vec4i maskBufferSize[2];
	Vec2i dispatchSizeIndexGen[2];

	const uint32 groupSizeX = 8;
	const uint32 groupSizeY = 8;


	CComputeRenderPass m_passBloomTileIndexGen1;
	CComputeRenderPass m_passBloomTileIndexGen2;
	CComputeRenderPass m_pass1H;
	CComputeRenderPass m_pass1V;
	CComputeRenderPass m_pass2H;
	CComputeRenderPass m_pass2V;
};
