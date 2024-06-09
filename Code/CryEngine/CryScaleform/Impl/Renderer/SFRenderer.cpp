// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "Renderer/SFRenderer.h"
#include <Kernel/SF_Debug.h>
#include <Kernel/SF_Random.h>
#include <Render/Render_BufferGeneric.h>
#include <Kernel/SF_HeapNew.h>
#include <Render/Render_TextureCacheGeneric.h>

namespace Scaleform {
namespace Render {

const uint64 CSFRenderer::s_depthStencilStates[DepthStencil_Count] =
{
	GS_NODEPTHTEST | GS_DEPTHWRITE, // Used to invalidate these states at during BeginFrame.
	GS_NODEPTHTEST,                 // All depth and stencil testing/writing is disabled.

	// Used for writing stencil values (depth operations disabled, color write enable off)
	GS_STENCIL | GS_NODEPTHTEST, // Clears all stencil values to the stencil reference.
	GS_STENCIL | GS_NODEPTHTEST, // Clears all stencil values lower than the reference, to the reference.
	GS_STENCIL | GS_NODEPTHTEST, // Increments any stencil values that match the reference.

	// Used for testing stencil values (depth operations disabled).
	GS_STENCIL | GS_NODEPTHTEST, // Allows rendering where the stencil value is less than or equal to the reference.
	GS_STENCIL | GS_NODEPTHTEST, // Allows rendering where the stencil value is greater than the reference.

	// Used for writing depth values (stencil operations disabled, color write enable off).
	GS_NODEPTHTEST | GS_DEPTHWRITE, // Depth is written (but not tested), stenciling is disabled.

	// Used for testing depth values (stencil operations disabled).
	GS_DEPTHFUNC_EQUAL,    // Allows rendering where the depth values are equal, stenciling and depth writing are disabled.
	GS_DEPTHFUNC_NOTEQUAL, // Allows rendering where the depth values are not equal, stenciling and depth writing are disabled.
};

const int CSFRenderer::s_depthStencilFunctions[DepthStencil_Count] =
{
	0, // Used to invalidate these states at during BeginFrame.
	0, // All depth and stencil testing/writing is disabled.

	// Used for writing stencil values (depth operations disabled, color write enable off)
	STENC_FUNC(FSS_STENCFUNC_ALWAYS) | STENCOP_PASS(FSS_STENCOP_REPLACE) | STENCOP_FAIL(FSS_STENCOP_REPLACE), // Clears all stencil values to the stencil reference.
	STENC_FUNC(FSS_STENCFUNC_LEQUAL) | STENCOP_PASS(FSS_STENCOP_REPLACE) | STENCOP_FAIL(FSS_STENCOP_KEEP),    // Clears all stencil values lower than the reference, to the reference.
	STENC_FUNC(FSS_STENCFUNC_EQUAL) | STENCOP_PASS(FSS_STENCOP_INCR) | STENCOP_FAIL(FSS_STENCOP_KEEP),        // Increments any stencil values that match the reference.

	// Used for testing stencil values (depth operations disabled).
	STENC_FUNC(FSS_STENCFUNC_LEQUAL) | STENCOP_PASS(FSS_STENCOP_KEEP) | STENCOP_FAIL(FSS_STENCOP_KEEP),      // Allows rendering where the stencil value is less than or equal to the reference.
	STENC_FUNC(FSS_STENCFUNC_GREATER) | STENCOP_PASS(FSS_STENCOP_KEEP) | STENCOP_FAIL(FSS_STENCOP_KEEP),     // Allows rendering where the stencil value is greater than the reference.

	// Used for writing depth values (stencil operations disabled, color write enable off).
	0,    // Depth is written (but not tested), stenciling is disabled.

	// Used for testing depth values (stencil operations disabled).
	0, // Allows rendering where the depth values are equal, stenciling and depth writing are disabled.
	0, // Allows rendering where the depth values are not equal, stenciling and depth writing are disabled.
};

CSFRenderer::CSFRenderer(ThreadCommandQueue* pCommandQueue)
	: Render::ShaderHAL<CSFTechniqueManager, CSFTechniqueInterface>(pCommandQueue)
	, m_pRenderPass(nullptr)
	, m_meshCache(Scaleform::Memory::GetGlobalHeap(), MeshCacheParams::PC_Defaults)
	, m_pEmptyConstants(nullptr)
{
	memset(&m_rs, 0, sizeof(SSFRenderState));
	m_indexStream.nStride = sizeof(IndexType) == 2 ? Index16 : Index32;
}

CSFRenderer::~CSFRenderer()
{
	ShutdownHAL();
}

CConstantBuffer* CSFRenderer::CreateConstantBuffer(const float* pData, const buffer_size_t m_size)
{
	CConstantBuffer* pConstantBuffer = m_pRenderCache->GetConstantBuffer();
	if (pConstantBuffer)
	{
		pConstantBuffer->UpdateBuffer(pData, m_size);
	}
	return pConstantBuffer;
}

RenderTarget* CSFRenderer::CreateRenderTarget(CTexture* pColor, CTexture* pDepth)
{
	if (!checkState(HS_Initialized, __FUNCTION__))
	{
		return nullptr;
	}

	ImageSize colorBufferSize(pColor->GetWidth(), pColor->GetHeight());
	GPtr<RenderTarget> ptarget = pRenderBufferManager->CreateRenderTarget(colorBufferSize, RBuffer_User, Image_R8G8B8A8, 0);
	GPtr<DepthStencilBuffer> pdsb = 0;
	if (pDepth)
	{
		ImageSize depthBufferSize(pDepth->GetWidth(), pDepth->GetHeight());
		pdsb = *SF_HEAP_AUTO_NEW(this) DepthStencilBuffer(0, depthBufferSize);
	}

	CSFRenderTargets::SetTextures(ptarget, pColor, pdsb, pDepth);
	return ptarget;
}

RenderTarget* CSFRenderer::CreateRenderTarget(Render::Texture* texture, bool needsStencil)
{
	if (!checkState(HS_Initialized, __FUNCTION__))
	{
		return nullptr;
	}
	CSFTexture* pt = (CSFTexture*)texture;
	if (!pt || pt->TextureCount != 1)
	{
		return nullptr;
	}

	RenderTarget* prt = pRenderBufferManager->CreateRenderTarget(texture->GetSize(), RBuffer_Texture, texture->GetFormat(), texture);
	if (!prt)
	{
		return nullptr;
	}

	CTexture* prtView = pt->m_pTextures[0].pTexture;
	CTexture* pdsView = nullptr;
	GPtr<DepthStencilBuffer> pdsb;

	if (needsStencil)
	{
		pdsb = *pRenderBufferManager->CreateDepthStencilBuffer(texture->GetSize(), false);
		if (pdsb)
		{
			CSFDepthStencilTexture* surf = (CSFDepthStencilTexture*)pdsb->GetSurface();
			if (surf)
			{
				pdsView = surf->pTexture;
			}
		}
	}
	CSFRenderTargets::SetTextures(prt, prtView, pdsb, pdsView);
	return prt;
}

RenderTarget* CSFRenderer::CreateTempRenderTarget(const ImageSize& size, bool needsStencil)
{
	if (!checkState(HS_Initialized, __FUNCTION__))
	{
		return nullptr;
	}

	RenderTarget* prt = pRenderBufferManager->CreateTempRenderTarget(size);
	if (!prt)
	{
		return nullptr;
	}

	CSFRenderTargets* phd = (CSFRenderTargets*)prt->GetRenderTargetData();
	if (phd && (!needsStencil || phd->pDepthStencilBuffer))
	{
		return prt;
	}

	CSFTexture* pt = (CSFTexture*)prt->GetTexture();
	CRY_ASSERT(pt->TextureCount == 1);
	CTexture* prtView = pt->m_pTextures[0].pTexture;
	CTexture* pdsView = 0;
	GPtr<DepthStencilBuffer> pdsb = 0;

	if (needsStencil)
	{
		pdsb = *pRenderBufferManager->CreateDepthStencilBuffer(pt->GetSize());
		if (pdsb)
		{
			CSFDepthStencilTexture* surf = (CSFDepthStencilTexture*)pdsb->GetSurface();
			if (surf)
			{
				pdsView = surf->pTexture;
			}
		}
	}

	CSFRenderTargets::SetTextures(prt, prtView, pdsb, pdsView);
	return prt;
}

bool CSFRenderer::InitHAL(const Render::HALInitParams& params)
{
	GPtr<Render::MemoryManager> pMemoryManager = params.pMemoryManager;

	if (params.pTextureManager)
	{
		m_pTextureManager = static_cast<CSFTextureManager*>(params.pTextureManager.GetPtr());
	}
	else
	{
		m_pTextureManager = *SF_HEAP_AUTO_NEW(this) CSFTextureManager(this, params.RenderThreadId, pRTCommandQueue, 0, pMemoryManager);
	}

	if (params.pRenderBufferManager)
	{
		pRenderBufferManager = params.pRenderBufferManager;
	}
	else
	{
		pRenderBufferManager = *SF_HEAP_AUTO_NEW(this) RenderBufferManagerGeneric(RBGenericImpl::DSSM_Exact);
	}

	if (!CreateBlendStates() ||
	    !m_meshCache.Initialize() ||
	    !pRenderBufferManager->Initialize(m_pTextureManager))
	{
		HALState |= HS_Initialized;
		ShutdownHAL();
		return false;
	}

	const buffer_size_t size = Scaleform::Render::Uniform::SU_VertexSize* sizeof(float);
	m_pEmptyConstants = static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_DevBufMan.CreateConstantBuffer(size);
	bool result = BaseHAL::InitHAL(params);
	SetStereoImpl(GPtr<StereoImplBase>(*SF_HEAP_AUTO_NEW(this) StereoImplBase));
	return result;
}

bool CSFRenderer::ShutdownHAL()
{
	if (!IsInitialized())
	{
		return true;
	}

	if (!BaseHAL::ShutdownHAL())
	{
		return false;
	}

	pRenderBufferManager.Clear();

	if (m_pTextureManager)
	{
		m_pTextureManager->Reset();
	}

	m_pTextureManager.Clear();
	SManager.Reset();
	m_meshCache.Reset();
	return true;
}

bool CSFRenderer::RestoreAfterReset()
{
	if (!BaseHAL::RestoreAfterReset())
	{
		return false;
	}

	if (0 == RenderTargetStack.GetSize() && !createDefaultRenderBuffer())
	{
		return false;
	}
	return true;
}

bool CSFRenderer::Submit()
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());
	if (m_pRenderPass)
	{
		if (m_pRenderPass->GetPrimitiveCount())
		{
			GetDeviceObjectFactory().GetCoreCommandList().Reset();
			m_pRenderPass->Execute();
		}
		m_pRenderPass = nullptr;
	}
	return true;
}

