#include "GIBaker/GIBaker.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "GIBaker/stb_rect_pack.h"
#include "XRenderD3D9/DriverD3D.h"

CGIbaker* pGIBaker = nullptr;

IGIBaker* CD3D9Renderer::GetIGIBaker()
{
	if (!pGIBaker)
	{
		pGIBaker = new CGIbaker();
	}

	return pGIBaker;
}

void CGIbaker::AddDirectionalLight()
{

}

void CGIbaker::AddPointLight()
{

}

void CGIbaker::AddMesh(IStatObj* inputStatObj, SMeshParam meshParam)
{
	m_giMeshArray.push_back(SGIMeshDescription());

	IIndexedMesh* pIMesh = inputStatObj->GetIndexedMesh();
	
	uint32 indexCount = pIMesh->GetIndexCount();
	uint32 vertexCount = pIMesh->GetVertexCount();
	uint32 lightMapUVCount = pIMesh->GetLightMapUVCount();
	assert(vertexCount == lightMapUVCount);

	SGIMeshDescription& giMeshDescription = m_giMeshArray.back();

	giMeshDescription.m_positions.resize(vertexCount);
	giMeshDescription.m_normals.resize(vertexCount);
	giMeshDescription.m_lightMapUVs.resize(vertexCount);
	giMeshDescription.m_indices.resize(indexCount);

	IIndexedMesh::SMeshDescription meshDescription;
	pIMesh->GetMeshDescription(meshDescription);
	assert(meshDescription.m_nVertCount = vertexCount);
	assert(meshDescription.m_nLightMapUVCount = vertexCount);
	assert(meshDescription.m_nIndexCount = indexCount);

	memcpy(giMeshDescription.m_positions.data(), meshDescription.m_pVerts, vertexCount * sizeof(Vec3));
	memcpy(giMeshDescription.m_normals.data(), meshDescription.m_pNorms, vertexCount * sizeof(SMeshNormal));
	memcpy(giMeshDescription.m_lightMapUVs.data(), meshDescription.m_pLightMapUV, vertexCount * sizeof(SMeshTexCoord));
	memcpy(giMeshDescription.m_indices.data(), meshDescription.m_pIndices, indexCount * sizeof(vtx_idx));

	giMeshDescription.m_vertexCount = vertexCount;
	giMeshDescription.m_indexCount  = indexCount;

	giMeshDescription.m_meshParam = meshParam;
	giMeshDescription.m_lightMapParam.m_lightMapSize = pIMesh->GetMesh()->m_nLightMapSize;

	giMeshDescription.m_statObj = inputStatObj;
}

static int NextPowOf2(int x)
{
	return static_cast<int>(pow(2, static_cast<int>(ceil(log(x) / log(2)))));
}

static std::vector<Vec3i> PackRects(const std::vector<Vec2i> sourceLightMapSizes, const Vec2i targetLightMapSize)
{
	std::vector<stbrp_node>stbrp_nodes;
	stbrp_nodes.resize(targetLightMapSize.x);
	memset(stbrp_nodes.data(), 0, sizeof(stbrp_node) * stbrp_nodes.size());

	stbrp_context context;
	stbrp_init_target(&context, targetLightMapSize.x, targetLightMapSize.y, stbrp_nodes.data(), targetLightMapSize.x);

	std::vector<stbrp_rect> stbrp_rects;
	stbrp_rects.resize(sourceLightMapSizes.size());

	for (int i = 0; i < sourceLightMapSizes.size(); i++) {
		stbrp_rects[i].id = i;
		stbrp_rects[i].w = sourceLightMapSizes[i].x;
		stbrp_rects[i].h = sourceLightMapSizes[i].y;
		stbrp_rects[i].x = 0;
		stbrp_rects[i].y = 0;
		stbrp_rects[i].was_packed = 0;
	}

	stbrp_pack_rects(&context, stbrp_rects.data(), stbrp_rects.size());

	std::vector<Vec3i> result;
	for (int i = 0; i < sourceLightMapSizes.size(); i++) 
	{
		result.push_back(Vec3i(stbrp_rects[i].x, stbrp_rects[i].y, stbrp_rects[i].was_packed != 0 ? 1 : 0));
	}
	return result;
}

void CGIbaker::DirtyResource()
{
	for (uint32 index = 0; index < m_giMeshArray.size(); index++)
	{
		SGIMeshDescription& giMeshDesc = m_giMeshArray[index];
		giMeshDesc.m_statObj->SetLightMapScaleAndOffset(giMeshDesc.m_meshParam.m_lightMapScaleAndBias);
		giMeshDesc.m_statObj->SetLightMapIndex(giMeshDesc.m_lightMapParam.m_LightMapIndex);
	}
}

