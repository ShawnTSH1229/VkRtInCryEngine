#include "LightMapRender.h"



void CLightMapRenderer::InitScene(std::vector<SAtlasBakeInformation>& atlasBakeInfomation, SBakerConfig bakerConfig)
{
	m_bakerConfig = bakerConfig;

	static const std::string resultTexName("$LightMapResult");
	for (uint32 index = 0; index < atlasBakeInfomation.size(); index++)
	{
		const std::string resultTexNameSub = resultTexName + std::to_string(index);
		SAtlasBakeInformation& atlasInfomation = atlasBakeInfomation[index];
		atlasInfomation.m_pResultTex = CTexture::GetOrCreateRenderTarget(resultTexNameSub.data(), m_bakerConfig.m_nUsedAtlasSize.x, m_bakerConfig.m_nUsedAtlasSize.y, ColorF(0.0, 0.0, 0.0, 0.0), ETEX_Type::eTT_2D, FT_DONT_RELEASE | FT_USAGE_UNORDERED_ACCESS, ETEX_Format::eTF_R32G32B32A32F);
	}
	
	m_pRayTracingCB = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(sizeof(Vec4));
	Vec4 lightDirection = Vec4(1.0, 1.0, 1.0, 1.0);
	m_pRayTracingCB->UpdateBuffer(&lightDirection, sizeof(Vec4));

	CreateAndBuildBLAS(atlasBakeInfomation);
	CreateAndBuildTLAS(atlasBakeInfomation);
}

void CLightMapRenderer::CreateAndBuildBLAS(const std::vector<SAtlasBakeInformation>& atlasBakeInfomation)
{
	for (uint32 indexAtlas = 0; indexAtlas < atlasBakeInfomation.size(); indexAtlas++)
	{
		const SAtlasBakeInformation& atlasInfo = atlasBakeInfomation[indexAtlas];
		for (uint32 indexGeo = 0; indexGeo < atlasInfo.m_atlasGeometries.size(); indexGeo++)
		{
			const SAtlasGeometry& atlasGeo = atlasInfo.m_atlasGeometries[indexGeo];
			SRayTracingBottomLevelASCreateInfo rtBottomLevelCreateInfo;
			rtBottomLevelCreateInfo.m_eBuildFlag = EBuildAccelerationStructureFlag::eBuild_PreferBuild;
			rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_sIndexStreaming = GetDeviceObjectFactory().CreateIndexStreamSet(&atlasGeo.m_indexStream);
			rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_nIndexBufferOffset = 0;

			SRayTracingGeometryTriangle rtGeometryTriangle;
			rtGeometryTriangle.m_sTriangVertexleInfo.m_sVertexStreaming = GetDeviceObjectFactory().CreateVertexStreamSet(1, &atlasGeo.m_posStream);
			rtGeometryTriangle.m_sTriangVertexleInfo.m_nMaxVertex = atlasGeo.m_nVertexCount;
			rtGeometryTriangle.m_sTriangVertexleInfo.m_hVertexPositionFormat = EDefaultInputLayouts::P3F;
			rtGeometryTriangle.m_sRangeInfo.m_nFirstVertex = 0;
			CRY_ASSERT(atlasGeo.m_nIndexCount % 3 == 0);
			rtGeometryTriangle.m_sRangeInfo.m_nPrimitiveCount = atlasGeo.m_nIndexCount / 3;
			rtGeometryTriangle.m_sRangeInfo.m_nPrimitiveOffset = 0;
			rtGeometryTriangle.bEnable = true;
			rtGeometryTriangle.bForceOpaque = true;
			rtBottomLevelCreateInfo.m_rtGeometryTriangles.push_back(rtGeometryTriangle);

			CRayTracingBottomLevelAccelerationStructurePtr pResBLAS = GetDeviceObjectFactory().CreateRayTracingBottomLevelAS(rtBottomLevelCreateInfo);
			m_pBLAS.push_back(pResBLAS);

			uint32 m_vbBindlessIndex = GetDeviceObjectFactory().SetBindlessStorageBuffer(rtGeometryTriangle.m_sTriangVertexleInfo.m_sVertexStreaming, 2);
			uint32 m_ibBindlessIndex = GetDeviceObjectFactory().SetBindlessStorageBuffer(rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_sIndexStreaming, 3);
			m_bindlessIndexArray.push_back(SBindlessIndex{ m_vbBindlessIndex ,m_ibBindlessIndex });
		}
	}

	CDeviceGraphicsCommandInterface* pCommandInterface = GetDeviceObjectFactory().GetCoreCommandList().GetGraphicsInterface();
	pCommandInterface->BuildRayTracingBottomLevelASs(m_pBLAS);
}