bool CSFRenderer::BeginScene()
{
	if (!BaseHAL::BeginScene())
	{
		return false;
	}

	ScopedRenderEvent GPUEvent(GetEvents(), Event_BeginScene, "HAL::BeginScene-SetState");
	return true;
}

bool CSFRenderer::BeginFrame()
{
	if (!BaseHAL::BeginFrame())
	{
		return false;
	}

	CRY_ASSERT(!m_pRenderPass);
	return true;
}

void CSFRenderer::EndFrame()
{
	Submit();

	BaseHAL::EndFrame();
}

bool CSFRenderer::IsRasterModeSupported(RasterModeType mode) const
{
	return mode != CSFRenderer::RasterMode_Point;
}

void CSFRenderer::MapVertexFormat(PrimitiveFillType fill, const VertexFormat* sourceFormat, const VertexFormat** single, const VertexFormat** batch, const VertexFormat** instanced, unsigned /*= 0*/)
{
	SManager.MapVertexFormat(fill, sourceFormat, single, batch, instanced);
}

void CSFRenderer::updateViewport()
{
	Submit();

	Rect<int> vpRect;

	if (HALState & HS_ViewValid)
	{
		float dx = ViewRect.x1 - VP.Left;
		float dy = ViewRect.y1 - VP.Top;

		calcHWViewMatrix(0, &Matrices->View2D, ViewRect, dx, dy);
		setUserMatrix(Matrices->User);
		Matrices->ViewRect = ViewRect;
		Matrices->UVPOChanged = 1;

		if (!(HALState & HS_InRenderTarget))
		{
			vpRect = ViewRect;
		}
		else
		{
			vpRect.SetRect(VP.Left, VP.Top, VP.Left + VP.Width, VP.Top + VP.Height);
		}
	}

	m_viewPort.TopLeftX = (FLOAT)vpRect.x1;
	m_viewPort.TopLeftY = (FLOAT)vpRect.y1;
	m_viewPort.Width = (FLOAT)vpRect.Width();
	m_viewPort.Height = (FLOAT)vpRect.Height();
	m_viewPort.Width = Alg::Max<FLOAT>(m_viewPort.Width, 1);
	m_viewPort.Height = Alg::Max<FLOAT>(m_viewPort.Height, 1);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
}

