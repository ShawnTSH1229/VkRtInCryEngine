#include "BindlessRayTracingTestStage.h"

// function to generate a sphere mesh
static void CreateSphere(ObjVertexBuffer& vb, ObjIndexBuffer& ib, float radius, unsigned int rings, unsigned int sections)
{
	// calc required number of vertices/indices/triangles to build a sphere for the given parameters
	uint32 numVertices((rings - 1) * (sections + 1) + 2);
	uint32 numTriangles((rings - 2) * sections * 2 + 2 * sections);
	uint32 numIndices(numTriangles * 3);

	// setup buffers
	vb.clear();
	vb.reserve(numVertices);

	ib.clear();
	ib.reserve(numIndices);

	// 1st pole vertex
	vb.emplace_back(Vec3(0.0f, 0.0f, radius), Vec3(0.0f, 0.0f, 1.0f));

	// calculate "inner" vertices
	float sectionSlice(DEG2RAD(360.0f / (float)sections));
	float ringSlice(DEG2RAD(180.0f / (float)rings));

	for (uint32 a(1); a < rings; ++a)
	{
		float w(sinf(a * ringSlice));
		for (uint32 i(0); i <= sections; ++i)
		{
			Vec3 v;
			v.x = radius * cosf(i * sectionSlice) * w;
			v.y = radius * sinf(i * sectionSlice) * w;
			v.z = radius * cosf(a * ringSlice);
			vb.emplace_back(v, v.GetNormalized());
		}
	}

	// 2nd vertex of pole (for end cap)
	vb.emplace_back(Vec3(0.0f, 0.0f, -radius), Vec3(0.0f, 0.0f, 1.0f));

	// build "inner" faces
	for (uint32 a(0); a < rings - 2; ++a)
	{
		for (uint32 i(0); i < sections; ++i)
		{
			ib.push_back((vtx_idx)(1 + a * (sections + 1) + i + 1));
			ib.push_back((vtx_idx)(1 + a * (sections + 1) + i));
			ib.push_back((vtx_idx)(1 + (a + 1) * (sections + 1) + i + 1));

			ib.push_back((vtx_idx)(1 + (a + 1) * (sections + 1) + i));
			ib.push_back((vtx_idx)(1 + (a + 1) * (sections + 1) + i + 1));
			ib.push_back((vtx_idx)(1 + a * (sections + 1) + i));
		}
	}

	// build faces for end caps (to connect "inner" vertices with poles)
	for (uint32 i(0); i < sections; ++i)
	{
		ib.push_back((vtx_idx)(1 + (0) * (sections + 1) + i));
		ib.push_back((vtx_idx)(1 + (0) * (sections + 1) + i + 1));
		ib.push_back((vtx_idx)0);
	}

	for (uint32 i(0); i < sections; ++i)
	{
		ib.push_back((vtx_idx)(1 + (rings - 2) * (sections + 1) + i + 1));
		ib.push_back((vtx_idx)(1 + (rings - 2) * (sections + 1) + i));
		ib.push_back((vtx_idx)((rings - 1) * (sections + 1) + 1));
	}
}

