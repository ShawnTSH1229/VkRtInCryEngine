#include "StdAfx.h"
#include "xatlas.h"
#include "LightMapHelper.h"
#include "../MeshCompiler/MeshCompiler.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>

static void RandomColor(uint8_t* color)
{
	for (int i = 0; i < 3; i++)
		color[i] = uint8_t((rand() % 255 + 192) * 0.5f);
}

static void SetPixel(uint8_t* dest, int destWidth, int x, int y, const uint8_t* color)
{
	uint8_t* pixel = &dest[x * 3 + y * (destWidth * 3)];
	pixel[0] = color[0];
	pixel[1] = color[1];
	pixel[2] = color[2];
}

static void RasterizeLine(uint8_t* dest, int destWidth, const int* p1, const int* p2, const uint8_t* color)
{
	const int dx = abs(p2[0] - p1[0]), sx = p1[0] < p2[0] ? 1 : -1;
	const int dy = abs(p2[1] - p1[1]), sy = p1[1] < p2[1] ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int current[2];
	current[0] = p1[0];
	current[1] = p1[1];
	while (SetPixel(dest, destWidth, current[0], current[1], color), current[0] != p2[0] || current[1] != p2[1])
	{
		const int e2 = err;
		if (e2 > -dx) { err -= dy; current[0] += sx; }
		if (e2 < dy) { err += dx; current[1] += sy; }
	}
}

static void RasterizeTriangle(uint8_t* dest, int destWidth, const int* t0, const int* t1, const int* t2, const uint8_t* color)
{
	if (t0[1] > t1[1]) std::swap(t0, t1);
	if (t0[1] > t2[1]) std::swap(t0, t2);
	if (t1[1] > t2[1]) std::swap(t1, t2);
	int total_height = t2[1] - t0[1];
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1[1] - t0[1] || t1[1] == t0[1];
		int segment_height = second_half ? t2[1] - t1[1] : t1[1] - t0[1];
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1[1] - t0[1] : 0)) / segment_height;
		int A[2], B[2];
		for (int j = 0; j < 2; j++) {
			A[j] = int(t0[j] + (t2[j] - t0[j]) * alpha);
			B[j] = int(second_half ? t1[j] + (t2[j] - t1[j]) * beta : t0[j] + (t1[j] - t0[j]) * beta);
		}
		if (A[0] > B[0]) std::swap(A, B);
		for (int j = A[0]; j <= B[0]; j++)
			SetPixel(dest, destWidth, j, t0[1] + i, color);
	}
}

void DebugUnwarp(xatlas::Atlas* atlas)
{
	if (atlas->width > 0 && atlas->height > 0) {
		printf("Rasterizing result...\n");
		// Dump images.
		std::vector<uint8_t> outputTrisImage, outputChartsImage;
		const uint32_t imageDataSize = atlas->width * atlas->height * 3;
		outputTrisImage.resize(atlas->atlasCount * imageDataSize);
		outputChartsImage.resize(atlas->atlasCount * imageDataSize);
		assert(atlas->meshCount == 1);

		int invalidTriNum = 0;

		for (uint32_t i = 0; i < atlas->meshCount; i++) {
			const xatlas::Mesh& mesh = atlas->meshes[i];
			// Rasterize mesh triangles.
			const uint8_t white[] = { 255, 255, 255 };
			const uint32_t faceCount = mesh.indexCount / 3;
			uint32_t faceFirstIndex = 0;
			for (uint32_t f = 0; f < faceCount; f++) {
				int32_t atlasIndex = -1;
				int verts[255][2];
				const uint32_t faceVertexCount = 3;
				for (uint32_t j = 0; j < faceVertexCount; j++) {
					const xatlas::Vertex& v = mesh.vertexArray[mesh.indexArray[faceFirstIndex + j]];
					atlasIndex = v.atlasIndex; // The same for every vertex in the face.
					verts[j][0] = int(v.uv[0]);
					verts[j][1] = int(v.uv[1]);
				}
				if (atlasIndex < 0)
				{
					invalidTriNum++;
					faceFirstIndex += faceVertexCount;
					continue; // Skip faces that weren't atlased.
				}
					
				uint8_t color[3];
				RandomColor(color);
				uint8_t* imageData = &outputTrisImage[atlasIndex * imageDataSize];
				RasterizeTriangle(imageData, atlas->width, verts[0], verts[1], verts[2], color);
				for (uint32_t j = 0; j < faceVertexCount; j++)
					RasterizeLine(imageData, atlas->width, verts[j], verts[(j + 1) % faceVertexCount], white);
				faceFirstIndex += faceVertexCount;
			}

			assert(invalidTriNum <= 2000);

			// Rasterize mesh charts.
			for (uint32_t j = 0; j < mesh.chartCount; j++) {
				const xatlas::Chart* chart = &mesh.chartArray[j];
				uint8_t color[3];
				RandomColor(color);
				for (uint32_t k = 0; k < chart->faceCount; k++) {
					const uint32_t face = chart->faceArray[k];
					const uint32_t faceVertexCount = 3;
					faceFirstIndex = face * 3;

					int verts[255][2];
					for (uint32_t l = 0; l < faceVertexCount; l++) {
						const xatlas::Vertex& v = mesh.vertexArray[mesh.indexArray[faceFirstIndex + l]];
						verts[l][0] = int(v.uv[0]);
						verts[l][1] = int(v.uv[1]);
					}
					uint8_t* imageData = &outputChartsImage[chart->atlasIndex * imageDataSize];
					RasterizeTriangle(imageData, atlas->width, verts[0], verts[1], verts[2], color);
					for (uint32_t l = 0; l < faceVertexCount; l++)
						RasterizeLine(imageData, atlas->width, verts[l], verts[(l + 1) % faceVertexCount], white);
				}
			}
		}
		for (uint32_t i = 0; i < atlas->atlasCount; i++) {
			char filename[256];
			snprintf(filename, sizeof(filename), "example_tris%02u.tga", i);
			printf("Writing '%s'...\n", filename);
			stbi_write_tga(filename, atlas->width, atlas->height, 3, &outputTrisImage[i * imageDataSize]);
			snprintf(filename, sizeof(filename), "example_charts%02u.tga", i);
			printf("Writing '%s'...\n", filename);
			stbi_write_tga(filename, atlas->width, atlas->height, 3, &outputChartsImage[i * imageDataSize]);
		}
	}
}