void CSFRenderer::SetSamplerState(uint32 stage, uint32 viewCount, CTexture** views, SamplerStateHandle state)
{
	for (uint32 i = 0; i < viewCount; ++i)
	{
		CTexture* pTexture = views[i];
		m_rs.resourceSet.textures[stage + i] = CTexture::IsTextureExist(pTexture) ? pTexture : CRendererResources::s_ptexNoTexture;
		m_rs.resourceSet.samplers[stage + i] = state;
	}
}

void CSFRenderer::setRenderTargetImpl(Render::RenderTargetData* phdinput, unsigned flags, const Color& clearColor)
{
	Submit();

	CSFRenderTargets* phd = reinterpret_cast<CSFRenderTargets*>(phdinput);
	CTexture* clearViews[CSFTextureManager::maxTextureSlots];
	void* nullAddresses[CSFTextureManager::maxTextureSlots];
	memset(clearViews, 0, sizeof(clearViews));
	memset(nullAddresses, 0, sizeof(nullAddresses));
	SetSamplerState(0, CSFTextureManager::maxTextureSlots, clearViews, EDefaultSamplerStates::PointClamp);

	if (phd->pRenderTargetTexture && !(flags & PRT_NoClear))
	{
		float clear[4];
		clearColor.GetRGBAFloat(clear);

		CClearSurfacePass& clearPassRT = m_pRenderCache->GetClearPass();
		clearPassRT.Execute(phd->pRenderTargetTexture, ColorF(clear[0], clear[1], clear[2], clear[3]));
	}

	m_pRenderTarget = phd->pRenderTargetTexture;
	m_pDepthStencil = phd->pDepthStencilTexture;
	m_rs.colorFormat = m_pRenderTarget ? m_pRenderTarget->GetSrcFormat() : eTF_Unknown;
	m_rs.depthFormat = m_pDepthStencil ? m_pDepthStencil->GetSrcFormat() : eTF_Unknown;
	++AccumulatedStats.RTChanges;
}

