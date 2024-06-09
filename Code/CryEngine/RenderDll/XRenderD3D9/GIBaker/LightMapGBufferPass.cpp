#include "LightMapGBufferPass.h"
#include "../GraphicsPipeline/Common/GraphicsPipeline.h"

void CLightMapGBufferGenerator::Init(uint32 constantBufferNum, SBakerConfig bakerConfig, std::vector<SAtlasBakeInformation>& atlasBakeInfomation)
{
	for (uint32 index = 0; index < constantBufferNum; index++)
	{
		CConstantBufferPtr m_pPerObjCB = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(sizeof(SMeshParam));
		constantBuffers.push_back(m_pPerObjCB);
	}

	static const std::string posTexName("$LightMapGBufferPos");
	static const std::string faceNormalTexName("$LightMapGBufferFaceNormal");
	static const std::string shadingNormalTexName("$LightMapGBufferShadingNormal");

	for (uint32 index = 0; index < atlasBakeInfomation.size(); index++)
	{
		const std::string posTexNameSub = posTexName + std::to_string(index);
		const std::string faceNormalTexNameSub = faceNormalTexName + std::to_string(index);
		const std::string shadingNormalTexNameSub = shadingNormalTexName + std::to_string(index);

		SAtlasBakeInformation& atlasBakeInformation = atlasBakeInfomation[index];
		atlasBakeInformation.m_pPosTex = CTexture::GetOrCreateRenderTarget(posTexNameSub.data(), m_bakerConfig.m_nUsedAtlasSize.x, m_bakerConfig.m_nUsedAtlasSize.y, ColorF(0.0, 0.0, 0.0, 0.0), ETEX_Type::eTT_2D, FT_DONT_RELEASE, ETEX_Format::eTF_R32G32B32A32F);
		atlasBakeInformation.m_pFaceNormalTex = CTexture::GetOrCreateRenderTarget(faceNormalTexNameSub.data(), m_bakerConfig.m_nUsedAtlasSize.x, m_bakerConfig.m_nUsedAtlasSize.y, ColorF(0.0, 0.0, 0.0, 0.0), ETEX_Type::eTT_2D, FT_DONT_RELEASE, ETEX_Format::eTF_R32G32B32A32F);
		atlasBakeInformation.m_pShadingNormalTex = CTexture::GetOrCreateRenderTarget(shadingNormalTexNameSub.data(), m_bakerConfig.m_nUsedAtlasSize.x, m_bakerConfig.m_nUsedAtlasSize.y, ColorF(0.0, 0.0, 0.0, 0.0), ETEX_Type::eTT_2D, FT_DONT_RELEASE, ETEX_Format::eTF_R32G32B32A32F);
	}

	m_bakerConfig = bakerConfig;
}

void CLightMapGBufferGenerator::ReleaseResource()
{
	bPsoGenerated = false;
	m_lightMapGBufferPass.Reset();
}

void CLightMapGBufferGenerator::GenerateGraphicsPSO()
{
	static CCryNameTSCRC tLightMapGBufferGen("LightMapGBufferGenTech");
	CShader* sLightMapGBuffer = CShaderMan::s_shLightMapGBuffer;

	SDeviceResourceLayoutDesc resourceLayoutDesc;
	resourceLayoutDesc.SetConstantBuffer(0, (EConstantBufferShaderSlot)0, EShaderStage_AllWithoutCompute);
	m_pResourceLayout = GetDeviceObjectFactory().CreateResourceLayout(resourceLayoutDesc);

	CDeviceGraphicsPSODesc psoDesc;
	psoDesc.m_pResourceLayout = m_pResourceLayout;
	psoDesc.m_pShader = sLightMapGBuffer;
	psoDesc.m_technique = tLightMapGBufferGen;
	psoDesc.m_ShaderFlags_RT = 0;
	psoDesc.m_ShaderFlags_MD = 0;
	psoDesc.m_ShaderFlags_MDV = MDV_NONE;
	psoDesc.m_PrimitiveType = ERenderPrimitiveType::eptTriangleList;
	
	//NORMAL: from object
	//POSITION3: from shader
	//LIGHTMAPUV: from shader
	psoDesc.m_VertexFormat = EDefaultInputLayouts::N3F;

	psoDesc.m_RenderState = 0;
	psoDesc.m_StencilState = STENC_FUNC(FSS_STENCFUNC_ALWAYS) | STENCOP_FAIL(FSS_STENCOP_KEEP) | STENCOP_ZFAIL(FSS_STENCOP_KEEP) | STENCOP_PASS(FSS_STENCOP_KEEP);
	psoDesc.m_StencilReadMask = 0xFF;
	psoDesc.m_StencilWriteMask = 0xFF;
	psoDesc.m_CullMode = eCULL_None;
	psoDesc.m_bDepthClip = false; //??
	psoDesc.m_pRenderPass = m_lightMapGBufferPass.GetRenderPass();
	psoDesc.m_RenderState = GS_NODEPTHTEST;

	m_graphicsPSO = GetDeviceObjectFactory().CreateGraphicsPSO(psoDesc);
	bPsoGenerated = true;
}

