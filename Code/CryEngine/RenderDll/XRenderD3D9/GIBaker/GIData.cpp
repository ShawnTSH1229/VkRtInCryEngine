#include "StdAfx.h"
#include "GIData.h"

static CGIData* giData = nullptr;

CGIData* CD3D9Renderer::GetGIData()
{
	if (!giData)
	{
		giData = new CGIData();
	}
	return giData;
}

void CGIData::AddLightMapTextures(_smart_ptr<CTexture> lightTexture)
{
	m_lightTextures.push_back(lightTexture);
}

_smart_ptr<CTexture> CGIData::GetLightMapTexture(uint32 index)
{
	if (index < m_lightTextures.size())
	{
		return m_lightTextures[index];
	}
	return nullptr;
}