// function to generate a cylinder mesh
static void CreateCylinder(ObjVertexBuffer& vb, ObjIndexBuffer& ib, float radius, float height, unsigned int sections)
{
	// calc required number of vertices/indices/triangles to build a cylinder for the given parameters
	uint32 numVertices(4 * (sections + 1) + 2);
	uint32 numTriangles(4 * sections);
	uint32 numIndices(numTriangles * 3);
	// setup buffers
	vb.clear();
	vb.reserve(numVertices);
	ib.clear();
	ib.reserve(numIndices);
	float sectionSlice(DEG2RAD(360.0f / (float)sections));
	// bottom cap
	{
		// center bottom vertex
		vb.emplace_back(Vec3(0.0f, -0.5f * height, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
		// create circle around it
		for (uint32 i(0); i <= sections; ++i)
		{
			Vec3 v;
			v.x = radius * cosf(i * sectionSlice);
			v.y = -0.5f * height;
			v.z = radius * sinf(i * sectionSlice);
			vb.emplace_back(v, Vec3(0.0f, -1.0f, 0.0f));
		}
		// build faces
		for (uint16 i(0); i < sections; ++i)
		{
			ib.push_back((vtx_idx)(0));
			ib.push_back((vtx_idx)(1 + i));
			ib.push_back((vtx_idx)(1 + i + 1));
		}
	}
	// side
	{
		int vIdx(vb.size());
		for (uint32 i(0); i <= sections; ++i)
		{
			Vec3 v;
			v.x = radius * cosf(i * sectionSlice);
			v.y = -0.5f * height;
			v.z = radius * sinf(i * sectionSlice);
			Vec3 n(v.normalized());
			vb.emplace_back(v, n);
			vb.emplace_back(Vec3(v.x, -v.y, v.z), n);
		}
		// build faces
		for (uint16 i(0); i < sections; ++i, vIdx += 2)
		{
			ib.push_back((vtx_idx)(vIdx));
			ib.push_back((vtx_idx)(vIdx + 1));
			ib.push_back((vtx_idx)(vIdx + 2));
			ib.push_back((vtx_idx)(vIdx + 1));
			ib.push_back((vtx_idx)(vIdx + 3));
			ib.push_back((vtx_idx)(vIdx + 2));
		}
	}
	// top cap
	{
		size_t vIdx(vb.size());
		// center top vertex
		vb.emplace_back(Vec3(0.0f, 0.5f * height, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		// create circle around it
		for (uint32 i(0); i <= sections; ++i)
		{
			Vec3 v;
			v.x = radius * cosf(i * sectionSlice);
			v.y = 0.5f * height;
			v.z = radius * sinf(i * sectionSlice);
			vb.emplace_back(v, Vec3(0.0f, 1.0f, 0.0f));
		}
		// build faces
		for (uint16 i(0); i < sections; ++i)
		{
			ib.push_back(vIdx);
			ib.push_back(vIdx + 1 + i + 1);
			ib.push_back(vIdx + 1 + i);
		}
	}
}
SObjectGeometry::~SObjectGeometry()
{
	GetDeviceObjectFactory().UnBindBindlessResource(m_vbBindlessIndex, 2);
	GetDeviceObjectFactory().UnBindBindlessResource(m_ibBindlessIndex, 3);
}

template<typename TMeshFunc>
void CreateMesh(SObjectGeometry& ObjectGeometry, TMeshFunc meshFunction)
{
	// Create vb/ib
	meshFunction(ObjectGeometry.m_objectVB, ObjectGeometry.m_objectIB);

	ObjectGeometry.m_pObjectVB = gcpRendD3D->m_DevBufMan.Create(BBT_VERTEX_BUFFER, BU_STATIC, ObjectGeometry.m_objectVB.size() * sizeof(SObjVertex));
	ObjectGeometry.m_pObjectIB = gcpRendD3D->m_DevBufMan.Create(BBT_INDEX_BUFFER, BU_STATIC, ObjectGeometry.m_objectIB.size() * sizeof(vtx_idx));

	gcpRendD3D->m_DevBufMan.UpdateBuffer(ObjectGeometry.m_pObjectVB, &ObjectGeometry.m_objectVB[0], ObjectGeometry.m_objectVB.size() * sizeof(SObjVertex));
	gcpRendD3D->m_DevBufMan.UpdateBuffer(ObjectGeometry.m_pObjectIB, &ObjectGeometry.m_objectIB[0], ObjectGeometry.m_objectIB.size() * sizeof(vtx_idx));

	SStreamInfo vertexStreamInfo;
	vertexStreamInfo.hStream = ObjectGeometry.m_pObjectVB;
	vertexStreamInfo.nStride = sizeof(SObjVertex);
	vertexStreamInfo.nSlot = 0;
	ObjectGeometry.m_pObjectVertexInputSet = GetDeviceObjectFactory().CreateVertexStreamSet(1, &vertexStreamInfo);

	SStreamInfo indexStreamInfo;
	indexStreamInfo.hStream = ObjectGeometry.m_pObjectIB;
	indexStreamInfo.nStride = sizeof(vtx_idx);
	indexStreamInfo.nSlot = 0;
	ObjectGeometry.m_pObjectIndexInputSet = GetDeviceObjectFactory().CreateIndexStreamSet(&indexStreamInfo);

	ObjectGeometry.m_vbBindlessIndex = GetDeviceObjectFactory().SetBindlessStorageBuffer(ObjectGeometry.m_pObjectVertexInputSet, 2);
	ObjectGeometry.m_ibBindlessIndex = GetDeviceObjectFactory().SetBindlessStorageBuffer(ObjectGeometry.m_pObjectIndexInputSet, 3);
}

void CBindlessRayTracingTestStage::CreateVBAndIB()
{
	CreateMesh(m_objectSphere, [](ObjVertexBuffer& vb, ObjIndexBuffer& ib)
		{
			CreateSphere(vb, ib, 1.0, 16, 16);
		}
	);

	CreateMesh(m_objectCylinder, [](ObjVertexBuffer& vb, ObjIndexBuffer& ib)
		{
			CreateCylinder(vb, ib, 1.0, 1.0, 16);
		}
	);

	CreateMesh(m_objectPlane, [](ObjVertexBuffer& vb, ObjIndexBuffer& ib)
		{
			vb.clear();
			ib.clear();

			//cw
			vb.push_back(SObjVertex{ Vec3( 8.0, -8.0, 0.0),Vec3(0, 0.0, 1.0) });
			vb.push_back(SObjVertex{ Vec3( 8.0,  8.0, 0.0),Vec3(0, 0.0, 1.0) });
			vb.push_back(SObjVertex{ Vec3(-8.0,  8.0, 0.0),Vec3(0, 0.0, 1.0) });
			vb.push_back(SObjVertex{ Vec3(-8.0, -8.0, 0.0),Vec3(0, 0.0, 1.0) });

			ib.push_back(1);
			ib.push_back(0);
			ib.push_back(2);

			ib.push_back(2);
			ib.push_back(0);
			ib.push_back(3);
		}
	);

	m_objectGeometry[0] = &m_objectSphere;
	m_objectGeometry[0]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(-4.0, 0.0, 3.0)) });
	m_objectGeometry[0]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(-0.0, 0.0, 3.0)) });
	m_objectGeometry[0]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3( 4.0, 0.0, 3.0)) });
	
	m_objectGeometry[0]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(-4.0, 4.0, 3.0)) });
	m_objectGeometry[0]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(-0.0, 4.0, 3.0)) });
	m_objectGeometry[0]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3( 4.0, 4.0, 3.0)) });

	m_objectGeometry[1] = &m_objectCylinder;
	m_objectGeometry[1]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(-4.0,-4.0, 3.0)) });
	m_objectGeometry[1]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(-0.0,-4.0, 3.0)) });
	m_objectGeometry[1]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3( 4.0,-4.0, 3.0)) });

	m_objectGeometry[2] = &m_objectPlane;
	m_objectGeometry[2]->m_rayTracingTransforms.push_back(SRayTracingInstanceTransform{ Matrix34::CreateTranslationMat(Vec3(0.0, 0.0, 0.0)) });
}

