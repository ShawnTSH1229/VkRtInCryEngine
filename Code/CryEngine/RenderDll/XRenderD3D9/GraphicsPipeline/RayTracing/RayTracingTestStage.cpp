#include "RayTracingTestStage.h"

void CRayTracingTestStage::CreateVbIb()
{
	// Create vertex buffer and index buffer
	m_RtVertices.push_back(SRtVertex{ Vec3(1, -1, 0) });
	m_RtVertices.push_back(SRtVertex{ Vec3(1,  1, 0) });
	m_RtVertices.push_back(SRtVertex{ Vec3(-1,  1, 0) });

	m_RtIndices.push_back(0);
	m_RtIndices.push_back(1);
	m_RtIndices.push_back(2);

	m_pVB = gcpRendD3D->m_DevBufMan.Create(BBT_VERTEX_BUFFER, BU_STATIC, m_RtVertices.size() * sizeof(SRtVertex));
	m_pIB = gcpRendD3D->m_DevBufMan.Create(BBT_INDEX_BUFFER, BU_STATIC, m_RtIndices.size() * sizeof(SRtIndex));

	gcpRendD3D->m_DevBufMan.UpdateBuffer(m_pVB, &m_RtVertices[0], m_RtVertices.size() * sizeof(SRtVertex));
	gcpRendD3D->m_DevBufMan.UpdateBuffer(m_pIB, &m_RtIndices[0], m_RtIndices.size() * sizeof(SRtIndex));

	SStreamInfo vertexStreamInfo;
	vertexStreamInfo.hStream = m_pVB;
	vertexStreamInfo.nStride = sizeof(SRtVertex);
	vertexStreamInfo.nSlot = 0;
	m_pVertexInputSet = GetDeviceObjectFactory().CreateVertexStreamSet(1, &vertexStreamInfo);

	SStreamInfo indexStreamInfo;
	indexStreamInfo.hStream = m_pIB;
	indexStreamInfo.nStride = sizeof(SRtIndex);
	indexStreamInfo.nSlot = 0;
	m_pIndexInputSet = GetDeviceObjectFactory().CreateIndexStreamSet(&indexStreamInfo);
}

void CRayTracingTestStage::CreateAndBuildBLAS(CDeviceGraphicsCommandInterface* pCommandInterface)
{
	SRayTracingBottomLevelASCreateInfo rtBottomLevelCreateInfo;
	rtBottomLevelCreateInfo.m_eBuildFlag = EBuildAccelerationStructureFlag::eBuild_PreferBuild;
	rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_sIndexStreaming = m_pIndexInputSet;
	rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_nIndexBufferOffset = 0;

	SRayTracingGeometryTriangle rtGeometryTriangle;
	rtGeometryTriangle.m_sTriangVertexleInfo.m_sVertexStreaming = m_pVertexInputSet;
	rtGeometryTriangle.m_sTriangVertexleInfo.m_nMaxVertex = m_RtVertices.size();
	rtGeometryTriangle.m_sTriangVertexleInfo.m_hVertexPositionFormat = EDefaultInputLayouts::P3F;
	rtGeometryTriangle.m_sRangeInfo.m_nFirstVertex = 0;
	rtGeometryTriangle.m_sRangeInfo.m_nPrimitiveCount = 1;
	rtGeometryTriangle.m_sRangeInfo.m_nPrimitiveOffset = 0;
	rtGeometryTriangle.bEnable = true;
	rtGeometryTriangle.bForceOpaque = true;
	rtBottomLevelCreateInfo.m_rtGeometryTriangles.push_back(rtGeometryTriangle);

	m_pRtBottomLevelAS = GetDeviceObjectFactory().CreateRayTracingBottomLevelAS(rtBottomLevelCreateInfo);

	std::vector<CRayTracingBottomLevelAccelerationStructurePtr> rtBottomLevelASPtrs;
	rtBottomLevelASPtrs.push_back(m_pRtBottomLevelAS);
	pCommandInterface->BuildRayTracingBottomLevelASs(rtBottomLevelASPtrs);
}