void CLightMapGBufferGenerator::GenerateLightMapGBuffer(std::vector<SAtlasBakeInformation>& atlasBakeInfomation)
{
	uint32 totalPrimitvie = 0;
	for (uint32 index = 0; index < atlasBakeInfomation.size(); index++)
	{
		totalPrimitvie += atlasBakeInfomation[index].m_atlasGeometries.size();
	}

	primitives.clear();
	primitives.resize(totalPrimitvie);

	uint32 primitvieIndex = 0;

	for (uint32 index = 0; index < atlasBakeInfomation.size(); index++)
	{
		D3DViewPort viewport = { 0.0, 0.0, static_cast<float>(m_bakerConfig.m_nUsedAtlasSize.x), static_cast<float>(m_bakerConfig.m_nUsedAtlasSize.y),0.0, 1.0 };
		
		SAtlasBakeInformation& atlasBakeInformation = atlasBakeInfomation[index];

		m_lightMapGBufferPass.SetRenderTarget(0, atlasBakeInformation.m_pPosTex);
		m_lightMapGBufferPass.SetRenderTarget(1, atlasBakeInformation.m_pFaceNormalTex);
		m_lightMapGBufferPass.SetRenderTarget(2, atlasBakeInformation.m_pShadingNormalTex);
		m_lightMapGBufferPass.SetTargetClearMask(CPrimitiveRenderPass::eClear_Color0);
		m_lightMapGBufferPass.SetViewport(viewport);
		m_lightMapGBufferPass.BeginAddingPrimitives();

		if (!bPsoGenerated)
		{
			GenerateGraphicsPSO();
		}

		for (uint32 geoIndex = 0; geoIndex < atlasBakeInfomation[index].m_atlasGeometries.size(); geoIndex++)
		{
			SAtlasGeometry& atlasGeometry = atlasBakeInfomation[index].m_atlasGeometries[geoIndex];

			std::vector<SStreamInfo> vertexStreamingInfos;
			vertexStreamingInfos.push_back(atlasGeometry.m_posStream);
			vertexStreamingInfos.push_back(atlasGeometry.m_normStream);
			vertexStreamingInfos.push_back(atlasGeometry.m_2uStream);
			
			SCompiledRenderPrimitive& primitive = primitives[primitvieIndex];
			primitive.m_pResourceLayout = m_pResourceLayout;
			primitive.m_pVertexInputSet = GetDeviceObjectFactory().CreateVertexStreamSet(3, vertexStreamingInfos.data());
			primitive.m_pIndexInputSet = GetDeviceObjectFactory().CreateIndexStreamSet(&atlasGeometry.m_indexStream);

			primitive.m_nMaxStreamingSlot = EStreamIDs::VSF_LIGHTMAPUV;;
			primitive.m_nStreamingCount = 3;

			primitive.m_stencilRef = 0;
			primitive.m_pPipelineState = m_graphicsPSO;
			primitive.m_pResources = nullptr;

			constantBuffers[primitvieIndex]->UpdateBuffer(&atlasGeometry.m_meshParam,sizeof(SMeshParam),0,1);
			primitive.m_inlineConstantBuffers[0].shaderSlot = (EConstantBufferShaderSlot)0;
			primitive.m_inlineConstantBuffers[0].shaderStages = EShaderStage_Vertex | EShaderStage_Pixel;
			primitive.m_inlineConstantBuffers[0].pBuffer = constantBuffers[primitvieIndex];

			primitive.m_drawInfo.vertexBaseOffset = 0;
			primitive.m_drawInfo.instanceCount = 1;
			primitive.m_drawInfo.vertexOrIndexOffset = 0;
			primitive.m_drawInfo.vertexOrIndexCount = atlasGeometry.m_nIndexCount;

			m_lightMapGBufferPass.AddPrimitive(&primitive);
			primitvieIndex++;
		}
		m_lightMapGBufferPass.Execute();
	}
	GetDeviceObjectFactory().GetCoreCommandList().Reset();
}