bool CSFRenderer::checkDepthStencilBufferCaps()
{
	if (0 == RenderTargetStack.GetSize())
	{
		return false;
	}

	RenderTargetEntry& rte = RenderTargetStack.Back();
	if (!rte.StencilChecked)
	{
		rte.StencilAvailable = false;
		rte.DepthBufferAvailable = false;

		const CTexture* pDepthStencil = m_pDepthStencil;
		if (pDepthStencil)
		{
			switch (pDepthStencil->GetTextureSrcFormat())
			{
			case eTF_D24S8:
			case eTF_D32FS8:
			case eTF_D16S8:
				rte.StencilAvailable = true;
				rte.MultiBitStencil = true;
				rte.DepthBufferAvailable = true;
				break;

			case eTF_D32F:
			case eTF_D24:
			case eTF_S8:
			case eTF_D16:
				rte.DepthBufferAvailable = true;
				break;

			default:
				SF_DEBUG_ASSERT1(0, "Unexpected DepthStencil surface format: 0x%08x", pDepthStencil->GetTextureSrcFormat());
				break;
			}
		}
		rte.StencilChecked = true;
	}

	return rte.StencilAvailable || rte.DepthBufferAvailable;
}

void CSFRenderer::applyDepthStencilMode(DepthStencilMode mode, unsigned stencilRef)
{
	ScopedRenderEvent GPUEvent(GetEvents(), Event_ApplyDepthStencil, "HAL::applyDepthStencilMode");
	if (mode != CurrentDepthStencilState || CurrentStencilRef != stencilRef)
	{
		CurrentStencilRef = stencilRef;

		if (DepthStencilModeTable[mode].ColorWriteEnable != DepthStencilModeTable[CurrentDepthStencilState].ColorWriteEnable)
		{
			CurrentDepthStencilState = mode;
			applyBlendModeImpl(CurrentBlendState.Mode, CurrentBlendState.SourceAc, CurrentBlendState.ForceAc);
		}
		CurrentDepthStencilState = mode;
	}
}