void CGIbaker::PackMeshInToAtlas()
{
	m_nAtlasSize = Vec2i(0, 0);

	std::vector<Vec2i> giMeshLightMapSize;
	for (uint32 index = 0; index < m_giMeshArray.size(); index++)
	{
		SGIMeshDescription& giMeshDesc = m_giMeshArray[index];
		giMeshLightMapSize.push_back(giMeshDesc.m_lightMapParam.m_lightMapSize);
		m_nAtlasSize.x = std::max(m_nAtlasSize.x, giMeshDesc.m_lightMapParam.m_lightMapSize.x);
		m_nAtlasSize.y = std::max(m_nAtlasSize.y, giMeshDesc.m_lightMapParam.m_lightMapSize.y);
	}

	int nearestPowerOfTwo = NextPowOf2(m_nAtlasSize.x);
	nearestPowerOfTwo = std::max(nearestPowerOfTwo, NextPowOf2(m_nAtlasSize.y));

	m_nAtlasSize = Vec2i(nearestPowerOfTwo, nearestPowerOfTwo);
	assert(m_nAtlasSize.x <= m_config.m_nMaxAtlasSize&& m_nAtlasSize.y <= m_config.m_nMaxAtlasSize);

	Vec2i bestAtlasSize;
	int bestAtlasSlices = 0;
	int bestAtlasArea = std::numeric_limits<int>::max();
	std::vector<Vec3i> bestAtlasOffsets;

	while (m_nAtlasSize.x <= m_config.m_nMaxAtlasSize && m_nAtlasSize.y <= m_config.m_nMaxAtlasSize)
	{
		std::vector<Vec2i>remainLightMapSizes;
		std::vector<int>remainLightMapIndices;

		for (int32 index = 0; index < giMeshLightMapSize.size(); index++)
		{
			remainLightMapSizes.push_back(giMeshLightMapSize[index] + Vec2i(2, 2)); //padding
			remainLightMapIndices.push_back(index);

		}
		
		std::vector<Vec3i> lightMapOffsets;
		lightMapOffsets.resize(giMeshLightMapSize.size());

		int atlasIndex = 0;

		while (remainLightMapSizes.size() > 0)
		{
			std::vector<Vec3i> offsets = PackRects(remainLightMapSizes, m_nAtlasSize);

			std::vector<Vec2i>newRemainSizes;
			std::vector<int>newRemainIndices;

			for (int offsetIndex = 0; offsetIndex < offsets.size(); offsetIndex++)
			{
				Vec3i subOffset = offsets[offsetIndex];
				int lightMapIndex = remainLightMapIndices[offsetIndex];

				if (subOffset.z > 0)
				{
					subOffset.z = atlasIndex;
					lightMapOffsets[lightMapIndex] = subOffset + Vec3i(1, 1, 0);
				}
				else
				{
					newRemainSizes.push_back(remainLightMapSizes[offsetIndex]);
					newRemainIndices.push_back(lightMapIndex);
				}
			}

			remainLightMapSizes = newRemainSizes;
			remainLightMapIndices = newRemainIndices;
			atlasIndex++;
		}

		int totalArea = m_nAtlasSize.x * m_nAtlasSize.y * atlasIndex;
		if (totalArea < bestAtlasArea)
		{
			bestAtlasSize = m_nAtlasSize;
			bestAtlasOffsets = lightMapOffsets;
			bestAtlasSlices = atlasIndex;
			bestAtlasArea = totalArea;
		}

		if (m_nAtlasSize.x == m_nAtlasSize.y) 
		{
			m_nAtlasSize.x *= 2;
		}
		else 
		{
			m_nAtlasSize.y *= 2;
		}
	}

	for (uint32 index = 0; index < m_giMeshArray.size(); index++)
	{
		SGIMeshDescription& giMeshDesc = m_giMeshArray[index];
		giMeshDesc.m_lightMapParam.m_LightMapOffset = Vec2i(bestAtlasOffsets[index].x, bestAtlasOffsets[index].y);
		giMeshDesc.m_lightMapParam.m_LightMapIndex = bestAtlasOffsets[index].z;

		Vec2 scale = Vec2(giMeshDesc.m_lightMapParam.m_lightMapSize) / Vec2(bestAtlasSize);
		Vec2 bias = Vec2(giMeshDesc.m_lightMapParam.m_LightMapOffset) / Vec2(bestAtlasSize);
		Vec4 scaleAndBias = Vec4(scale.x, scale.y, bias.x, bias.y);
		giMeshDesc.m_meshParam.m_lightMapScaleAndBias = scaleAndBias;
	}

	m_config.m_nUsedAtlasSize = bestAtlasSize;
	m_nAtlasNum = bestAtlasSlices;
}

