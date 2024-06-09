// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Renderer/SFConfig.h"
#include "Renderer/SFShaderDescs.h"
#include <Render/Render_Shader.h>

namespace Scaleform {
namespace Render {

class CSFRenderer;
class CSFTexture;

struct SSFTechnique
{
	typedef ShaderDesc::ShaderType ShaderType;

	SSFTechnique() : pVDesc(nullptr), pFDesc(nullptr), pVFormat(0), shaderType(ShaderDesc::ST_None) {}

	const SSFTechnique* operator->() const { return this; }
	operator bool() const { return pVDesc && pFDesc && pVFormat; }

	const TechniqueDesc*   pVDesc;
	const TechniqueDesc*   pFDesc;
	const VertexFormat*    pVFormat;
	ShaderDesc::ShaderType shaderType;
	uint16                 batchSize;
};

class CSFVertexLayout : public Render::SystemVertexFormat
{
public:
	CSFVertexLayout(const VertexFormat* vf, const TechniqueDesc& vdesc);

	InputLayoutHandle hVertexLayout;
};

class CSFTechniqueInterface : public ShaderInterfaceBase<Uniform, SSFTechnique>
{
public:
	typedef const SSFTechnique Shader;

	CSFTechniqueInterface(Render::HAL* pRenderer) : m_pHal((CSFRenderer*)pRenderer), m_uniformsCount(0) {}

	const Shader& GetCurrentShaders() const { return m_currentTechnique; }
	bool          SetStaticShader(ShaderDesc::ShaderType shader, const VertexFormat* pvf);
	void          SetTexture(const Shader&, unsigned stage, Render::Texture* pTexture, ImageFillMode fm, unsigned index = 0);
	void          Finish(unsigned meshCount);

	void          SetUniformRaw(const Shader& sd, unsigned var, const float* v, unsigned n, unsigned index = 0, unsigned batch = 0)
	{
		CRY_ASSERT(batch == 0);
		SF_UNUSED(batch);

		if (sd.pFDesc->Uniforms[var].Size)
		{
			CRY_ASSERT(sd.pFDesc->Uniforms[var].ShadowOffset + sd.pFDesc->Uniforms[var].ElementSize * index + n <= Uniform::SU_TotalSize);
			uint32 offset = sd.pFDesc->Uniforms[var].ShadowOffset + sd.pFDesc->Uniforms[var].ElementSize * index;
			m_uniformsCount = max(m_uniformsCount, buffer_size_t(n + offset));
			memcpy(UniformData + offset, v, n * sizeof(float));
		}
	}

	void BeginPrimitive(unsigned meshCount = 1)
	{
		SF_UNUSED(meshCount);
		CRY_ASSERT(!PrimitiveOpen, "BeginPrimitive called multiple times, without Finish.");
		if (PrimitiveOpen)
		{
			return;
		}

		m_uniformsCount = 0;
		PrimitiveOpen = true;
		CurrentPrimitiveMeshCount = meshCount;
		memset(Textures, 0, sizeof(Textures));
	}

private:
	CSFRenderer*  m_pHal;
	SSFTechnique  m_currentTechnique;
	buffer_size_t m_uniformsCount;
};

template<>
inline unsigned ShaderInterfaceStoragelessBase<Uniform, SSFTechnique >::GetUniformSize(const SSFTechnique& sd, unsigned i)
{
	unsigned size = 0;
	if (sd->pFDesc->BatchUniforms[i].Offset >= 0)
	{
		size = sd->pFDesc->BatchUniforms[i].Size * sd->pFDesc->Uniforms[Uniform::SU_vfuniforms].ElementSize;
	}
	else if (sd->pFDesc->Uniforms[i].Location >= 0)
	{
		size = sd->pFDesc->Uniforms[i].Size;
	}
	return size;
}

template<>
inline void ShaderInterfaceStoragelessBase<Uniform, SSFTechnique >::SetUniform(const SSFTechnique& sd, unsigned var, const float* v, unsigned n, unsigned index, unsigned batch)
{
	if (sd.pFDesc->BatchUniforms[var].Offset >= 0)
	{
		SetUniformRaw(sd, Uniform::SU_vfuniforms, v, n, batch * sd.batchSize + sd.pFDesc->BatchUniforms[var].Offset + index);
	}
	else
	{
		SetUniformRaw(sd, var, v, n, index, batch);
	}
}

class CSFTechniqueManager : public StaticShaderManager<ShaderDesc, TechniqueDesc, Uniform, CSFTechniqueInterface, CSFTexture>
{
	friend class CSFTechniqueInterface;
public:
	typedef StaticShaderManager<ShaderDesc, TechniqueDesc, Uniform, CSFTechniqueInterface, CSFTexture> Base;
	typedef Uniform                                                                                    UniformType;

	CSFTechniqueManager(Render::HAL* pRenderer) : StaticShaderManager(pRenderer) {}

	void MapVertexFormat(PrimitiveFillType fill,
	                     const VertexFormat* sourceFormat,
	                     const VertexFormat** single,
	                     const VertexFormat** batch,
	                     const VertexFormat** instanced);

	bool            HasInstancingSupport() const { return true; }
	static unsigned GetDrawableImageFlags()      { return 0; }
};

} // ~Render namespace
} // ~Scaleform namespace