void CSFRenderer::applyRasterModeImpl(RasterModeType mode)
{
	m_rs.rasterState = RasterMode_Wireframe == mode ? GS_WIREFRAME : 0;
}

void CSFRenderer::applyBlendModeImpl(BlendMode mode, bool sourceAc, bool forceAc)
{
	SF_UNUSED(forceAc);
	bool colorWrite = EnableIgnore_On == DepthStencilModeTable[CurrentDepthStencilState].ColorWriteEnable;
	int32 blendType = GetBlendType(mode, colorWrite, sourceAc);
	m_rs.blendState = blendType;
}

void CSFRenderer::setBatchUnitSquareVertexStream()
{
	m_vertexStream.hStream = m_meshCache.m_quadVertexBuffer;
	m_indexStream.hStream = ~0u;
	m_rs.vertexOffset = 0;
	m_rs.vertexStride = sizeof(VertexXY16fAlpha);
}

UPInt CSFRenderer::setVertexArray(PrimitiveBatch* pbatch, MeshCacheItem* pmeshBase)
{
	CSFMesh* pmesh = static_cast<CSFMesh*>(pmeshBase);
	m_vertexStream.hStream = pmesh->m_pVertexBuffer;
	m_indexStream.hStream = pmesh->m_pIndexBuffer;
	m_rs.vertexStride = pbatch->pFormat->Size;
	m_rs.vertexOffset = 0;
	return 0;
}

UPInt CSFRenderer::setVertexArray(const ComplexMesh::FillRecord& fr, unsigned formatIndex, MeshCacheItem* pmeshBase)
{
	CSFMesh* pmesh = static_cast<CSFMesh*>(pmeshBase);
	m_vertexStream.hStream = pmesh->m_pVertexBuffer;
	m_indexStream.hStream = pmesh->m_pIndexBuffer;
	const Render::VertexFormat* pFormat = fr.pFormats[formatIndex];
	m_rs.vertexStride = pFormat->Size;

	// TODO: m_rs.vertexOffset = fr.VertexByteOffset;
	// Unfortunately our engine doesn't support "vertex byte" offset, only "vertex index" offset
	// We can convert "vertex byte" offset to our "vertex index" offset as m_rs.vertexOffset = ffr.VertexByteOffset / m_rs.vertexStride
	// Unfortunately this works only in case that the whole mesh has the same vertex format, but this is not always satisfied
	// e.g. SF4 mesh has part of the mesh with Position vertex information only, while rest of the mesh contains Position and Color information
	// and therefore the stride is not constant within the same mesh buffer
	m_rs.vertexOffset = 0;
	return 0;
}