void CGIbaker::GenerateBufferHandle()
{
	for (uint32 index = 0; index < m_nAtlasNum; index++)
	{
		m_atlasBakeInfomation.push_back(SAtlasBakeInformation());
	}

	for (uint32 index = 0; index < m_giMeshArray.size(); index++)
	{
		SGIMeshDescription& giMeshDesc = m_giMeshArray[index];

		buffer_handle_t pPosBuffer = gcpRendD3D->m_DevBufMan.Create(BBT_VERTEX_BUFFER, BU_STATIC, giMeshDesc.m_vertexCount * sizeof(Vec3));
		buffer_handle_t pNormalBuffer = gcpRendD3D->m_DevBufMan.Create(BBT_VERTEX_BUFFER, BU_STATIC, giMeshDesc.m_vertexCount * sizeof(SMeshNormal));
		buffer_handle_t p2UBuffer = gcpRendD3D->m_DevBufMan.Create(BBT_VERTEX_BUFFER, BU_STATIC, giMeshDesc.m_vertexCount * sizeof(SMeshTexCoord));
		buffer_handle_t pIndexBuffer = gcpRendD3D->m_DevBufMan.Create(BBT_INDEX_BUFFER, BU_STATIC, giMeshDesc.m_indexCount * sizeof(vtx_idx));

		gcpRendD3D->m_DevBufMan.UpdateBuffer(pPosBuffer, giMeshDesc.m_positions.data(), giMeshDesc.m_vertexCount * sizeof(Vec3));
		gcpRendD3D->m_DevBufMan.UpdateBuffer(pNormalBuffer, giMeshDesc.m_normals.data(), giMeshDesc.m_vertexCount * sizeof(SMeshNormal));
		gcpRendD3D->m_DevBufMan.UpdateBuffer(p2UBuffer, giMeshDesc.m_lightMapUVs.data(), giMeshDesc.m_vertexCount * sizeof(SMeshTexCoord));
		gcpRendD3D->m_DevBufMan.UpdateBuffer(pIndexBuffer, giMeshDesc.m_indices.data(), giMeshDesc.m_indexCount * sizeof(vtx_idx));

		SStreamInfo posStreamInfo;
		posStreamInfo.hStream = pPosBuffer;
		posStreamInfo.nStride = sizeof(Vec3);
		posStreamInfo.nSlot = EStreamIDs::VSF_VERTEX_VELOCITY;
		
		SStreamInfo normStreamInfo;
		normStreamInfo.hStream = pNormalBuffer;
		normStreamInfo.nStride = sizeof(SMeshNormal);
		normStreamInfo.nSlot = EStreamIDs::VSF_NORMALS;

		SStreamInfo tuStreamInfo;
		tuStreamInfo.hStream = p2UBuffer;
		tuStreamInfo.nStride = sizeof(SMeshTexCoord);
		tuStreamInfo.nSlot = EStreamIDs::VSF_LIGHTMAPUV;

		SStreamInfo indexStreamInfo;
		indexStreamInfo.hStream = pIndexBuffer;
		indexStreamInfo.nStride = sizeof(vtx_idx);
		indexStreamInfo.nSlot = 0;
	
		m_atlasBakeInfomation[giMeshDesc.m_lightMapParam.m_LightMapIndex].m_atlasGeometries.push_back(SAtlasGeometry{ posStreamInfo ,normStreamInfo ,tuStreamInfo ,indexStreamInfo ,giMeshDesc.m_vertexCount,giMeshDesc.m_indexCount,giMeshDesc.m_meshParam });
	}

	
	m_lightMapGBufferGenerator.Init(m_giMeshArray.size(), m_config, m_atlasBakeInfomation);
	m_lightMapRenderer.InitScene(m_atlasBakeInfomation, m_config);

	bInit = true;
}

void CGIbaker::GenerateLightMapGBuffer()
{
	m_lightMapGBufferGenerator.GenerateLightMapGBuffer(m_atlasBakeInfomation);
}

void CGIbaker::RenderLightMap()
{
	m_lightMapRenderer.RenderLightMap(m_atlasBakeInfomation);
}

void CGIbaker::RealseResource()
{
	m_lightMapGBufferGenerator.ReleaseResource();
}

void CGIbaker::Bake()
{
	CD3D9Renderer* d3dRender = static_cast<CD3D9Renderer*>(gRenDev);
	d3dRender->m_pBakeFunctionCallBack = ([=]
		{
			if (!this->bInit)
			{
				this->PackMeshInToAtlas();
				this->GenerateBufferHandle();
				for (uint32 index = 0; index < m_atlasBakeInfomation.size(); index++)
				{
					d3dRender->GetGIData()->AddLightMapTextures(m_atlasBakeInfomation[index].m_pResultTex);
				}
			}

			this->GenerateLightMapGBuffer();
			this->RenderLightMap();
			this->RealseResource();
			this->DirtyResource();
		});
}
