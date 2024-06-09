#pragma once
#include <CryRenderer/IGIBaker.h>
#include "LightMapGBufferPass.h"
#include "LightMapRender.h"
#include "GIBakerCommon.h"

class CGIbaker : public IGIBaker
{
public:

	bool bInit = false;

	CGIbaker() {};

	virtual void AddDirectionalLight() override;
	virtual void AddPointLight() override;
	virtual void AddMesh(IStatObj* inputStatObj, SMeshParam meshParam) override;
	virtual void Bake() override;
	virtual void DirtyResource();
private:

	void PackMeshInToAtlas();
	void GenerateBufferHandle();
	void GenerateLightMapGBuffer();
	void RenderLightMap();
	void RealseResource();

	Vec2i m_nAtlasSize;
	int	  m_nAtlasNum;

	std::vector<SGIMeshDescription> m_giMeshArray;
	std::vector<SAtlasBakeInformation> m_atlasBakeInfomation;

	std::vector<Vec4>m_giLightMapOffsetAndBias;

	CLightMapGBufferGenerator m_lightMapGBufferGenerator;
	CLightMapRenderer m_lightMapRenderer;

	SBakerConfig m_config;
};