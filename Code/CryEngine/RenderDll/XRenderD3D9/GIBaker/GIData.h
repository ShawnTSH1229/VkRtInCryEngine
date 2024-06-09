#pragma once
#include <vector>
#include "Common/Textures/Texture.h"

class CGIData
{
public:
	void AddLightMapTextures(_smart_ptr<CTexture> lightTexture);
	_smart_ptr<CTexture> GetLightMapTexture(uint32 index = 0);

private:
	std::vector<_smart_ptr<CTexture>> m_lightTextures;
};