void CSFRenderer::drawPrimitive(unsigned indexCount, unsigned meshCount)
{
	Render(0, 0, indexCount, 1);

	SF_UNUSED(meshCount);
#if !defined(SF_BUILD_SHIPPING)
	AccumulatedStats.Meshes += meshCount;
	AccumulatedStats.Triangles += indexCount / 3;
	AccumulatedStats.Primitives++;
#endif
}

void CSFRenderer::drawIndexedPrimitive(unsigned indexCount, unsigned vertexCount, unsigned meshCount, UPInt indexPtr, UPInt vertexOffset)
{
	Render(m_rs.vertexOffset, indexPtr, indexCount, 1);

	SF_UNUSED3(meshCount, vertexCount, vertexOffset);
#if !defined(SF_BUILD_SHIPPING)
	AccumulatedStats.Meshes += meshCount;
	AccumulatedStats.Triangles += indexCount / 3;
	AccumulatedStats.Primitives++;
#endif
}

void CSFRenderer::drawIndexedInstanced(unsigned indexCount, unsigned vertexCount, unsigned meshCount, UPInt indexPtr, UPInt vertexOffset)
{
	Render(m_rs.vertexOffset, indexPtr, indexCount, meshCount);

	SF_UNUSED3(meshCount, vertexCount, vertexOffset);
#if !defined(SF_BUILD_SHIPPING)
	AccumulatedStats.Meshes += meshCount;
	AccumulatedStats.Triangles += (indexCount / 3) * meshCount;
	AccumulatedStats.Primitives++;
#endif
}

