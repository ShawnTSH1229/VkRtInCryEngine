// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "Renderer/SFRenderCache.h"

namespace Scaleform {
namespace Render {

CSFRenderCache::~CSFRenderCache()
{
	for (const auto& entry : m_resourceSetCache)
	{
		for (int32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
		{
			if (CRendererResources::s_ptexNoTexture != entry.first.textures[i])
			{
				entry.first.textures[i]->RemoveInvalidateCallbacks(const_cast<SResourceSetCacheEntry*>(&entry.first));
			}
		}
	}
}

bool CSFRenderCache::OnTextureInvalidated(void* pListener, SResourceBindPoint bindPoint, UResourceReference resource, uint32 invalidationFlags)
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	SResourceSetCacheEntry* pResourceSetEntry = static_cast<SResourceSetCacheEntry*>(pListener);
	for (int32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
	{
		if (CRendererResources::s_ptexNoTexture != pResourceSetEntry->textures[i] && 
			resource.pTexture != pResourceSetEntry->textures[i])
		{
			pResourceSetEntry->textures[i]->RemoveInvalidateCallbacks(pResourceSetEntry);
		}
	}
	pResourceSetEntry->pCache->m_resourceSetCache.erase(*pResourceSetEntry);

	return false;
}

CDeviceResourceSetPtr CSFRenderCache::GetResourceSet(const SResourceSetCacheEntry& resourceSetEntry)
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	const auto& it = m_resourceSetCache.find(resourceSetEntry);
	if (it != m_resourceSetCache.end())
	{
		return (it->second);
	}

	CDeviceResourceSetDesc resourcesSetDesc;
	for (int32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
	{
		resourcesSetDesc.SetTexture(i, resourceSetEntry.textures[i], EDefaultResourceViews::Default, EShaderStage_Pixel);
		resourcesSetDesc.SetSampler(i, resourceSetEntry.samplers[i], EShaderStage_Pixel);
	}

	CDeviceResourceSetPtr pResourceSet = GetDeviceObjectFactory().CreateResourceSet(CDeviceResourceSet::EFlags_ForceSetAllState);
	if (pResourceSet->Update(resourcesSetDesc))
	{
		const auto& new_it = m_resourceSetCache.insert({ resourceSetEntry, pResourceSet });
		for (int32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
		{
			if (CRendererResources::s_ptexNoTexture != resourceSetEntry.textures[i])
			{
				resourceSetEntry.textures[i]->AddInvalidateCallback(const_cast<SResourceSetCacheEntry*>(&new_it.first->first), SResourceBindPoint(), OnTextureInvalidated);
			}
		}
	}

	return pResourceSet;
}

CDeviceGraphicsPSOPtr CSFRenderCache::GetPSO(const SPSOCacheEntry& psoEntry, const CPrimitiveRenderPass& targetPass, InputLayoutHandle vertexLayout, CDeviceResourceLayoutPtr& pResourceLayout)
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	auto it = m_psoCache.find(psoEntry);
	if (it != m_psoCache.end())
	{
		pResourceLayout = it->second.second;
		return it->second.first;
	}

	SDeviceResourceLayoutDesc resourceLayoutDesc;
	CDeviceResourceSetDesc resourcesSetDesc;

	for (int32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
	{
		resourcesSetDesc.SetTexture(i, nullptr, EDefaultResourceViews::Default, EShaderStage_Pixel);
		resourcesSetDesc.SetSampler(i, psoEntry.samplers[i], EShaderStage_Pixel);
	}

	resourceLayoutDesc.SetResourceSet(0, resourcesSetDesc);
	resourceLayoutDesc.SetConstantBuffer(1, eConstantBufferShaderSlot_PerDraw, EShaderStage_Vertex);
	resourceLayoutDesc.SetConstantBuffer(2, eConstantBufferShaderSlot_PerDraw, EShaderStage_Pixel);
	pResourceLayout = GetDeviceObjectFactory().CreateResourceLayout(resourceLayoutDesc);

	const TechniqueDesc& techniqueDesc = TechniqueDesc::GetDesc(psoEntry.shaderType);
	SShaderItem shaderItem = gEnv->pRenderer->EF_LoadShaderItem("Scaleform4", false, 0, 0, techniqueDesc.ShaderFeatures);

	CDeviceGraphicsPSODesc psoDesc;
	psoDesc.m_pResourceLayout = pResourceLayout;
	psoDesc.m_pShader = (CShader*)shaderItem.m_pShader;
	psoDesc.m_technique = techniqueDesc.Technique;
	psoDesc.m_ShaderFlags_RT = 0;
	psoDesc.m_ShaderFlags_MD = 0;
	psoDesc.m_ShaderFlags_MDV = MDV_NONE;
	psoDesc.m_PrimitiveType = eptTriangleList;
	psoDesc.m_VertexFormat = vertexLayout;
	psoDesc.m_RenderState = psoEntry.renderState;
	psoDesc.m_StencilState = psoEntry.stencilState;
	psoDesc.m_StencilReadMask = 0xFF;
	psoDesc.m_StencilWriteMask = 0xFF;
	psoDesc.m_CullMode = eCULL_None;
	psoDesc.m_bDepthClip = true;
	psoDesc.m_pRenderPass = targetPass.GetRenderPass();

	CDeviceGraphicsPSOPtr pPipelineState = GetDeviceObjectFactory().CreateGraphicsPSO(psoDesc);
	m_psoCache[psoEntry] = { pPipelineState, pResourceLayout };
	return pPipelineState;
}

SCompiledRenderPrimitive& CSFRenderCache::GetCompiledPrimitive()
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	if (m_cachedCompiledPrimitives.begin() == m_cachedCompiledPrimitives.end())
	{
		m_usedCompiledPrimitives.emplace_front();

		SCompiledRenderPrimitive& compiledPrimitive = *m_usedCompiledPrimitives.begin();
		compiledPrimitive.m_inlineConstantBuffers[0].shaderSlot = eConstantBufferShaderSlot_PerDraw;
		compiledPrimitive.m_inlineConstantBuffers[0].shaderStages = EShaderStage_Vertex;
		compiledPrimitive.m_inlineConstantBuffers[1].shaderSlot = eConstantBufferShaderSlot_PerDraw;
		compiledPrimitive.m_inlineConstantBuffers[1].shaderStages = EShaderStage_Pixel;
		return compiledPrimitive;
	}
	else
	{
		m_usedCompiledPrimitives.splice_after(m_usedCompiledPrimitives.before_begin(), m_cachedCompiledPrimitives, m_cachedCompiledPrimitives.before_begin());
		return *m_usedCompiledPrimitives.begin();
	}
}

CPrimitiveRenderPass& CSFRenderCache::GetRenderPass(SRPCacheEntry& rpCacheEntry)
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	auto& renderPasses = m_renderPassCache[rpCacheEntry];
	if (renderPasses.first.begin() == renderPasses.first.end())
	{
		renderPasses.second.emplace_front();
	}
	else
	{
		renderPasses.second.splice_after(renderPasses.second.before_begin(), renderPasses.first, renderPasses.first.before_begin());
	}

	return *renderPasses.second.begin();
}

CClearSurfacePass& CSFRenderCache::GetClearPass()
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	if (m_cachedClearPasses.begin() == m_cachedClearPasses.end())
	{
		m_usedClearPasses.emplace_front();
	}
	else
	{
		m_usedClearPasses.splice_after(m_usedClearPasses.before_begin(), m_cachedClearPasses, m_cachedClearPasses.before_begin());
	}
	return *m_usedClearPasses.begin();
}

CConstantBuffer* CSFRenderCache::GetConstantBuffer()
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());

	if (m_cachedConstantBuffers.begin() == m_cachedConstantBuffers.end())
	{
		const buffer_size_t size = Uniform::SU_TotalSize* sizeof(float);
		m_usedConstantBuffers.emplace_front(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_DevBufMan.CreateConstantBuffer(size));
	}
	else
	{
		m_usedConstantBuffers.splice_after(m_usedConstantBuffers.before_begin(), m_cachedConstantBuffers, m_cachedConstantBuffers.before_begin());
	}
	return *m_usedConstantBuffers.begin();
}

void CSFRenderCache::Flush()
{
	CRY_ASSERT(static_cast<CD3D9Renderer*>(gEnv->pRenderer)->m_pRT->IsRenderThread());
	for (auto& renderPasses : m_renderPassCache)
	{
		renderPasses.second.first.splice_after(renderPasses.second.first.before_begin(), renderPasses.second.second);
	}
	m_cachedClearPasses.splice_after(m_cachedClearPasses.before_begin(), m_usedClearPasses);
	m_cachedConstantBuffers.splice_after(m_cachedConstantBuffers.before_begin(), m_usedConstantBuffers);
	m_cachedCompiledPrimitives.splice_after(m_cachedCompiledPrimitives.before_begin(), m_usedCompiledPrimitives);
}

} // ~Render namespace
} // ~Scaleform namespace