void CLightMapRenderer::CreateAndBuildTLAS(const std::vector<SAtlasBakeInformation>& atlasBakeInfomation)
{
	std::vector<SAccelerationStructureInstanceInfo> accelerationStructureInstanceInfos;

	uint32 nBLASIndex = 0;

	for (uint32 indexAtlas = 0; indexAtlas < atlasBakeInfomation.size(); indexAtlas++)
	{
		const SAtlasBakeInformation& atlasInfo = atlasBakeInfomation[indexAtlas];
		for (uint32 indexGeo = 0; indexGeo < atlasInfo.m_atlasGeometries.size(); indexGeo++)
		{
			const SAtlasGeometry& atlasGeo = atlasInfo.m_atlasGeometries[indexGeo];

			SAccelerationStructureInstanceInfo accelerationStructureInstanceInfo;
			Matrix34 aTransform;
			aTransform.SetRow4(0, atlasGeo.m_meshParam.m_worldTM.GetRow4(0));
			aTransform.SetRow4(1, atlasGeo.m_meshParam.m_worldTM.GetRow4(1));
			aTransform.SetRow4(2, atlasGeo.m_meshParam.m_worldTM.GetRow4(2));

			accelerationStructureInstanceInfo.m_transformPerInstance.push_back(SRayTracingInstanceTransform{aTransform});
			accelerationStructureInstanceInfo.m_nCustomIndex.push_back(0);
			accelerationStructureInstanceInfo.m_blas = m_pBLAS[nBLASIndex];
			accelerationStructureInstanceInfo.m_rayMask = 0xFF;
			accelerationStructureInstanceInfos.push_back(accelerationStructureInstanceInfo);
			nBLASIndex++;
		}
	}

	m_pBindlessIndexBuffer.Create(m_bindlessIndexArray.size(), sizeof(SBindlessIndex), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_SHADER_RESOURCE, m_bindlessIndexArray.data());


	SRayTracingTopLevelASCreateInfo rtTopLevelASCreateInfo;
	rtTopLevelASCreateInfo.m_nHitShaderNumPerTriangle = 1;
	rtTopLevelASCreateInfo.m_nMissShaderNum = 1;
	for (const auto& blas : accelerationStructureInstanceInfos)
	{
		rtTopLevelASCreateInfo.m_nInstanceTransform += blas.m_transformPerInstance.size();
		rtTopLevelASCreateInfo.m_nTotalGeometry += blas.m_blas->m_sRtBlASCreateInfo.m_rtGeometryTriangles.size();
		rtTopLevelASCreateInfo.m_nPreGeometryNumSum.push_back(rtTopLevelASCreateInfo.m_nTotalGeometry);
	}

	m_pRtTopLevelAS = GetDeviceObjectFactory().CreateRayTracingTopLevelAS(rtTopLevelASCreateInfo);

	std::vector<SAccelerationStructureInstanceData> accelerationStructureInstanceDatas;
	accelerationStructureInstanceDatas.resize(accelerationStructureInstanceInfos.size());
	for (uint32 index = 0; index < accelerationStructureInstanceInfos.size(); index++)
	{
		const SAccelerationStructureInstanceInfo& instanceInfo = accelerationStructureInstanceInfos[index];
		SAccelerationStructureInstanceData& instanceData = accelerationStructureInstanceDatas[index];
		for (uint32 transformIndex = 0; transformIndex < instanceInfo.m_transformPerInstance.size(); transformIndex++)
		{
			instanceData.m_aTransform = instanceInfo.m_transformPerInstance[transformIndex].m_aTransform;
			instanceData.m_nInstanceCustomIndex = instanceInfo.m_nCustomIndex[transformIndex];
			instanceData.m_nMask = instanceInfo.m_rayMask;

			instanceData.m_instanceShaderBindingTableRecordOffset = 0;
			instanceData.m_flags = 0x00000000;

			instanceData.accelerationStructureReference = instanceInfo.m_blas->GetAccelerationStructureAddress();
		}
	}

	m_instanceBuffer.Create(accelerationStructureInstanceDatas.size(), sizeof(SAccelerationStructureInstanceData), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::USAGE_ACCELERATION_STRUCTURE, accelerationStructureInstanceDatas.data());
	
	CDeviceGraphicsCommandInterface* pCommandInterface = GetDeviceObjectFactory().GetCoreCommandList().GetGraphicsInterface();
	pCommandInterface->BuildRayTracingTopLevelAS(m_pRtTopLevelAS, &m_instanceBuffer, 0);
}

void CLightMapRenderer::RenderLightMap(std::vector<SAtlasBakeInformation>& atlasBakeInfomation)
{
	if (m_bindlessRayTracingRenderPasses.size() < atlasBakeInfomation.size())
	{
		m_bindlessRayTracingRenderPasses.resize(atlasBakeInfomation.size());
	}

	for (uint32 index = 0; index < atlasBakeInfomation.size(); index++)
	{
		CRayTracingRenderPass& m_bindlessRayTracingRenderPass = m_bindlessRayTracingRenderPasses[index];
		m_bindlessRayTracingRenderPass.SetTechnique(CShaderMan::s_shLightMapRenderer, CCryNameTSCRC("LightMapRayTracing"), 0);
		m_bindlessRayTracingRenderPass.SetNeedBindless(true);
		m_bindlessRayTracingRenderPass.SetMaxPipelineRayRecursionDepth(m_bakerConfig.m_nMaxBounce);

		const SAtlasBakeInformation& atlasInfo = atlasBakeInfomation[index];
		CClearSurfacePass::Execute(atlasInfo.m_pResultTex, ColorF(0, 0, 0, 0));
		m_bindlessRayTracingRenderPass.SetConstantBuffer(0, m_pRayTracingCB);
		m_bindlessRayTracingRenderPass.SetBuffer(1, m_pRtTopLevelAS->GetAccelerationStructureBuffer());
		m_bindlessRayTracingRenderPass.SetTexture(2, atlasInfo.m_pPosTex);
		m_bindlessRayTracingRenderPass.SetOutputUAV(3, atlasInfo.m_pResultTex);
		m_bindlessRayTracingRenderPass.PrepareResourcesForUse(GetDeviceObjectFactory().GetCoreCommandList());
		m_bindlessRayTracingRenderPass.DispatchRayTracing(GetDeviceObjectFactory().GetCoreCommandList(), m_bakerConfig.m_nUsedAtlasSize.x, m_bakerConfig.m_nUsedAtlasSize.y);
	}

	GetDeviceObjectFactory().GetCoreCommandList().Reset();
}