void CSFRenderer::Render(unsigned vertexBase, unsigned vertexOrIndexOffset, unsigned vertexOrIndexCount, unsigned instanceCount)
{
	CRY_ASSERT(CurrentStencilRef < 256);
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	CSFRenderCache::SPSOCacheEntry psoCacheEntry;
	psoCacheEntry.rp.colorFormat = m_rs.colorFormat;
	psoCacheEntry.rp.depthFormat = m_rs.depthFormat;

	if (!m_pRenderPass)
	{
		m_pRenderPass = &m_pRenderCache->GetRenderPass(psoCacheEntry.rp);
		m_pRenderPass->SetRenderTarget(0, m_pRenderTarget);
		m_pRenderPass->SetDepthTarget(m_pDepthStencil);
		m_pRenderPass->SetViewport(m_viewPort);
		m_pRenderPass->BeginAddingPrimitives();
	}

	const SSFTechnique& technique = ShaderData.GetCurrentShaders();
	InputLayoutHandle vertexLayout = (static_cast<const CSFVertexLayout*>(technique.pVFormat->pSysFormat.GetPtr()))->hVertexLayout;
	m_vertexStream.nStride = technique.pVFormat->Size;

	psoCacheEntry.shaderType = technique.shaderType;
	psoCacheEntry.renderState = m_rs.rasterState | s_depthStencilStates[CurrentDepthStencilState] | m_blendStates[m_rs.blendState];
	psoCacheEntry.stencilState = s_depthStencilFunctions[CurrentDepthStencilState];

	for (int32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
	{
		psoCacheEntry.samplers[i] = m_rs.resourceSet.samplers[i];
	}

	SCompiledRenderPrimitive& compiledPrimitive = m_pRenderCache->GetCompiledPrimitive();
	compiledPrimitive.m_pVertexInputSet = GetDeviceObjectFactory().CreateVertexStreamSet(1, &m_vertexStream);
	compiledPrimitive.m_pIndexInputSet = GetDeviceObjectFactory().CreateIndexStreamSet(&m_indexStream);
	compiledPrimitive.m_pResources = m_pRenderCache->GetResourceSet(m_rs.resourceSet);
	compiledPrimitive.m_pPipelineState = m_pRenderCache->GetPSO(psoCacheEntry, *m_pRenderPass, vertexLayout, compiledPrimitive.m_pResourceLayout);

	compiledPrimitive.m_inlineConstantBuffers[0].pBuffer = m_rs.pConstantBuffer ? m_rs.pConstantBuffer : m_pEmptyConstants.get();
	compiledPrimitive.m_inlineConstantBuffers[1].pBuffer = m_rs.pConstantBuffer ? m_rs.pConstantBuffer : m_pEmptyConstants.get();

	compiledPrimitive.m_stencilRef = CurrentStencilRef;
	compiledPrimitive.m_drawInfo.vertexBaseOffset = vertexBase;
	compiledPrimitive.m_drawInfo.vertexOrIndexOffset = vertexOrIndexOffset;
	compiledPrimitive.m_drawInfo.vertexOrIndexCount = vertexOrIndexCount;
	compiledPrimitive.m_drawInfo.instanceCount = instanceCount;

	if (compiledPrimitive.m_pPipelineState && compiledPrimitive.m_pPipelineState->IsValid())
	{
		m_pRenderPass->AddPrimitive(&compiledPrimitive);
	}
}

void CSFRenderer::drawUncachedFilter(const FilterStackEntry& e)
{
	if (!e.pPrimitive || !e.pRenderTarget)
	{
		return;
	}
	BaseHAL::drawUncachedFilter(e);
}

int32 CSFRenderer::GetBlendType(BlendMode blendMode, bool colorWrite, bool sourceAc)
{
	int32 base = 0;
	if (!colorWrite)
	{
		base = Blend_Count * 2;
		blendMode = Blend_None;
	}
	else if (sourceAc)
	{
		base += Blend_Count;
	}
	return base + (int32)blendMode;
}

bool CSFRenderer::CreateBlendStates()
{
	const uint64 s_cryBlendTable[Blend_Count] =
	{
		GS_BLEND(BS_SRC_ALPHA,     BS_INV_SRC_ALPHA, BS_OP_ADD,     BS_ONE,       BS_INV_SRC_ALPHA, BS_OP_ADD), // None
		GS_BLEND(BS_SRC_ALPHA,     BS_INV_SRC_ALPHA, BS_OP_ADD,     BS_ONE,       BS_INV_SRC_ALPHA, BS_OP_ADD), // Normal
		GS_BLEND(BS_SRC_ALPHA,     BS_INV_SRC_ALPHA, BS_OP_ADD,     BS_SRC_ALPHA, BS_INV_SRC_ALPHA, BS_OP_ADD), // Layer
		GS_BLEND(BS_DST_COLOR,     BS_INV_SRC_ALPHA, BS_OP_ADD,     BS_ONE,       BS_INV_SRC_ALPHA, BS_OP_ADD), // Multiply
		GS_BLEND(BS_INV_DST_COLOR, BS_ONE,           BS_OP_ADD,     BS_ONE,       BS_INV_SRC_ALPHA, BS_OP_ADD), // Screen
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ONE,       BS_ZERO, BS_OP_ADD),          // Lighten (Same as overwrite all)
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ONE,       BS_ZERO, BS_OP_ADD),          // Darken (Same as overwrite all)
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ONE,       BS_ZERO, BS_OP_ADD),          // Difference (Same as overwrite all)
		GS_BLEND(BS_SRC_ALPHA,     BS_ONE,           BS_OP_ADD,     BS_ZERO,      BS_ONE, BS_OP_ADD),           // Add
		GS_BLEND(BS_SRC_ALPHA,     BS_ONE,           BS_OP_INV_SUB, BS_ZERO,      BS_INV_SRC_ALPHA, BS_OP_ADD), // Subtract
		GS_BLEND(BS_INV_DST_COLOR, BS_INV_SRC_ALPHA, BS_OP_ADD,     BS_SRC_ALPHA, BS_INV_SRC_ALPHA, BS_OP_ADD), // Invert
		GS_BLEND(BS_ZERO,          BS_ONE,           BS_OP_ADD,     BS_ZERO,      BS_SRC_ALPHA, BS_OP_ADD),     // Alpha
		GS_BLEND(BS_ZERO,          BS_ONE,           BS_OP_ADD,     BS_ZERO,      BS_INV_SRC_ALPHA, BS_OP_ADD), // Erase
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ONE,       BS_ZERO, BS_OP_ADD),          // Overlay (Same as overwrite all)
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ONE,       BS_ZERO, BS_OP_ADD),          // Hardlight (Same as overwrite all)
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ZERO,      BS_ONE, BS_OP_ADD),           // Overwrite - overwrite the destination.
		GS_BLEND(BS_ONE,           BS_ZERO,          BS_OP_ADD,     BS_ONE,       BS_ZERO, BS_OP_ADD),          // OverwriteAll - overwrite the destination (including alpha).
		GS_BLEND(BS_ONE,           BS_ONE,           BS_OP_ADD,     BS_ONE,       BS_ONE, BS_OP_ADD),           // FullAdditive - add all components together, without multiplication.
		GS_BLEND(BS_SRC_ALPHA,     BS_INV_SRC_ALPHA, BS_OP_ADD,     BS_ONE,       BS_INV_SRC_ALPHA, BS_OP_ADD), // FilterBlend
		GS_BLEND(BS_ZERO,          BS_ONE,           BS_OP_ADD,     BS_ZERO,      BS_ONE, BS_OP_ADD),           // Ignore - leaves dest unchanged.
	};

	for (uint32 i = 0; i < (s_blendTypeCount - 1); ++i)
	{
		m_blendStates[i] = s_cryBlendTable[i % Blend_Count];
		if (i >= Blend_Count && ((m_blendStates[i] & BS_SRC_COLOR_MASK) >> BS_SRC_COLOR_SHIFT) == BS_SRC_ALPHA)
			m_blendStates[i] = (m_blendStates[i] & ~BS_SRC_COLOR_MASK) | BS_SET_SRC_COLOR(BS_ONE);
	}

	m_blendStates[Blend_Count * 2] = GS_NOCOLMASK_RGBA;
	return true;
}

