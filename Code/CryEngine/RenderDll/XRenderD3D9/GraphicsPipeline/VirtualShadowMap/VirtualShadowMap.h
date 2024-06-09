//TanGram
#pragma once
#include "../Common/GraphicsPipelineStage.h"
#include "../Common/FullscreenPass.h"
#include "../Common/ComputeRenderPass.h"
#include "../Common/GraphicsPipeline.h"

#define PHYSICAL_VSM_TEX_SIZE 2048
#define VIRTUAL_VSM_TEX_SIZE 8192

#define VSM_TILE_SIZE 256

#define VSM_PHYSICAL_TILE_SIZE_WH (PHYSICAL_VSM_TEX_SIZE	/ VSM_TILE_SIZE)//8 
#define VSM_VIRTUAL_TILE_SIZE_WH	(VIRTUAL_VSM_TEX_SIZE	/ VSM_TILE_SIZE)//32

#define TILE_MASK_CS_GROUP_SIZE 16
#define TILE_TABLE_GEN_CS_GROUP_SIZE 16

#define VSM_SHADOW_PROJ_WH 512

struct SShadowProjectMatrix
{
	Matrix44A m_lightViewProjMatrix;
	uint indexX;
	uint indexY;
	uint unused0;
	uint unused1;
};

class CVSMGlobalInfo
{
public:
	CVSMGlobalInfo()
		:m_frustumValid(false)
	{

	}



	bool m_frustumValid;
	CRenderView* m_pRenderView;
	

	//used in tile flag generation pass
	CTexture* m_texDeviceZ;
	Matrix44A m_lightViewProjMatrix;

	//used in tile table generation pass
	CGpuBuffer* m_vsmTileFlagBuffer;

	CGpuBuffer* m_vsmTileTableBuffer;

	CGpuBuffer* m_culledCmdBuffer;

	CGpuBuffer* m_culledCmdCountBuffer;
};

class CTileFlagGenStage
{
public:
	CTileFlagGenStage(CVSMGlobalInfo* vsmGlobalInfo, CComputeRenderPass* compRenderPass)
		:m_vsmGlobalInfo(vsmGlobalInfo),
		m_compPass(compRenderPass){};

	~CTileFlagGenStage();

	struct CTileFlagGenParameter
	{
		Matrix44 invViewProj;
		Matrix44 lightViewProj;
		Vec4 deviceZTexSize;
		Vec4i vsmVirtualTileSizeWH;
	};

	void Init();
	void Update();
	void Execute();

	//visualize
	CGpuBuffer* GetVsmTileFlagBuffer()
	{
		return &m_vsmTileFlagBuffer;
	}

private:
	CComputeRenderPass* m_compPass;
	CVSMGlobalInfo* m_vsmGlobalInfo;

	//buffers
	CGpuBuffer m_vsmTileFlagBuffer;
	CConstantBufferPtr  m_tileFlagGenConstantBuffer;

	//parameters
	Vec2i m_texDeviceZWH;
	CTileFlagGenParameter m_tileFlagGenParameters;
};


class CTileTableGenStage
{
public:
	CTileTableGenStage(CVSMGlobalInfo* vsmGlobalInfo, CComputeRenderPass* compRenderPass)
		:
		m_vsmGlobalInfo(vsmGlobalInfo),
		m_compPass(compRenderPass)
	{};

	void Init();
	void Update();
	void Execute();

	struct CTileTableGenParameter
	{
		Vec4i vsmVirtualTileSizeWH;
		Vec4i vsmPhyTileSizeWH;
	};

	//visualize
	CGpuBuffer* GetVsmTileTableBuffer()
	{
		return &m_vsmTileTableBuffer;
	}
private:
	CComputeRenderPass* m_compPass;
	CVSMGlobalInfo* m_vsmGlobalInfo;

	//buffers
	CGpuBuffer m_vsmTileTableBuffer;
	CGpuBuffer m_vsmValidTileCountBuffer;
	CConstantBufferPtr  m_tileTableGenConstantBuffer;

	//parameters
	CTileTableGenParameter m_tileTableGenParameters;
};

class CShadowCmdBuildStage
{
public:
	CShadowCmdBuildStage(CVSMGlobalInfo* vsmGlobalInfo, CComputeRenderPass* compRenderPass)
		:m_vsmGlobalInfo(vsmGlobalInfo),
		m_compPass(compRenderPass) {};

	void Init();
	void Update();
	void Execute();