void CBindlessRayTracingTestStage::CreateAndBuildBLAS(CDeviceGraphicsCommandInterface* pCommandInterface)
{
	for (uint32 index = 0; index < m_nObjectNum; index++)
	{
		SRayTracingBottomLevelASCreateInfo rtBottomLevelCreateInfo;
		rtBottomLevelCreateInfo.m_eBuildFlag = EBuildAccelerationStructureFlag::eBuild_PreferBuild;
		rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_sIndexStreaming = m_objectGeometry[index]->m_pObjectIndexInputSet;
		rtBottomLevelCreateInfo.m_sSTriangleIndexInfo.m_nIndexBufferOffset = 0;

		SRayTracingGeometryTriangle rtGeometryTriangle;
		rtGeometryTriangle.m_sTriangVertexleInfo.m_sVertexStreaming = m_objectGeometry[index]->m_pObjectVertexInputSet;
		rtGeometryTriangle.m_sTriangVertexleInfo.m_nMaxVertex = m_objectGeometry[index]->m_objectVB.size();
		rtGeometryTriangle.m_sTriangVertexleInfo.m_hVertexPositionFormat = EDefaultInputLayouts::P3F;
		rtGeometryTriangle.m_sRangeInfo.m_nFirstVertex = 0;
		CRY_ASSERT(m_objectGeometry[index]->m_objectIB.size() % 3 == 0);
		rtGeometryTriangle.m_sRangeInfo.m_nPrimitiveCount = m_objectGeometry[index]->m_objectIB.size() / 3;
		rtGeometryTriangle.m_sRangeInfo.m_nPrimitiveOffset = 0;
		rtGeometryTriangle.bEnable = true;
		rtGeometryTriangle.bForceOpaque = true;
		rtBottomLevelCreateInfo.m_rtGeometryTriangles.push_back(rtGeometryTriangle);

		m_objectGeometry[index]->m_pRtBottomLevelAS = GetDeviceObjectFactory().CreateRayTracingBottomLevelAS(rtBottomLevelCreateInfo);
	}

	std::vector<CRayTracingBottomLevelAccelerationStructurePtr> rtBottomLevelASPtrs;
	for (uint32 index = 0; index < m_nObjectNum; index++)
	{
		rtBottomLevelASPtrs.push_back(m_objectGeometry[index]->m_pRtBottomLevelAS);
	}
	
	pCommandInterface->BuildRayTracingBottomLevelASs(rtBottomLevelASPtrs);
}

