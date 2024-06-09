// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"

#include "Renderer/SFTechnique.h"
#include "Renderer/SFRenderer.h"
#include <Kernel/SF_Debug.h>

namespace Scaleform {
namespace Render {

CSFVertexLayout::CSFVertexLayout(const VertexFormat* vf, const TechniqueDesc& techniqueDesc)
{
	D3D11_INPUT_ELEMENT_DESC elements[TechniqueDesc::MaxVertexAttributes];
	UINT offset = 0;
	for (int attr = 0; attr < techniqueDesc.NumAttribs; attr++)
	{
		D3D11_INPUT_ELEMENT_DESC& e = elements[attr];
		e.SemanticName = techniqueDesc.Attributes[attr].SemanticName;
		e.SemanticIndex = techniqueDesc.Attributes[attr].SemanticIndex;
		e.Format = (D3DFormat)techniqueDesc.Attributes[attr].Format;
		e.InputSlot = 0;
		e.AlignedByteOffset = offset;
		e.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		e.InstanceDataStepRate = 0;

		offset += DeviceFormats::GetStride(e.Format);
	}

	hVertexLayout = CDeviceObjectFactory::CreateCustomVertexFormat(techniqueDesc.NumAttribs, elements);
}

bool CSFTechniqueInterface::SetStaticShader(ShaderDesc::ShaderType shaderType, const VertexFormat* pFormat)
{
	const TechniqueDesc& techniqueDesc = TechniqueDesc::GetDesc(shaderType);
	m_currentTechnique.pVDesc = &techniqueDesc;
	m_currentTechnique.pFDesc = &techniqueDesc;
	m_currentTechnique.pVFormat = pFormat;
	m_currentTechnique.shaderType = shaderType;
	m_currentTechnique.batchSize = techniqueDesc.BatchSize;

	if (pFormat && !pFormat->pSysFormat)
	{
		(const_cast<VertexFormat*>(pFormat))->pSysFormat = *SF_NEW CSFVertexLayout(pFormat, techniqueDesc);
	}
	return (bool)m_currentTechnique;
}

void CSFTechniqueInterface::SetTexture(const Shader&, unsigned var, Render::Texture* pTexture, ImageFillMode fm, unsigned index)
{
	CSFTexture* pd3dTexture = (CSFTexture*)pTexture;
	CRY_ASSERT(m_currentTechnique.pFDesc->Uniforms[var].Location >= 0);
	CRY_ASSERT(m_currentTechnique.pFDesc->Uniforms[var].Location + m_currentTechnique.pFDesc->Uniforms[var].Size >= (short)(index + pTexture->TextureCount));
	pd3dTexture->ApplyTexture(m_currentTechnique.pFDesc->Uniforms[var].Location + index, fm);
}

void CSFTechniqueInterface::Finish(unsigned meshCount)
{
	ShaderInterfaceBase::Finish(meshCount);
	m_pHal->SetConstants(UniformData, m_uniformsCount * sizeof(float));
}

void CSFTechniqueManager::MapVertexFormat(
	PrimitiveFillType fill
	, const VertexFormat* sourceFormat
	, const VertexFormat** single, const VertexFormat** batch
	, const VertexFormat** instanced)
{
	VertexElement floatPositionElements[8];
	VertexFormat floatPositionFormat;
	floatPositionFormat.pElements = floatPositionElements;
	floatPositionFormat.pSysFormat = 0;
	uint32 i = 0;
	for (; sourceFormat->pElements[i].Attribute != VET_None; ++i)
	{
		floatPositionElements[i].Attribute = sourceFormat->pElements[i].Attribute;
		floatPositionElements[i].Offset = sourceFormat->pElements[i].Offset;
		if (VET_Pos == (floatPositionElements[i].Attribute & VET_Usage_Mask))
		{
			floatPositionElements[i].Attribute &= ~VET_CompType_Mask;
			floatPositionElements[i].Attribute |= VET_F32;
		}
	}
	floatPositionElements[i].Attribute = VET_None;
	floatPositionElements[i].Offset = 0;

	Base::MapVertexFormat(fill, &floatPositionFormat, single, batch, instanced, MVF_HasInstancing | MVF_Align);
}

} // ~Render namespace
} // ~Scaleform namespace
