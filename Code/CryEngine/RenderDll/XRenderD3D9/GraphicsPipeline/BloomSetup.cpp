#include "BloomSetup.h"


CBloomSetupStage::~CBloomSetupStage()
{
	for (int32 Index = 0; Index < 2; Index++)
	{
		m_tileBloomMaskBuffer[Index].Release();
		m_tileIndexBuffer[Index].Release();
		m_dispatchIndirectCount[Index].Release();
	}
}

void CBloomSetupStage::InitBuffer()
{

	for (int32 Index = 0; Index < 2; Index++)
	{
		if (!m_tileBloomMaskBuffer[Index].IsAvailable())
		{
			m_tileBloomMaskBuffer[Index].Create(maskBufferSize[Index].x * maskBufferSize[Index].y, sizeof(uint32), DXGI_FORMAT_R32_UINT, CDeviceObjectFactory::BIND_SHADER_RESOURCE | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
			m_tileBloomMaskBuffer[Index].SetDebugName("m_tileBloomMaskBuffer" + Index);
		}

		if (!m_tileIndexBuffer[Index].IsAvailable())
		{
			m_tileIndexBuffer[Index].Create(maskBufferSize[Index].x * maskBufferSize[Index].y, sizeof(uint32), DXGI_FORMAT_UNKNOWN, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS, NULL);
			m_tileIndexBuffer[Index].SetDebugName("m_tileIndexBuffer" + Index);
		}

		if (!m_dispatchIndirectCount[Index].IsAvailable())
		{
			//USAGE_RAW	BIND_UNORDERED_ACCESS USAGE_INDIRECTARGS BIND_SHADER_RESOURCE No USAGE_STRUCTURED

			m_dispatchIndirectCount[Index].Create(3, sizeof(uint32), DXGI_FORMAT_R32_UINT, CDeviceObjectFactory::USAGE_STRUCTURED | CDeviceObjectFactory::BIND_UNORDERED_ACCESS | CDeviceObjectFactory::USAGE_INDIRECTARGS, NULL);
			//m_dispatchIndirectCount[Index].Create(3, sizeof(uint32), DXGI_FORMAT_R32_UINT, CDeviceObjectFactory::BIND_UNORDERED_ACCESS | CDeviceObjectFactory::USAGE_INDIRECTARGS, NULL);
			m_dispatchIndirectCount[Index].SetDebugName("m_dispatchIndirectCount" + Index);
		}
	}
}

void CBloomSetupStage::Resize(int renderWidth, int renderHeight)
{
	for (int32 Index = 0; Index < 2; Index++)
	{
		if (m_tileBloomMaskBuffer[Index].IsAvailable())
		{
			m_tileBloomMaskBuffer[Index].Release();
		}

		if (m_tileIndexBuffer[Index].IsAvailable())
		{
			m_tileIndexBuffer[Index].Release();
		}

		if (m_dispatchIndirectCount[Index].IsAvailable())
		{
			m_dispatchIndirectCount[Index].Release();
		}
	}

	bBufferInit = false;
}

void CBloomSetupStage::Execute(CTexture* pSrcRT, CTexture* pTiledBloomRT0, CTexture* pTiledBloomRT1)
{
	FUNCTION_PROFILER_RENDERER();
	PROFILE_LABEL_SCOPE("BLOOM_SETUP");

	CTexture* pTiledBloomRT[2] = { pTiledBloomRT0 ,pTiledBloomRT1 };

	if (pTiledBloomRT[1])
	{
		width = pTiledBloomRT[1]->GetWidth();
		height = pTiledBloomRT[1]->GetHeight();
	}
	else
	{
		return;
	}

	//m_tileBloomMaskBuffer: mask the tiles containing pixels whose luminance exceeds the threshold.


	//bloom setup pass: Select and output the pixels whose luminance exceeds the threshold and mask the tiles containing the selected pixels.
	//@input :		bloom texture size:			512 * 256
	//@output:		bloom mask buffer1:			64 * 32
	//@parameters:	dispatch size:				64 * 32

	textureSize = Vec4(static_cast<float>(width), static_cast<float>(height), 1.0f / static_cast<float>(width), 1.0f / static_cast<float>(height));
	maskBufferSize[0] = Vec4i(divideRoundUp(Vec2i(width, height), Vec2i(groupSizeX, groupSizeY)), 0, 0);
	maskBufferSize[1] = Vec4i(divideRoundUp(Vec2i(width, height), Vec2i(groupSizeX * 2, groupSizeY * 2)), 0, 0);
	Vec2i bloomSetupDispatchSize(maskBufferSize[0].x, maskBufferSize[0].y);


	//bloom tile infomation generate:1.Mask the tiles affected by the Gaussian blur. 2.Output the tile index and the masked tile count.
	
	//generation pass 1:
	//		@input:			bloom mask buffer1:			64 * 32
	//		@output:		bloom tile index buffer1 :	64 * 32
	//		@output:		dispatch indirect cound 1:	3
	//		@output:		bloom mask buffer2:			32 * 16
	//		@parameters:	dispatch size:				8 * 4
	
	//generation pass 2:	
	//		@input:			bloom mask buffer2:			32 * 16
	//		@output:		bloom tile index buffer2 :	32 * 16
	//		@output:		dispatch indirect cound 1:	3
	//		@parameters:	dispatch size:				4 * 2

	dispatchSizeIndexGen[0] = divideRoundUp(Vec2i(maskBufferSize[0].x, maskBufferSize[0].y), Vec2i(groupSizeX, groupSizeY));
	dispatchSizeIndexGen[1] = divideRoundUp(Vec2i(maskBufferSize[1].x, maskBufferSize[1].y), Vec2i(groupSizeX, groupSizeY));

	if (!bBufferInit)
	{
		InitBuffer();
		bBufferInit = true;
	}
	
	const ColorI nulls = { 0, 0, 0, 0 };

	const bool bAsynchronousCompute = CRenderer::CV_r_D3D12AsynchronousCompute & BIT((eStage_BloomSetup - eStage_FIRST_ASYNC_COMPUTE)) ? true : false;
	{
		m_passBloomSetup.SetTechnique(CShaderMan::s_shBloomSetup, CCryNameTSCRC("BloomSetup"), 0);
		m_passBloomSetup.SetOutputUAV(0, pTiledBloomRT[0]);
		m_passBloomSetup.SetOutputUAV(1, &m_tileBloomMaskBuffer[0]);
		m_passBloomSetup.SetTextureSamplerPair(0, pSrcRT, EDefaultSamplerStates::LinearClamp);

		m_parameters->TexSize = textureSize;
		m_parameters->BufferSize = maskBufferSize[0];
		m_passBloomSetup.SetConstantBuffer(0, m_parameters.GetDeviceConstantBuffer());

		m_passBloomSetup.SetDispatchSize(bloomSetupDispatchSize.x, bloomSetupDispatchSize.y, 1);
		m_passBloomSetup.PrepareResourcesForUse(GetDeviceObjectFactory().GetCoreCommandList());

		SScopedComputeCommandList computeCommandList(bAsynchronousCompute);
		m_passBloomSetup.Execute(computeCommandList);
	}

	for (int32 Index = 0; Index < 2; Index++)
	{
		m_passBloomTileIndexGen[Index]->SetTechnique(CShaderMan::s_shBloomSetup, CCryNameTSCRC("BloomTileIndexGen"), Index == 0 ? g_HWSR_MaskBit[HWSR_SAMPLE0] : 0);

		m_passBloomTileIndexGen[Index]->SetOutputUAV(0, &m_tileIndexBuffer[Index]);
		m_passBloomTileIndexGen[Index]->SetOutputUAV(1, &m_dispatchIndirectCount[Index]);

		if (Index == 0)
		{
			m_passBloomTileIndexGen[Index]->SetOutputUAV(2, &m_tileBloomMaskBuffer[1]);
		}

		m_passBloomTileIndexGen[Index]->SetBuffer(0, &m_tileBloomMaskBuffer[Index]);

		m_passBloomTileIndexGen[Index]->BeginConstantUpdate();
		m_passBloomTileIndexGen[Index]->SetConstant(CCryNameR("MaskBufferSize"), maskBufferSize[Index]);

		m_passBloomTileIndexGen[Index]->SetDispatchSize(dispatchSizeIndexGen[Index].x, dispatchSizeIndexGen[Index].y, 1);
		m_passBloomTileIndexGen[Index]->PrepareResourcesForUse(GetDeviceObjectFactory().GetCoreCommandList());

		SScopedComputeCommandList computeCommandList(bAsynchronousCompute);
		m_passBloomTileIndexGen[Index]->Execute(computeCommandList);
	}

	CClearSurfacePass::Execute(pTiledBloomRT[1], ColorF(0, 0, 0, 0));
	
	for (int32 IndexPass = 0; IndexPass < 2; IndexPass++)
	{
		const CCryNameTSCRC TechName[2] = { ("BloomFinal"),("BloomCombine") };
		for (int32 IndexAxis = 0; IndexAxis < 2; IndexAxis++)
		{
			m_passBloom[IndexPass][IndexAxis]->SetTechnique(CShaderMan::s_shBloomSetup, TechName[IndexPass], 0);
			m_passBloom[IndexPass][IndexAxis]->SetOutputUAV(0, pTiledBloomRT[1 - (IndexAxis + IndexPass * 2) % 2]);
			m_passBloom[IndexPass][IndexAxis]->SetOutputUAV(1, &m_tileIndexBuffer[IndexPass]);
			m_passBloom[IndexPass][IndexAxis]->SetTextureSamplerPair(0, pTiledBloomRT[(IndexAxis + IndexPass * 2) % 2], EDefaultSamplerStates::LinearClamp);
			m_passBloom[IndexPass][IndexAxis]->BeginConstantUpdate();

			Vec4 bloomParams;
			if (IndexAxis == 0)
			{
				bloomParams = Vec4(1.0f * textureSize.z * (IndexPass + 1), 0, 0, 0);
			}
			else
			{
				bloomParams = Vec4(0, 1.0f * textureSize.w * (IndexPass + 1), 0, 0);
			}
			
			m_passBloom[IndexPass][IndexAxis]->SetConstant(CCryNameR("bloomParams"), bloomParams);
			m_passBloom[IndexPass][IndexAxis]->SetConstant(CCryNameR("bloomTexSize"), textureSize);
			m_passBloom[IndexPass][IndexAxis]->SetDispatchIndirectArgs(&m_dispatchIndirectCount[IndexPass], 0);
			m_passBloom[IndexPass][IndexAxis]->PrepareResourcesForUse(GetDeviceObjectFactory().GetCoreCommandList());

			SScopedComputeCommandList computeCommandList(bAsynchronousCompute);
			m_passBloom[IndexPass][IndexAxis]->Execute(computeCommandList);
		}
	}
}

