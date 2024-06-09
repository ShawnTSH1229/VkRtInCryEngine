// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Renderer/SFConfig.h"
#include "Renderer/SFTexture.h"
#include "Renderer/SFShaderDescs.h"
#include <XRenderD3D9/GraphicsPipeline/Common/PrimitiveRenderPass.h>

namespace Scaleform {
namespace Render {

class CSFRenderCache : public NewOverrideBase<StatRender_RenderPipeline_Mem>, public RefCountVImpl
{
public:
	struct SRPCacheEntry
	{
		ETEX_Format colorFormat;
		ETEX_Format depthFormat;

		bool operator==(const SRPCacheEntry& rp) const
		{
			return colorFormat == rp.colorFormat && depthFormat == rp.depthFormat;
		}
	};

	struct SRPHash
	{
		uint64 operator()(const SRPCacheEntry& rp) const
		{
			return rp.colorFormat | uint64(rp.depthFormat) << 32;
		}
	};

	struct SPSOCacheEntry
	{
		ShaderDesc::ShaderType shaderType;
		uint32                 stencilState;
		uint64                 renderState;
		SamplerStateHandle     samplers[CSFTextureManager::maxTextureSlots];
		SRPCacheEntry          rp;

		bool operator==(const SPSOCacheEntry& pso) const
		{
			if (shaderType == pso.shaderType && renderState == pso.renderState && stencilState == pso.stencilState)
			{
				for (uint32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
				{
					if (samplers[i] != pso.samplers[i])
					{
						return false;
					}
				}
				return rp == pso.rp;
			}
			return false;
		}
	};

	struct SPSOHash
	{
		uint64 operator()(const SPSOCacheEntry& pso) const
		{
			uint64 hash = pso.renderState | uint64(pso.shaderType ^ pso.stencilState) << 32;
			for (uint32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
			{
				hash ^= uint64(pso.samplers[i].value) << (16 * i);
			}
			return hash ^ SRPHash()(pso.rp);
		}
	};

	struct SResourceSetCacheEntry
	{
		CTexture*          textures[CSFTextureManager::maxTextureSlots];
		SamplerStateHandle samplers[CSFTextureManager::maxTextureSlots];
		CSFRenderCache*    pCache;

		bool operator==(const SResourceSetCacheEntry& rsce) const
		{
			for (uint32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
			{
				if (textures[i] != rsce.textures[i])
				{
					return false;
				}
			}
			for (uint32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
			{
				if (samplers[i] != rsce.samplers[i])
				{
					return false;
				}
			}
			return pCache == rsce.pCache;
		}
	};

	struct SResourceSetHash
	{
		uint64 operator()(const SResourceSetCacheEntry& rsce) const
		{
			uint64 hash = 0;
			for (uint32 i = 0; i < CSFTextureManager::maxTextureSlots; ++i)
			{
				// Lets not count samplers into hash - the same texture is usually using the same sampler, so the sampler will be mostly a reduntant information.
				// Texture IDs should be in range of uint16 so let's spread all 4 IDs across uint64 hash.
				hash |= uint64(rsce.textures[i]->GetID()) << (16 * i);
			}
			return hash;
		}
	};

	virtual ~CSFRenderCache() override;

	static bool               OnTextureInvalidated(void* pListener, SResourceBindPoint bindPoint, UResourceReference resource, uint32 invalidationFlags);
	CDeviceResourceSetPtr     GetResourceSet(const SResourceSetCacheEntry& resourceSetEntry);
	CDeviceGraphicsPSOPtr     GetPSO(const SPSOCacheEntry& psoEntry, const CPrimitiveRenderPass& targetPass, InputLayoutHandle vertexLayout, CDeviceResourceLayoutPtr& pResourceLayout);
	CConstantBuffer*          GetConstantBuffer();
	SCompiledRenderPrimitive& GetCompiledPrimitive();
	CPrimitiveRenderPass&     GetRenderPass(SRPCacheEntry& rpCacheEntry);
	CClearSurfacePass&        GetClearPass();
	void                      Flush();

private:
	std::unordered_map<SPSOCacheEntry, std::pair<CDeviceGraphicsPSOPtr, CDeviceResourceLayoutPtr>, SPSOHash>                                m_psoCache;
	std::unordered_map<SResourceSetCacheEntry, CDeviceResourceSetPtr, SResourceSetHash>                                                     m_resourceSetCache;
	std::unordered_map<SRPCacheEntry, std::pair<std::forward_list<CPrimitiveRenderPass>, std::forward_list<CPrimitiveRenderPass>>, SRPHash> m_renderPassCache;
	std::forward_list<CConstantBufferPtr>       m_cachedConstantBuffers;
	std::forward_list<CConstantBufferPtr>       m_usedConstantBuffers;
	std::forward_list<SCompiledRenderPrimitive> m_cachedCompiledPrimitives;
	std::forward_list<SCompiledRenderPrimitive> m_usedCompiledPrimitives;
	std::forward_list<CClearSurfacePass>		m_cachedClearPasses;
	std::forward_list<CClearSurfacePass>		m_usedClearPasses;
};

} // ~Render namespace
} // ~Scaleform namespace
