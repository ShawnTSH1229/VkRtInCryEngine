#include "VirtualShadowMap.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// vsm tile flag generation stage ////////////////////////////////////////////////////////////////////////

CTileFlagGenStage::~CTileFlagGenStage()
{
	m_vsmTileFlagBuffer.Release();
	m_tileFlagGenConstantBuffer.reset();
}

void CTileFlagGenStage::Init()
{
	if (!m_vsmTileFlagBuffer.IsAvailable())
	{
		m_vsmTileFlagBuffer.Create(VSM_VIRTUAL_TILE_SIZE_WH * VSM_VIRTUAL_TILE_SIZE_WH, sizeof(uint32), DXGI_FORMAT_R32_UINT, CDeviceObjectFactory::BIND_SHADER_RESOURCE | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
		m_vsmTileFlagBuffer.SetDebugName("m_vsmTileFlagBuffer");

		m_vsmGlobalInfo->m_vsmTileFlagBuffer = &m_vsmTileFlagBuffer;
	}

	m_tileFlagGenConstantBuffer = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(sizeof(CTileFlagGenParameter));
}

void CTileFlagGenStage::Update()
{
	m_tileFlagGenParameters.lightViewProj = m_vsmGlobalInfo->m_lightViewProjMatrix.GetTransposed();

	const SRenderViewInfo& viewInfo = m_vsmGlobalInfo->m_pRenderView->GetViewInfo(CCamera::eEye_Left);
	m_tileFlagGenParameters.invViewProj = viewInfo.invCameraProjMatrix.GetTransposed();

	m_texDeviceZWH = Vec2i(m_vsmGlobalInfo->m_texDeviceZ->GetWidth(), m_vsmGlobalInfo->m_texDeviceZ->GetHeight());
	m_tileFlagGenParameters.deviceZTexSize = Vec4(m_texDeviceZWH, 1.0f / m_texDeviceZWH.x, 1.0f / m_texDeviceZWH.y);

	m_tileFlagGenParameters.vsmVirtualTileSizeWH = Vec4i(VSM_VIRTUAL_TILE_SIZE_WH, VSM_VIRTUAL_TILE_SIZE_WH, 0, 0);
	m_tileFlagGenConstantBuffer->UpdateBuffer(&m_tileFlagGenParameters, sizeof(CTileFlagGenParameter), 0, 1);
}

void CTileFlagGenStage::Execute()
{
	PROFILE_LABEL_SCOPE("VSM_TILE_FLAG_GEN");

	CClearSurfacePass::Execute(&m_vsmTileFlagBuffer, ColorI(0, 0, 0, 0));

	Vec4i DispatchSize = Vec4i(divideRoundUp(m_texDeviceZWH, Vec2i(TILE_MASK_CS_GROUP_SIZE, TILE_MASK_CS_GROUP_SIZE)), 0, 0);

	//m_compPass->SetTechnique(CShaderMan::s_ShaderVSM, CCryNameTSCRC("VSMTileFlagGen"), 0);
	m_compPass->SetOutputUAV(0, &m_vsmTileFlagBuffer);
	m_compPass->SetTexture(0, m_vsmGlobalInfo->m_texDeviceZ);
	m_compPass->SetConstantBuffer(0, m_tileFlagGenConstantBuffer);
	m_compPass->SetDispatchSize(DispatchSize.x, DispatchSize.y, 1);

	CDeviceBuffer* uavs[1] = { m_vsmTileFlagBuffer.GetDevBuffer()};

	const CDeviceCommandListRef coreCmdList = GetDeviceObjectFactory().GetCoreCommandList();
	coreCmdList.GetGraphicsInterface()->PrepareUAVsForUse(1, uavs, true);
	m_compPass->PrepareResourcesForUse(coreCmdList);

	const bool bAsynchronousCompute = false;
	SScopedComputeCommandList computeCommandList(bAsynchronousCompute);
	m_compPass->Execute(computeCommandList);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// vsm tile table generation stage ////////////////////////////////////////////////////////////////////////

void CTileTableGenStage::Init()
{
	if (!m_vsmTileTableBuffer.IsAvailable())
	{
		m_vsmTileTableBuffer.Create(VSM_VIRTUAL_TILE_SIZE_WH * VSM_VIRTUAL_TILE_SIZE_WH, sizeof(uint32), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS | CDeviceObjectFactory::BIND_SHADER_RESOURCE, NULL);
		m_vsmTileTableBuffer.SetDebugName("m_vsmTileTableBuffer");
		m_vsmGlobalInfo->m_vsmTileTableBuffer = &m_vsmTileTableBuffer;
	}

	if (!m_vsmValidTileCountBuffer.IsAvailable())
	{
		m_vsmValidTileCountBuffer.Create(1, sizeof(uint32), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
		m_vsmValidTileCountBuffer.SetDebugName("m_vsmValidTileCountBuffer");
	}

	m_tileTableGenConstantBuffer = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(sizeof(CTileTableGenParameter));
}

void CTileTableGenStage::Update()
{
	m_tileTableGenParameters.vsmVirtualTileSizeWH = Vec4i(VSM_VIRTUAL_TILE_SIZE_WH, VSM_VIRTUAL_TILE_SIZE_WH, 0, 0);
	m_tileTableGenParameters.vsmPhyTileSizeWH = Vec4i(VSM_PHYSICAL_TILE_SIZE_WH, VSM_PHYSICAL_TILE_SIZE_WH, 0, 0);
	m_tileTableGenConstantBuffer->UpdateBuffer(&m_tileTableGenParameters, sizeof(CTileTableGenParameter), 0, 1);
}

void CTileTableGenStage::Execute()
{
	PROFILE_LABEL_SCOPE("VSM_TILE_TABLE_GEN");

	Vec4i DispatchSize = Vec4i(divideRoundUp(Vec2i(VSM_VIRTUAL_TILE_SIZE_WH, VSM_VIRTUAL_TILE_SIZE_WH), Vec2i(TILE_TABLE_GEN_CS_GROUP_SIZE, TILE_TABLE_GEN_CS_GROUP_SIZE)), 0, 0);


	//tile table generation
	{
		//m_compPass->SetTechnique(CShaderMan::s_ShaderVSMTileTableGen, CCryNameTSCRC("VSMTileTableGen"), 0);
		m_compPass->SetOutputUAV(0, m_vsmGlobalInfo->m_vsmTileFlagBuffer);
		m_compPass->SetOutputUAV(1, &m_vsmTileTableBuffer);
		m_compPass->SetOutputUAV(2, &m_vsmValidTileCountBuffer);
		m_compPass->SetConstantBuffer(0, m_tileTableGenConstantBuffer);
		m_compPass->SetDispatchSize(DispatchSize.x, DispatchSize.y, 1);

		CDeviceBuffer* uavs[3] = { m_vsmGlobalInfo->m_vsmTileFlagBuffer->GetDevBuffer() ,m_vsmTileTableBuffer.GetDevBuffer() ,m_vsmValidTileCountBuffer.GetDevBuffer() };

		const CDeviceCommandListRef coreCmdList = GetDeviceObjectFactory().GetCoreCommandList();
		coreCmdList.GetGraphicsInterface()->PrepareUAVsForUse(3, uavs, true);
		m_compPass->PrepareResourcesForUse(coreCmdList);

		const bool bAsynchronousCompute = false;
		SScopedComputeCommandList computeCommandList(bAsynchronousCompute);

		m_compPass->Execute(computeCommandList);
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// vsm shadow cmd build stage////////////////////////////////////////////////////////////////////////

void CShadowCmdBuildStage::Init()
{
	if (!m_culledCmdCountBuffer.IsAvailable())
	{
		m_culledCmdCountBuffer.Create(1, sizeof(uint32), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
		m_culledCmdCountBuffer.SetDebugName("m_culledCmdCountBuffer");
		m_vsmGlobalInfo->m_culledCmdCountBuffer = &m_culledCmdCountBuffer;
	}
	m_cmdBuildConstantBuffer = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(sizeof(SCmdBuildPara));
}

void CShadowCmdBuildStage::Update()
{

}

void CShadowCmdBuildStage::Execute()
{
	//Creating CompiledRenderObjects should happen after Update() call of the GraphicsPipeline, as it requires access to initialized Render Targets
	bool cullDataValid = false;


	CRenderView* RenderView = m_vsmGlobalInfo->m_pRenderView;
	RenderView->PrepareVSMShadowView();
	CRenerItemGPUDrawer& gpuDrawer = RenderView->GetGPUDrawer();
	{
		std::vector<SShadowFrustumToRender*>& frustumsToRender = RenderView->GetShadowFrustumsByType(CRenderView::eShadowFrustumRenderType_VSM);
		if (frustumsToRender.size() > 0)
		{
			SShadowFrustumToRender*& frustumToRender = frustumsToRender[0];
			auto* pShadowView = reinterpret_cast<CRenderView*>(frustumToRender->pShadowsView.get());
			const RenderItems& renderItems = pShadowView->GetRenderItems(ERenderListID(0));/*shadow view only has one render item list*/

			if (renderItems.size() > 0)
			{
				gpuDrawer.SetPassContext(TTYPE_SHADOWGEN/*todo :vsm type shadow gen*/, 0, eStage_VSM, 0/*pass id*/);
				gpuDrawer.UpdateGPURenderItems(&renderItems, 0, renderItems.size() - 1);
				cullDataValid = true;
			}
		}
	}

	{
		for (uint32 indexX = 0; indexX < VSM_VIRTUAL_TILE_SIZE_WH; indexX++)
		{
			for (uint32 indexY = 0; indexY < VSM_VIRTUAL_TILE_SIZE_WH; indexY++)
			{
				Matrix44A lightViewProjMatrix;
				SShadowProjectMatrix ShadowProjectMatrix{ lightViewProjMatrix ,indexX,indexY };
				m_vsmFrustumProjectBuffer[indexX + indexY * VSM_VIRTUAL_TILE_SIZE_WH]->UpdateBuffer(&ShadowProjectMatrix,sizeof(SShadowProjectMatrix));
			}
		}
	}

	if (cullDataValid)
	{
		//init culled cmd buffer and counter offset
		{
			//m_draw.inputBuffer = m_memoryAllocator.createBuffer(totalSize, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, aid);
			if (!m_culledCmdBuffer.IsAvailable())
			{
				m_culledCmdBuffer.Create(CRenerItemGPUDrawer::m_maxDrawSize, gpuDrawer.m_gpuDrawCmdDataSize, DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
				m_culledCmdBuffer.SetDebugName("m_culledCmdBuffer");
			}
	
			m_vsmGlobalInfo->m_culledCmdBuffer = &m_culledCmdBuffer;
		}
	
		{
			SCmdBuildPara cmdBuildPara{ m_vsmGlobalInfo->m_lightViewProjMatrix,Vec4i(gpuDrawer.GetDrawCount(),VSM_VIRTUAL_TILE_SIZE_WH,CRenerItemGPUDrawer::m_maxDrawSize,0) };
			m_cmdBuildConstantBuffer->UpdateBuffer(&cmdBuildPara, sizeof(SCmdBuildPara));
		}
	
	
		//m_compPass->SetTechnique(CShaderMan::s_ShaderVSMCmdBuild, CCryNameTSCRC("VSMCmdBuild"), 0);
	
		m_compPass->SetOutputUAV(0, &m_culledCmdBuffer);
		m_compPass->SetOutputUAV(1, &m_culledCmdCountBuffer);
	
		m_compPass->SetOutputUAV(2, gpuDrawer.GetUnCulledCmdBuffer());
		m_compPass->SetOutputUAV(3, gpuDrawer.GetGpuCullDataBuffer());
		m_compPass->SetOutputUAV(4, m_vsmGlobalInfo->m_vsmTileTableBuffer);

		m_compPass->SetConstantBuffer(0, m_cmdBuildConstantBuffer);
	
		int dispatchSizex = gpuDrawer.GetDrawCount() / 128 + (gpuDrawer.GetDrawCount() % 128 > 0 ? 1 : 0);
		m_compPass->SetDispatchSize(dispatchSizex, 1, 1);
	
		CDeviceBuffer* uavs[2] = { m_culledCmdBuffer.GetDevBuffer(),m_culledCmdCountBuffer.GetDevBuffer() };
	
		const CDeviceCommandListRef coreCmdList = GetDeviceObjectFactory().GetCoreCommandList();
		coreCmdList.GetGraphicsInterface()->PrepareUAVsForUse(2, uavs, true);
		m_compPass->PrepareResourcesForUse(coreCmdList);
	
		const bool bAsynchronousCompute = false;
		SScopedComputeCommandList computeCommandList(bAsynchronousCompute);
	
		m_compPass->Execute(computeCommandList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// vsm projection stage -- shadow project pass ////////////////////////////////////////////////////////////////////////

void CShadowProjectStage::CVSMShadowProjectPass::Init(CShadowProjectStage* Stage)
{
	m_perPassResources = &Stage->m_perPassResources;
	m_pPerPassResourceSet = GetDeviceObjectFactory().CreateResourceSet(CDeviceResourceSet::EFlags_ForceSetAllState);
}

bool CShadowProjectStage::CVSMShadowProjectPass::PrepareResources(const CRenderView* pMainView)
{
	m_pPerPassResourceSet->Update(*m_perPassResources);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// vsm projection stage ////////////////////////////////////////////////////////////////////////

void CShadowProjectStage::Init()
{
	std::string name = "$RT_ShadowPool" + m_graphicsPipeline.GetUniqueIdentifierName();
	m_pShadowDepthRT = CTexture::GetOrCreateTextureObject(name.c_str(), 0, 0, 1, eTT_2D, FT_DONT_STREAM | FT_USAGE_DEPTHSTENCIL, eTF_Unknown);
	m_pShadowDepthRT->Invalidate(PHYSICAL_VSM_TEX_SIZE, PHYSICAL_VSM_TEX_SIZE, eTF_D32F);
	m_pShadowDepthRT->CreateDepthStencil(eTF_D32F, ColorF(Clr_FarPlane.r, 5.f, 0.f, 0.f));

	SDeviceResourceLayoutDesc layoutDesc;
	m_perPassResources.SetTexture(0, CRendererResources::s_pTexNULL, EDefaultResourceViews::UnorderedAccess, EShaderStage_All);//Physical VSM Shadow Depth Texture
	m_perPassResources.SetBuffer(0, CDeviceBufferManager::GetNullBufferStructured(), EDefaultResourceViews::UnorderedAccess, EShaderStage_All);//PageTable
	layoutDesc.SetResourceSet(EResourceLayoutSlot_PerPassRS, m_perPassResources);
	layoutDesc.AddPushConstant(EShaderStage_Vertex | EShaderStage_Pixel, sizeof(uint64) * 2, 0);
	m_pResourceLayout = GetDeviceObjectFactory().CreateResourceLayout(layoutDesc);//see m_graphicsPipeline.CreateScenePassLayout(m_perPassResources);
	m_perPassResources.AcceptChangedBindPoints();


	SDeviceResourceIndirectLayoutDesc deviceResourceIndirectLayoutDesc;
	deviceResourceIndirectLayoutDesc.AddShaderGroupToken();
	deviceResourceIndirectLayoutDesc.AddPushConstant(m_pResourceLayout, EShaderStage_Vertex | EShaderStage_Pixel, 0, sizeof(uint64) * 2);
	deviceResourceIndirectLayoutDesc.AddIBToken();
	deviceResourceIndirectLayoutDesc.AddVBToken();
	deviceResourceIndirectLayoutDesc.AddDrawIndexed();
	m_pResourceIndirectLayout = GetDeviceObjectFactory().CreateResourceIndirectLayout(deviceResourceIndirectLayoutDesc);

	m_pRenderPass.Init(this);

	
}

void CShadowProjectStage::Update()
{
	PrepareShadowPasses();

	CRenerItemGPUDrawer& gpuDrawer = m_vsmGlobalInfo->m_pRenderView->GetGPUDrawer();



	// compute input buffer space requirements

	if ((!m_pIndirectGraphicsPSO->IsValid() || gpuDrawer.IsPSOGroupChanged()) && gpuDrawer.GetRenderItemPSO().size() > 0)
	{
		CDeviceGraphicsPSODesc indirectPsoDesc = GetDeviceObjectFactory().GetGraphicsPsoDescByHash(gpuDrawer.GetRenderItemPSO()[0]->m_psoDescHash);
		indirectPsoDesc.indirectPso = gpuDrawer.GetRenderItemPSO();
		m_pIndirectGraphicsPSO = GetDeviceObjectFactory().CreateGraphicsPSO(indirectPsoDesc);
	}

	//if (!m_preprocessBuffer.IsAvailable())
	{
		m_preprocessBuffer.CreatePreprocessBuffer(CRenerItemGPUDrawer::m_maxDrawSize, m_pResourceIndirectLayout, m_pIndirectGraphicsPSO);
	}
	
}

void CShadowProjectStage::Execute()
{
	//CRenderView* pRenderView = m_vsmGlobalInfo->m_pRenderView;
	//CRenerItemGPUDrawer& renderItemGPUDrawer = pRenderView->GetGPUDrawer();

	m_pRenderPass.BeginExecution(m_graphicsPipeline);
	//m_vsmShadowProjectPass.SetupDrawContext(StageID, curPass.m_eShadowPassID, TTYPE_SHADOWGEN, 0);
	//m_vsmShadowProjectPass.DrawRenderItems(pShadowsView, (ERenderListID)curPass.m_nShadowFrustumSide);
	//m_vsmShadowProjectPass.EndExecution();

	//CRenderItemDrawer& rendItemDrawer = pRenderView->GetDrawer();
	//rendItemDrawer.InitDrawSubmission();
	//rendItemDrawer.JobifyDrawSubmission();
	//rendItemDrawer.WaitForDrawSubmission(); disable multi thread

			//TODO: ShadowView
	//CRenderView* pShadowView = nullptr;
	//CDeviceCommandListRef  commandList = GetDeviceObjectFactory().GetCoreCommandList();
	//CDeviceGraphicsCommandInterface& commandInterface = *(commandList.GetGraphicsInterface());
	//const RenderItems& renderItems = pShadowView->GetRenderItems(ERenderListID(0));
	//commandInterface.ExecuteGeneratedCommands(m_pResourceIndirectLayout, m_pIndirectGraphicsPSO, renderItems.size(), &m_culledCmdBuffer, &m_preprocessBuffer);
}


void CShadowProjectStage::PrepareShadowPasses()
{
	m_pRenderPass.SetRenderTargets(m_pShadowDepthRT, nullptr);
	D3DViewPort viewport;// = { float(arrViewport[0]), float(arrViewport[1]), float(arrViewport[2]), float(arrViewport[3]), 0, 1 };
	m_pRenderPass.SetViewport(viewport);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// virtual shadow map stage ////////////////////////////////////////////////////////////////////////

void CVirtualShadowMapStage::Init()
{
	CRenderView* pRenderView = RenderView();
	m_vsmGlobalInfo.m_pRenderView = pRenderView;
	if (m_gloablEnableVSM)
	{
		m_tileFlagGenStage.Init();
		m_tileTableGenStage.Init();
		m_shadowCmdBuildStage.Init();
		//m_vsmShadowProjectStage.Init();
	}

}

void CVirtualShadowMapStage::Update()
{
	if (m_gloablEnableVSM)
	{
		ShadowViewUpdate();
		m_tileFlagGenStage.Update();
		m_tileTableGenStage.Update();
		m_shadowCmdBuildStage.Update();
		//m_vsmShadowProjectStage.Update();
	}
}

void CVirtualShadowMapStage::PrePareShadowMap()
{
	//Physical VSM Shadow Depth Texture RW Texture2D Temp!!!
}

void CVirtualShadowMapStage::Execute()
{
	if (m_vsmGlobalInfo.m_frustumValid)
	{
		m_tileFlagGenStage.Execute();
		m_tileTableGenStage.Execute();
		m_shadowCmdBuildStage.Execute();
		//m_vsmShadowProjectStage.Execute();
	}

	VisualizeBuffer();

	return;
}

bool CShadowProjectStage::CreatePipelineStateInner(const SGraphicsPipelineStateDescription& description, CDeviceGraphicsPSOPtr& outPSO)
{
	outPSO = NULL;

	CShader* pShader = static_cast<CShader*>(description.shaderItem.m_pShader);
	SShaderTechnique* pTechnique = pShader->GetTechnique(description.shaderItem.m_nTechnique, description.technique, true);
	if (!pTechnique)
		return true;

	CShaderResources* pRes = static_cast<CShaderResources*>(description.shaderItem.m_pShaderResources);
	if (pRes->m_ResFlags & MTL_FLAG_NOSHADOW)
		return true;

	SShaderPass* pShaderPass = &pTechnique->m_Passes[0];
	uint64 objectFlags = description.objectFlags;

	CDeviceGraphicsPSODesc psoDesc(m_pResourceLayout, description);
	psoDesc.m_bDynamicDepthBias = true;

	// Handle quality flags
	CGraphicsPipeline::ApplyShaderQuality(psoDesc, gcpRendD3D->GetShaderProfile(pShader->m_eShaderType));

	// Set resource states
	bool bTwoSided = false;
	if (pRes->m_ResFlags & MTL_FLAG_2SIDED)
		bTwoSided = true;

	// Set Cull mode
	psoDesc.m_bAllowTesselation = false;
	psoDesc.m_CullMode = bTwoSided ? eCULL_None : ((pShaderPass && pShaderPass->m_eCull != -1) ? (ECull)pShaderPass->m_eCull : eCULL_Back);
	if (pShader->m_eSHDType == eSHDT_Terrain)
	{
		psoDesc.m_CullMode = eCULL_Front; //front faces culling by default for terrain
	}
	if (!bTwoSided && psoDesc.m_CullMode == eCULL_Front)
		psoDesc.m_CullMode = eCULL_Back;

	//Set Shader Flags RT
	psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_NO_TESSELLATION];
	psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_HW_PCF_COMPARE];
	psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_SAMPLE0];
	psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_SAMPLE4];
	if (pRes->IsAlphaTested())
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_ALPHATEST];
	if (objectFlags & FOB_DECAL_TEXGEN_2D)
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_DECAL_TEXGEN_2D];
	if ((objectFlags & FOB_BLEND_WITH_TERRAIN_COLOR))
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_BLEND_WITH_TERRAIN_COLOR];
	if (!(objectFlags & FOB_TRANS_MASK))
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_OBJ_IDENTITY];
	if (objectFlags & FOB_NEAREST)
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_NEAREST];
	if (objectFlags & FOB_DISSOLVE)
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_DISSOLVE];
	if (psoDesc.m_RenderState & GS_ALPHATEST)
		psoDesc.m_ShaderFlags_RT |= g_HWSR_MaskBit[HWSR_ALPHATEST];


	//Set VertexModifier
	psoDesc.m_ShaderFlags_MDV |= pShader->m_nMDV;
	if (objectFlags & FOB_OWNER_GEOMETRY)
		psoDesc.m_ShaderFlags_MDV &= ~MDV_DEPTH_OFFSET;
	if (objectFlags & FOB_BENDED)
		psoDesc.m_ShaderFlags_MDV |= MDV_BENDING;
	if (pRes->m_Textures[EFTT_DIFFUSE] && pRes->m_Textures[EFTT_DIFFUSE]->m_Ext.m_pTexModifier)
		psoDesc.m_ShaderFlags_MD |= pRes->m_Textures[EFTT_DIFFUSE]->m_Ext.m_nUpdateFlags;
	if (pRes->m_pDeformInfo)
		psoDesc.m_ShaderFlags_MDV |= EVertexModifier(pRes->m_pDeformInfo->m_eType);

	//Set Primitive Type
	if (psoDesc.m_bAllowTesselation && (psoDesc.m_PrimitiveType < ept1ControlPointPatchList || psoDesc.m_PrimitiveType > ept4ControlPointPatchList))
	{
		psoDesc.m_PrimitiveType = ept3ControlPointPatchList;
		psoDesc.m_ObjectStreamMask |= VSM_NORMALS;
	}

	// rendertarget and depth stencil format
	psoDesc.m_pRenderPass = m_pRenderPass.GetRenderPass();

	outPSO = GetDeviceObjectFactory().CreateGraphicsPSO(psoDesc);
	return outPSO != nullptr;
}


bool CVirtualShadowMapStage::CreatePipelineStates(DevicePipelineStatesArray* pStateArray, const SGraphicsPipelineStateDescription& stateDesc, CGraphicsPipelineStateLocalCache* pStateCache)
{

	return true;//TEMP!

	DevicePipelineStatesArray& stageStates = pStateArray[eStage_VSM];

	if (pStateCache->Find(stateDesc, stageStates))
		return true;

	bool bFullyCompiled = m_vsmShadowProjectStage.CreatePipelineStateInner(stateDesc, stageStates[0]);
	if (bFullyCompiled)
	{
		pStateCache->Put(stateDesc, stageStates);
	}

	return bFullyCompiled;
}

void CVirtualShadowMapStage::ShadowViewUpdate()
{
	Matrix44A viewMatrix, projMatrix;
	CRenderView* pRenderView = RenderView();
	m_vsmGlobalInfo.m_pRenderView = pRenderView;
	m_vsmGlobalInfo.m_texDeviceZ = pRenderView->GetDepthTarget();

	SShadowFrustumToRender* pVSMFrustum = nullptr;
	for (auto& fr : pRenderView->m_shadows.m_renderFrustums)
	{
		if (fr.pFrustum->m_eFrustumType == ShadowMapFrustum::e_VSM)
		{
			pVSMFrustum = &fr;
			break;
		}
	}

	if (!pVSMFrustum)
	{
		m_vsmGlobalInfo.m_frustumValid = false;
		return;
	}

	//m_light.m_Origin = passInfo.GetCamera().GetPosition() + GetSunDir()
	//vLightSrcRelPos = m_light.m_Origin - passInfo.GetCamera().GetPosition() = GetSunDir()

	f32 aabbRadiusCam = VSM_SHADOW_PROJ_WH;
	f32 w = aabbRadiusCam;
	f32 h = w;
	f32 zn = 0.1f;
	f32 zf = 10000.0f;

	mathMatrixOrtho(&projMatrix, w, h, zn, zf);

	const Vec3 zAxis(0.f, 0.f, 1.f);
	const Vec3 yAxis(0.f, 1.f, 0.f);
	Vec3 up;
	Vec3 camPos = pRenderView->GetCamera(CCamera::eEye_Left).GetPosition();
	Vec3 at = camPos;
	//Vec3 vLightDir = -pVSMFrustum->pFrustum->vLightSrcRelPos;
	Vec3 vLightDir = pVSMFrustum->pFrustum->vLightSrcRelPos;
	vLightDir.Normalize();

	//Vec3 eye = at - pVSMFrustum->pFrustum->vLightSrcRelPos.len() * vLightDir;
	Vec3 eye = at + pVSMFrustum->pFrustum->vLightSrcRelPos.len() * vLightDir;

	if (fabsf(vLightDir.Dot(zAxis)) > 0.9995f)
		up = yAxis;
	else
		up = zAxis;

	mathMatrixLookAt(&viewMatrix, eye, at, up);

	m_vsmGlobalInfo.m_lightViewProjMatrix = viewMatrix * projMatrix;
	m_vsmGlobalInfo.m_frustumValid = true;

	for (auto& fr : pRenderView->m_shadows.m_renderFrustums)
	{
		auto pShadowView = reinterpret_cast<CRenderView*>(fr.pShadowsView.get());
		pShadowView->SwitchUsageMode(IRenderView::eUsageModeReading);
	}
}

void CVirtualShadowMapStage::VisualizeBuffer()
{
	PROFILE_LABEL_SCOPE("VSM_VISUALIZE_BUFFER");

	if (m_tileTableGenStage.GetVsmTileTableBuffer()->IsAvailable())
	{
		if (m_passBufferVisualize.IsDirty())
		{
			m_passBufferVisualize.SetPrimitiveFlags(CRenderPrimitive::eFlags_ReflectShaderConstants_PS);
			m_passBufferVisualize.SetPrimitiveType(CRenderPrimitive::ePrim_ProceduralTriangle);
			//m_passBufferVisualize.SetTechnique(CShaderMan::s_ShaderVSM, CCryNameTSCRC("VSMVisualizeBuffer"), 0);
			m_passBufferVisualize.SetRenderTarget(0, m_graphicsPipelineResources.m_pTexVSMVisualize);
			m_passBufferVisualize.SetState(GS_NODEPTHTEST | GS_NOCOLMASK_A);
			m_passBufferVisualize.SetBuffer(0, m_tileFlagGenStage.GetVsmTileFlagBuffer());
			//m_passBufferVisualize.SetOutputUAV(0, m_tileTableGenStage.GetVsmTileTableBuffer());
		}

		m_passBufferVisualize.BeginConstantUpdate();
		m_passBufferVisualize.SetConstant(CCryNameR("visualizeBufferSize"), Vec4(VSM_VIRTUAL_TILE_SIZE_WH, VSM_VIRTUAL_TILE_SIZE_WH, 1.0, 1.0), eHWSC_Pixel);
		m_passBufferVisualize.Execute();
	}

}