	struct SCmdBuildPara
	{
		Matrix44 lightViewProj;
		Vec4i cmdBuildPara;
	};

private:
	CComputeRenderPass* m_compPass;
	CVSMGlobalInfo* m_vsmGlobalInfo;

	CConstantBufferPtr  m_cmdBuildConstantBuffer;
	CGpuBuffer m_culledCmdBuffer;

	CGpuBuffer m_culledCmdCountBuffer;

	CConstantBufferPtr m_vsmFrustumProjectBuffer[VSM_VIRTUAL_TILE_SIZE_WH * VSM_VIRTUAL_TILE_SIZE_WH];
};

class CShadowProjectStage
{
public:
	CShadowProjectStage(CVSMGlobalInfo* vsmGlobalInfo, CGraphicsPipeline& graphicsPipeline)
		: m_vsmGlobalInfo(vsmGlobalInfo)
		, m_graphicsPipeline(graphicsPipeline)
		, m_pShadowDepthRT(nullptr)
	{

	}

	void Init();
	void Update();
	void Execute();

	void PrepareShadowPasses();
	bool CreatePipelineStateInner(const SGraphicsPipelineStateDescription& description, CDeviceGraphicsPSOPtr& outPSO);

	class CVSMShadowProjectPass : public CSceneRenderPass
	{
	public:
		void Init(CShadowProjectStage* Stage);
		bool PrepareResources(const CRenderView* pMainView);

		CDeviceResourceSetDesc*   m_perPassResources;
	};

private:
	CVSMShadowProjectPass m_pRenderPass;
	CVSMGlobalInfo* m_vsmGlobalInfo;
	CGraphicsPipeline& m_graphicsPipeline;

	CDeviceResourceLayoutPtr m_pResourceLayout;
	CDeviceResourceSetDesc   m_perPassResources;

	CDeviceResourceIndirectLayoutPtr m_pResourceIndirectLayout;

	CDeviceGraphicsPSOPtr m_pIndirectGraphicsPSO;

	_smart_ptr<CTexture> m_pShadowDepthRT;

	CGpuBuffer m_preprocessBuffer;
	
};

class CVirtualShadowMapStage : public CGraphicsPipelineStage
{
public:
	static const EGraphicsPipelineStage StageID = eStage_VSM;

	CVirtualShadowMapStage(CGraphicsPipeline& graphicsPipeline)
		: CGraphicsPipelineStage(graphicsPipeline)
		, m_passVSMTileFlagGen(&graphicsPipeline)
		, m_passVSMTileTableGen(&graphicsPipeline)
		, m_passVSMCmdBuild(&graphicsPipeline)
		, m_passBufferVisualize(&graphicsPipeline)
		, m_vsmShadowProjectStage(&m_vsmGlobalInfo, graphicsPipeline)
		, m_tileFlagGenStage(&m_vsmGlobalInfo, &m_passVSMTileFlagGen)
		, m_tileTableGenStage(&m_vsmGlobalInfo, &m_passVSMTileTableGen)
		, m_shadowCmdBuildStage(&m_vsmGlobalInfo, &m_passVSMCmdBuild)
	{
		
	}

	static constexpr bool m_gloablEnableVSM = false;

	bool IsStageActive(EShaderRenderingFlags flags) const final
	{
		return false;
		//return CRenderer::CV_r_VirtualShadowMap;
	}

	void  Init()   final;
	void  Update() final;
	void Execute();
	
	void ShadowViewUpdate();

	void PrePareShadowMap();
	void VisualizeBuffer();

private:

	CVSMGlobalInfo m_vsmGlobalInfo;

	//tile flag generation stage
	CComputeRenderPass	m_passVSMTileFlagGen;
	CTileFlagGenStage m_tileFlagGenStage;

	//tile table generation 
	CComputeRenderPass m_passVSMTileTableGen;
	CTileTableGenStage m_tileTableGenStage;

	//shadow cmd build stage
	CComputeRenderPass m_passVSMCmdBuild;
	CShadowCmdBuildStage m_shadowCmdBuildStage;

	_smart_ptr<CTexture>	m_pShadowDepthTarget;

	CFullscreenPass m_passBufferVisualize;
	CTexture* m_pVisualizeTarget;

	//shadow project stage
public:

	bool CreatePipelineStates(DevicePipelineStatesArray* pStateArray, const SGraphicsPipelineStateDescription& stateDesc, CGraphicsPipelineStateLocalCache* pStateCache);

	CShadowProjectStage m_vsmShadowProjectStage;
	

};