void CBindlessRayTracingTestStage::CreateAndBuildTLAS(CDeviceGraphicsCommandInterface* pCommandInterface)
{
	std::vector<SAccelerationStructureInstanceInfo> accelerationStructureInstanceInfos;
	std::vector<SBindlessIndex>bindlessIndexArray;

	for (uint32 geometryIndex = 0; geometryIndex < m_nObjectNum; geometryIndex++)
	{
		for (uint32 transformIndex = 0; transformIndex < m_objectGeometry[geometryIndex]->m_rayTracingTransforms.size(); transformIndex++)
		{
			SAccelerationStructureInstanceInfo accelerationStructureInstanceInfo;
			accelerationStructureInstanceInfo.m_transformPerInstance.push_back(m_objectGeometry[geometryIndex]->m_rayTracingTransforms[transformIndex]);
			accelerationStructureInstanceInfo.m_nCustomIndex.push_back(0);
			accelerationStructureInstanceInfo.m_blas = m_objectGeometry[geometryIndex]->m_pRtBottomLevelAS;
			accelerationStructureInstanceInfo.m_rayMask = 0xFF;

			accelerationStructureInstanceInfos.push_back(accelerationStructureInstanceInfo);
			SBindlessIndex bindlessIndex = { m_objectGeometry[geometryIndex]->m_vbBindlessIndex,m_objectGeometry[geometryIndex]->m_ibBindlessIndex };
			bindlessIndexArray.push_back(bindlessIndex);
		}
	}

	m_pBindlessIndexBuffer.Create(bindlessIndexArray.size(), sizeof(SBindlessIndex), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_SHADER_RESOURCE, bindlessIndexArray.data());
	

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

			instanceData.m_instanceShaderBindingTableRecordOffset = 0;
			instanceData.m_flags = 0x00000000; 

			instanceData.accelerationStructureReference = instanceInfo.m_blas->GetAccelerationStructureAddress();
		}
	}

	m_instanceBuffer.Create(accelerationStructureInstanceDatas.size(), sizeof(SAccelerationStructureInstanceData), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::USAGE_ACCELERATION_STRUCTURE, accelerationStructureInstanceDatas.data());
	pCommandInterface->BuildRayTracingTopLevelAS(m_pRtTopLevelAS, &m_instanceBuffer, 0);
}

void CBindlessRayTracingTestStage::CreateUniformBuffer()
{
	float fov = 60.0 / 360.0 * (2 * PI);
	float aspect = 1.0 / 0.618;

	Vec3 eye(5.0, -15.0, 8.0);
	Vec3 lookAt(0.0, 0.0, 0.0);
	Vec3 up(0.0, 0.0, 1.0);

	Matrix44A mProj, mView, mVP;
	mathMatrixPerspectiveFov(&mProj, fov, aspect, 0.1f, 1000.0f);
	mathMatrixLookAt(&mView, eye, lookAt, up);
	mVP = mView * mProj;

	SRayCameraMatrix rayCameraMatrix;
	rayCameraMatrix.m_viewProj = mVP.GetTransposed();

	mView.Invert();
	rayCameraMatrix.m_viewInverse = mView.GetTransposed();

	mProj.Invert();
	rayCameraMatrix.m_projInverse = mProj.GetTransposed();

	rayCameraMatrix.m_lightDirection = Vec4(-1.0, -1.0, 1.0, 1.0);

	m_pRayTracingCB = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(sizeof(SRayCameraMatrix));
	m_pRayTracingCB->UpdateBuffer(&rayCameraMatrix, sizeof(SRayCameraMatrix), 0, 1);

	
}

void CBindlessRayTracingTestStage::Init()
{
	CDeviceGraphicsCommandInterface* pCommandInterface = GetDeviceObjectFactory().GetCoreCommandList().GetGraphicsInterface();
	CreateVBAndIB();
	CreateAndBuildBLAS(pCommandInterface);
	CreateAndBuildTLAS(pCommandInterface);
	CreateUniformBuffer();
	bInit = true;
}

void CBindlessRayTracingTestStage::Execute(CTexture* rayTracingResultTexture)
{
	if (bInit)
	{
		CClearSurfacePass::Execute(rayTracingResultTexture, ColorF(0, 0, 0, 0));
		m_bindlessRayTracingRenderPass.SetTechnique(CShaderMan::s_shBindlessRayTracingTest, CCryNameTSCRC("BindlessRayTracingTestTech"), 0);
		m_bindlessRayTracingRenderPass.SetNeedBindless(true);
		m_bindlessRayTracingRenderPass.SetMaxPipelineRayRecursionDepth(2);
		m_bindlessRayTracingRenderPass.SetConstantBuffer(0, m_pRayTracingCB);
		m_bindlessRayTracingRenderPass.SetBuffer(1, m_pRtTopLevelAS->GetAccelerationStructureBuffer());
		m_bindlessRayTracingRenderPass.SetBuffer(2, &m_pBindlessIndexBuffer);
		m_bindlessRayTracingRenderPass.SetOutputUAV(3, rayTracingResultTexture);
		m_bindlessRayTracingRenderPass.PrepareResourcesForUse(GetDeviceObjectFactory().GetCoreCommandList());
		m_bindlessRayTracingRenderPass.DispatchRayTracing(GetDeviceObjectFactory().GetCoreCommandList(), rayTracingResultTexture->GetWidth(), rayTracingResultTexture->GetHeight());
	}
}