void CRayTracingTestStage::CreateAndBuildTLAS(CDeviceGraphicsCommandInterface* pCommandInterface)
{
	SRayTracingInstanceTransform transform;
	memset(&transform.m_aTransform, 0, sizeof(float) * 3 * 4);
	transform.m_aTransform[0][0] = 1;
	transform.m_aTransform[1][1] = 1;
	transform.m_aTransform[2][2] = 1;

	SAccelerationStructureInstanceInfo accelerationStructureInstanceInfo;
	accelerationStructureInstanceInfo.m_transformPerInstance.push_back(transform);
	accelerationStructureInstanceInfo.m_nCustomIndex.push_back(0);
	accelerationStructureInstanceInfo.m_blas = m_pRtBottomLevelAS;
	accelerationStructureInstanceInfo.m_rayMask = 0xFF;

	std::vector<SAccelerationStructureInstanceInfo> accelerationStructureInstanceInfos;
	accelerationStructureInstanceInfos.push_back(accelerationStructureInstanceInfo);

	SRayTracingTopLevelASCreateInfo rtTopLevelASCreateInfo;
	rtTopLevelASCreateInfo.m_nHitShaderNumPerTriangle = 1;
	rtTopLevelASCreateInfo.m_nMissShaderNum = 1;
	for (const auto& blas : accelerationStructureInstanceInfos)
	{
		rtTopLevelASCreateInfo.m_nInstanceTransform += blas.m_transformPerInstance.size();
		rtTopLevelASCreateInfo.m_nTotalGeometry += blas.m_blas->m_sRtBlASCreateInfo.m_rtGeometryTriangles.size();
		rtTopLevelASCreateInfo.m_nPreGeometryNumSum.push_back(rtTopLevelASCreateInfo.m_nTotalGeometry);
	}

	m_pRtTopLevelAS = GetDeviceObjectFactory().CreateRayTracingTopLevelAS(rtTopLevelASCreateInfo);

	std::vector<SAccelerationStructureInstanceData> accelerationStructureInstanceDatas;
	accelerationStructureInstanceDatas.resize(accelerationStructureInstanceInfos.size());
	for (uint32 index = 0; index < accelerationStructureInstanceInfos.size(); index++)
	{
		const SAccelerationStructureInstanceInfo& instanceInfo = accelerationStructureInstanceInfos[index];
		SAccelerationStructureInstanceData& instanceData = accelerationStructureInstanceDatas[index];
		for (uint32 transformIndex = 0; transformIndex < instanceInfo.m_transformPerInstance.size(); transformIndex++)
		{
			instanceData.m_aTransform = instanceInfo.m_transformPerInstance[transformIndex].m_aTransform;
			instanceData.m_nInstanceCustomIndex = instanceInfo.m_nCustomIndex[transformIndex];
			instanceData.m_nMask = instanceInfo.m_rayMask;

			//@todo fixme
			instanceData.m_instanceShaderBindingTableRecordOffset = 0;
			instanceData.m_flags = 0x00000002; // Flip Face

			instanceData.accelerationStructureReference = instanceInfo.m_blas->GetAccelerationStructureAddress();
		}
	}

	m_instanceBuffer.Create(accelerationStructureInstanceDatas.size(), sizeof(SAccelerationStructureInstanceData), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::USAGE_ACCELERATION_STRUCTURE, accelerationStructureInstanceDatas.data());
	pCommandInterface->BuildRayTracingTopLevelAS(m_pRtTopLevelAS, &m_instanceBuffer, 0);
}

void CRayTracingTestStage::CreateRayAndResultBuffer()
{
	constexpr uint32 NumRays = 4;

	std::vector<SRayTracingRay> rayTracingRays;
	rayTracingRays.resize(NumRays);
	rayTracingRays[0] = SRayTracingRay{ { 0.75f, 0.0f, -1.0f}, 0xFFFFFFFF, {0.0f, 0.0f,  1.0f}, 100000.0f }; // expected to hit
	rayTracingRays[1] = SRayTracingRay{ { 0.75f, 0.0f, -1.0f}, 0xFFFFFFFF, {0.0f, 0.0f,  1.0f},      0.5f }; // expected to miss (short ray)
	rayTracingRays[2] = SRayTracingRay{ { 0.75f, 0.0f,  1.0f}, 0xFFFFFFFF, {0.0f, 0.0f, -1.0f}, 100000.0f }; // expected to hit  (should hit back face)
	rayTracingRays[3] = SRayTracingRay{ {-0.75f, 0.0f, -1.0f}, 0xFFFFFFFF, {0.0f, 0.0f,  1.0f}, 100000.0f }; // expected to miss (doesn't intersect)
	m_rayBuffer.Create(4, sizeof(SRayTracingRay), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_SHADER_RESOURCE, rayTracingRays.data());
	m_rayBuffer.SetDebugName("RayBuffer");

	m_resultBuffer.Create(NumRays, sizeof(uint32), DXGI_FORMAT_R32_UINT, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::USAGE_CPU_WRITE | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
	m_resultBuffer.SetDebugName("RayTracingResultBuffer");
}

CRayTracingTestStage::~CRayTracingTestStage()
{
	delete m_pVertexInputSet;
	delete m_pIndexInputSet;
}

void CRayTracingTestStage::Init()
{
	CDeviceGraphicsCommandInterface* pCommandInterface = GetDeviceObjectFactory().GetCoreCommandList().GetGraphicsInterface();
	CreateVbIb();
	CreateAndBuildBLAS(pCommandInterface);
	CreateAndBuildTLAS(pCommandInterface);
	CreateRayAndResultBuffer();
}

void CRayTracingTestStage::Execute()
{
	m_rayTracingRenderPass.SetTechnique(CShaderMan::s_shRayTracingTest, CCryNameTSCRC("RayTracingTestTech"), 0);
	m_rayTracingRenderPass.SetBuffer(0, m_pRtTopLevelAS->GetAccelerationStructureBuffer());
	m_rayTracingRenderPass.SetBuffer(1, &m_rayBuffer);
	m_rayTracingRenderPass.SetOutputUAV(2, &m_resultBuffer);
	m_rayTracingRenderPass.PrepareResourcesForUse(GetDeviceObjectFactory().GetCoreCommandList());
	m_rayTracingRenderPass.DispatchRayTracing(GetDeviceObjectFactory().GetCoreCommandList(), 4, 1);
}

