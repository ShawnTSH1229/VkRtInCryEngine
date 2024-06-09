#pragma once

struct SLightMapResult
{
	uint32 m_width;
	uint32 m_height;
};

struct SMeshTexCoord;

void GenerateLightMapUV(Vec3* positions, uint32_t vertexCount, uint32* indices, uint32 indexCound, Vec3* normals, SMeshTexCoord* outputTexCoords, SLightMapResult* pReturnResult);