void GenerateLightMapUV(Vec3* positions, uint32_t vertexCount, uint32* indices, uint32 indexCount, Vec3* normals, SMeshTexCoord* outputTexCoords, SLightMapResult* pReturnResult)
{
	xatlas::MeshDecl inputMesh;
	inputMesh.indexData = indices;
	inputMesh.indexCount = indexCount;
	inputMesh.indexFormat = xatlas::IndexFormat::UInt32;

	inputMesh.vertexCount = vertexCount;
	inputMesh.vertexPositionData = positions;
	inputMesh.vertexPositionStride = sizeof(Vec3);
	inputMesh.vertexNormalData = normals;
	inputMesh.vertexNormalStride = sizeof(Vec3);
	inputMesh.vertexUvData = nullptr;
	inputMesh.vertexUvStride = 0;

	xatlas::ChartOptions chartOptions;
	chartOptions.fixWinding = true;

	float texelSize = 0.2; // todo:

	xatlas::PackOptions packOptions;
	packOptions.padding = 1;
	packOptions.maxChartSize = 2046; // 2048 - 2 padding
	packOptions.blockAlign = true;

	//temporary code
	if (indexCount == 12 * 3 || indexCount == 3072 * 3)
	{
		packOptions.resolution = 1024;
		packOptions.texelsPerUnit = 0.0f;
	}
	else
	{
		packOptions.texelsPerUnit = 1.0f / texelSize;
	}

	

	xatlas::Atlas* atlas = xatlas::Create();
	xatlas::AddMeshError err = xatlas::AddMesh(atlas, inputMesh, 1);
	assert(err == xatlas::AddMeshError::Success);

	xatlas::Generate(atlas, chartOptions, packOptions);

	uint32 w = atlas->width;
	uint32 h = atlas->height;

	pReturnResult->m_width = w;
	pReturnResult->m_height = h;

	if (w == 0 || h == 0) {
		xatlas::Destroy(atlas);
		return;
	}

	const xatlas::Mesh& resultMesh = atlas->meshes[0];
	
	uint32 genIndexCount = resultMesh.indexCount;
	for (uint32 i = 0; i < genIndexCount; i ++)
	{
		uint32 newIndex = resultMesh.indexArray[i];
		uint32_t originIndex = resultMesh.vertexArray[newIndex].xref;
		float* uv = resultMesh.vertexArray[newIndex].uv;
		outputTexCoords[originIndex] = SMeshTexCoord(uv[0] / w, uv[1] / h);
		assert(originIndex >= 0 && originIndex <= vertexCount);
	}

#if 1
	DebugUnwarp(atlas);
#endif

	xatlas::Destroy(atlas);
}