void CSFRenderer::GetStats(Render::HAL::Stats* pstats, bool clear)
{
    *pstats = AccumulatedStats;
    if (clear)
        AccumulatedStats.Clear();
}

void CSFRenderTargets::SetTextures(RenderBuffer* buffer, CTexture* prt, DepthStencilBuffer* pdsb, CTexture* pdss)
{
	if (!buffer)
	{
		return;
	}

	CSFRenderTargets* poldHD = (CSFRenderTargets*)buffer->GetRenderTargetData();
	if (!poldHD)
	{
		poldHD = SF_NEW CSFRenderTargets(buffer, prt, pdsb, pdss);
		buffer->SetRenderTargetData(poldHD);
		return;
	}

	poldHD->pDepthStencilBuffer = pdsb;
	poldHD->pDepthStencilTexture = pdss;
	poldHD->pRenderTargetTexture = prt;
}

CSFRenderTargets::~CSFRenderTargets()
{
}

bool CSFRenderTargets::ReplaceDepthStencilBuffer(DepthStencilBuffer* pdsb)
{
	if (!Render::RenderTargetData::ReplaceDepthStencilBuffer(pdsb))
	{
		return false;
	}

	if (!pdsb)
	{
		pDepthStencilTexture.reset();
		return true;
	}

	CSFDepthStencilTexture* pdss = reinterpret_cast<CSFDepthStencilTexture*>(pdsb->GetSurface());
	pDepthStencilTexture = pdss->pTexture;
	return true;
}

CSFRenderTargets::CSFRenderTargets(RenderBuffer* buffer, CTexture* prt, DepthStencilBuffer* pdsb, CTexture* pdss)
	: Render::RenderTargetData(buffer, pdsb), pRenderTargetTexture(prt), pDepthStencilTexture(pdss)
{
}

} // ~Render namespace
} // ~Scaleform namespace
