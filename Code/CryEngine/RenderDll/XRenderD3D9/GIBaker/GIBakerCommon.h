#pragma once
#include "../GraphicsPipeline/Common/GraphicsPipelineStage.h"
#include "../GraphicsPipeline/Common/GraphicsPipelineStateSet.h"
#include "../GraphicsPipeline/Common/SceneRenderPass.h"
#include "../GraphicsPipeline/Common/FullscreenPass.h"
#include "../GraphicsPipeline/Common/UtilityPasses.h"
#include <CryRenderer/IGIBaker.h>

struct SLightMapParam
{
	Vec2i m_lightMapSize;
	Vec2i m_LightMapOffset;
	int m_LightMapIndex;
};

struct SGIMeshDescription
{
	std::vector<Vec3>m_positions;
	std::vector<Vec3>m_normals;
	std::vector<SMeshTexCoord>m_lightMapUVs;
	std::vector<vtx_idx>m_indices;

	uint32 m_vertexCount;
	uint32 m_indexCount;

	SMeshParam m_meshParam;
	SLightMapParam m_lightMapParam;

	IStatObj* m_statObj;
};

struct SGIMeshBufferHandles
{
	buffer_handle_t m_pPos;
	buffer_handle_t m_pNorm;
	buffer_handle_t m_p2U;
	buffer_handle_t m_pIB;
};

struct SAtlasGeometry
{
	SStreamInfo m_posStream;
	SStreamInfo m_normStream;
	SStreamInfo m_2uStream;
	SStreamInfo m_indexStream;

	uint32 m_nVertexCount = 0;
	uint32 m_nIndexCount = 0;

	SMeshParam m_meshParam;
};

struct SAtlasBakeInformation
{
	std::vector<SAtlasGeometry>m_atlasGeometries;
	_smart_ptr<CTexture> m_pPosTex;
	_smart_ptr<CTexture> m_pFaceNormalTex;
	_smart_ptr<CTexture> m_pShadingNormalTex;

	_smart_ptr<CTexture> m_pResultTex;
};

struct SBakerConfig
{
	int m_nMaxAtlasSize = 2048;
	int m_nMaxBounce = 5;
	Vec2i m_nUsedAtlasSize = Vec2i(2048, 2048);
};