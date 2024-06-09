start_sources()
sources_platform(ALL)
add_sources("CryScaleform_Uber_0.cpp"
	PROJECTS CryScaleformGNM CryScaleformD3D11 CryScaleformD3D12 CryScaleformVulkan
	SOURCE_GROUP "Root"
		"Module.cpp"
		"Module.h"
		"ScaleformTypes.h"
		"ScaleformHelper.h"
		"FlashPlayerInstance.cpp"
		"FlashPlayerInstance.h"
		"SharedStates.cpp"
		"SharedStates.h"
		"SharedResources.cpp"
		"SharedResources.h"
		"GFileCryPak.cpp"
		"GFileCryPak.h"
		"GAllocatorCryMem.cpp"
		"GAllocatorCryMem.h"
		"GImeHelper.cpp"
		"GImeHelper.h"
		"GFxVideoSoundCrySound.cpp"
		"GFxVideoWrapper.cpp"
		"GFxVideoSoundCrySound.h"
		"GFxVideoWrapper.h"
)

sources_platform(ALL)
add_sources("CryScaleform_Renderer.cpp"
	PROJECTS CryScaleformGNM CryScaleformD3D11 CryScaleformD3D12 CryScaleformVulkan
	SOURCE_GROUP "Renderer"
		"Renderer/SFRenderCache.cpp"
		"Renderer/SFRenderCache.h"
		"Renderer/SFRenderer.cpp"
		"Renderer/SFRenderer.h"
		"Renderer/SFMesh.cpp"
		"Renderer/SFMesh.h"
		"Renderer/SFTechnique.cpp"
		"Renderer/SFTechnique.h"
		"Renderer/SFTexture.cpp"
		"Renderer/SFTexture.h"
		"Renderer/SFConfig.h"
		"Renderer/SFShaderDescs.cpp"
		"Renderer/SFShaderDescs.h"
)

sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS CryScaleform CryScaleformGNM CryScaleformD3D11 CryScaleformD3D12 CryScaleformVulkan
	SOURCE_GROUP "Root"
		"StdAfx.cpp"
		"StdAfx.h"
)

sources_platform(ALL)
add_sources("CryScaleformLoader.cpp"
	PROJECTS CryScaleform
	SOURCE_GROUP "Root"
		"ScaleformSelector.cpp"
		"ScaleformSelector.h"
)

end_sources()

function(orbis_compile_options)
	target_compile_options(${THIS_PROJECT} PRIVATE
		-Wno-unused-value
		-Wno-unknown-pragmas
		-Wno-multichar
		-Wno-switch
		-Wno-unused-function
		-Wno-unused-lambda-capture
		-Wno-c++11-narrowing
		-Wno-format
		-Wno-empty-body
	)
endfunction()

macro(setup_CryScaleformModule)
	target_include_directories(${THIS_PROJECT} PRIVATE  ${extra_includes})
	target_include_directories(${THIS_PROJECT} PRIVATE "${SDK_DIR}")
	target_include_directories(${THIS_PROJECT} PRIVATE "${SDK_DIR}/Scaleform4/Include")
	target_include_directories(${THIS_PROJECT} PRIVATE "${SDK_DIR}/Scaleform4/Src")
	target_include_directories(${THIS_PROJECT} PRIVATE "${CRYENGINE_DIR}/Code/CryEngine/RenderDll")
	target_include_directories(${THIS_PROJECT} PRIVATE "${CRYENGINE_DIR}/Code/CryEngine/RenderDll/Common")
	target_include_directories(${THIS_PROJECT} PRIVATE "${CRYENGINE_DIR}/Code/CryEngine/RenderDll/XRenderD3D9")
	set(SCALEFORM_LIB_FOLDER "${SDK_DIR}/Scaleform4/Lib")
	set(SCALEFORM_LIBS AIR_SCU AS2_SCU AS3_SCU Expat Video GFx)
	if (WINDOWS)
		list(APPEND extra_libs winmm imm32 lz4)
		list(APPEND SCALEFORM_LIBS IME)
		set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/win64" )
		set(THIRD_PARTY_LIBS LzmaLib libcurl expat pcre freetype libjpeg libpng zlib)
	elseif(DURANGO)
		set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/durango" )
		set(THIRD_PARTY_LIBS LzmaLib libcurl expat pcre freetype jpeg libpng zlib legacy_stdio_definitions legacy_stdio_wide_specifiers)
	elseif(ORBIS)
		set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/orbis")
		set(THIRD_PARTY_LIBS curl expat freetype jpeg LzmaLib pcre pcreposix png z)
		orbis_compile_options()
	endif()
	
	target_compile_definitions(${THIS_PROJECT} PRIVATE -DINCLUDE_SCALEFORM_SDK $<$<CONFIG:Debug>:SF_BUILD_DEBUG> $<$<CONFIG:Profile>:SF_BUILD_RELEASE> $<$<CONFIG:Release>:SF_BUILD_SHIPPING> ${extra_defines})

	make_library(SCALEFORM_LIBS "${SCALEFORM_LIB_FOLDER}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Profile>:Release>$<$<CONFIG:Release>:Shipping>/")
	make_library(THIRD_PARTY_LIBS "${SCALEFORM_LIB_FOLDER}/")

	target_link_libraries(${THIS_PROJECT} PRIVATE ${SCALEFORM_LIBS})
	target_link_libraries(${THIS_PROJECT} PRIVATE ${THIRD_PARTY_LIBS})
	target_link_libraries(${THIS_PROJECT} PRIVATE ${extra_libs})
endmacro()

if (OPTION_SCALEFORM4 AND (EXISTS ${SDK_DIR}/Scaleform4))
	CryEngineModule(CryScaleform PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
	target_compile_definitions(${THIS_PROJECT} PRIVATE -DSCALEFORM4_LOADER)
	if(ORBIS)
		orbis_compile_options()
	endif()

	if (WINDOWS)
		if (RENDERER_DX11)
			SET(extra_defines -DCRY_RENDERER_DIRECT3D=110)
			SET(extra_libs CryRenderD3D11 dxguid)
			SET(extra_includes)
			CryEngineModule(CryScaleformD3D11 FORCE_SHARED_WIN PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
			setup_CryScaleformModule()
		else()
			CryFileContainer(CryScaleformD3D11 SOLUTION_FOLDER "CryEngine")
		endif()

		if (RENDERER_DX12)
			SET(extra_defines -DCRY_RENDERER_DIRECT3D=120)
			SET(extra_libs CryRenderD3D12 d3d12 dxgi)
			SET(extra_includes)
			CryEngineModule(CryScaleformD3D12 FORCE_SHARED_WIN PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
			setup_CryScaleformModule()
		else()
			CryFileContainer(CryScaleformD3D12 SOLUTION_FOLDER "CryEngine")
		endif()

		if (RENDERER_VULKAN)
			SET(extra_defines -DCRY_RENDERER_VULKAN=10 -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS)
			SET(extra_libs CryRenderVulkan spirv-cross)
			SET(extra_includes "${SDK_DIR}/VulkanSDK/Include")
			CryEngineModule(CryScaleformVulkan FORCE_SHARED_WIN PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
			setup_CryScaleformModule()
		else()
			CryFileContainer(CryScaleformVulkan SOLUTION_FOLDER "CryEngine")
		endif()

	elseif(DURANGO)
		if (RENDERER_DX11)
			SET(extra_defines -DCRY_RENDERER_DIRECT3D=111 -DSF_USE_STD11_THREADS)
			SET(extra_libs CryRenderD3D11 lz4 PIXEvt)
			SET(extra_includes)
			CryEngineModule(CryScaleformD3D11 FORCE_SHARED_WIN PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
			setup_CryScaleformModule()
		else()
			CryFileContainer(CryScaleformD3D11 SOLUTION_FOLDER "CryEngine")
		endif()
		
		if (RENDERER_DX12)
			SET(extra_defines -DCRY_RENDERER_DIRECT3D=120 -DSF_USE_STD11_THREADS)
			SET(extra_libs CryRenderD3D12 lz4 PIXEvt)
			SET(extra_includes)
			CryEngineModule(CryScaleformD3D12 FORCE_SHARED_WIN PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
			setup_CryScaleformModule()
		else()
			CryFileContainer(CryScaleformD3D12 SOLUTION_FOLDER "CryEngine")
		endif()

	elseif(ORBIS)
		SET(extra_defines -DCRY_RENDERER_GNM=40 -DSF_FLOATING_POINT_FAST -DGNM_FEATURE_VALIDATION=$<BOOL:${GNM_VALIDATION}>)
		SET(extra_libs CryRenderGNM SceGnf)
		SET(extra_includes)
		CryEngineModule(CryScaleformGNM PCH "StdAfx.cpp" SOLUTION_FOLDER "CryEngine")
		setup_CryScaleformModule()
	endif()
endif()

CryInterfaceProject(CryScaleformInterface)

if (OPTION_SCALEFORM3)
	target_compile_definitions(${THIS_PROJECT} INTERFACE -DCRY_FEATURE_SCALEFORM_HELPER)
	if (EXISTS ${SDK_DIR}/Scaleform)
		target_include_directories(${THIS_PROJECT} INTERFACE "${SDK_DIR}/Scaleform/Include" )
		target_compile_definitions(${THIS_PROJECT} INTERFACE -DINCLUDE_SCALEFORM_SDK -DINCLUDE_SCALEFORM3_SDK $<$<CONFIG:Debug>:GFC_BUILD_DEBUG>)
		set(SCALEFORM_LIB_FOLDER "${SDK_DIR}/Scaleform/Lib")
		set(SCALEFORM_HAS_SHIPPING_LIB TRUE)
		if (WINDOWS)
			set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/${MSVC_LIB_PREFIX}/Win64" )
			set(SCALEFORM_LIBS libgfx)
		elseif(DURANGO)
			set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/${MSVC_LIB_PREFIX}/Durango" )
			set(SCALEFORM_LIBS legacy_stdio_definitions libgfx)
		elseif(LINUX)
			set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/linux")
			set(SCALEFORM_HAS_SHIPPING_LIB FALSE)
			set(SCALEFORM_LIBS gfx jpeg png16)
		elseif(ORBIS)
			set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/ORBIS")
			set(SCALEFORM_LIBS gfx gfx_video)
		elseif(ANDROID)
			if (EXISTS "${SCALEFORM_LIB_FOLDER}/android-${CMAKE_ANDROID_ARCH_ABI}")
				set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/android-${CMAKE_ANDROID_ARCH_ABI}")
				set(SCALEFORM_HAS_SHIPPING_LIB FALSE)
				set(SCALEFORM_LIBS gfx)
			else()
				message(FATAL_ERROR "Scaleform libraries not found for Android ABI ${CMAKE_ANDROID_ARCH_ABI}. You need to disable Scaleform.")
			endif()
		elseif(APPLE)
			set(SCALEFORM_LIB_FOLDER "${SCALEFORM_LIB_FOLDER}/mac")
			set(SCALEFORM_HAS_SHIPPING_LIB FALSE)
			set(SCALEFORM_LIBS gfx jpeg png16)
		endif()

		if(SCALEFORM_HAS_SHIPPING_LIB)
			set(SCALEFORM_RELEASE_CONFIG Shipping)
			target_compile_definitions(${THIS_PROJECT} INTERFACE "$<$<CONFIG:Release>:GFC_BUILD_SHIPPING>")
		else()
			set(SCALEFORM_RELEASE_CONFIG Release)
		endif()

		if(APPLE OR LINUX)
			target_link_directories(${THIS_PROJECT} INTERFACE "${SCALEFORM_LIB_FOLDER}/")
		endif()
		target_link_directories(${THIS_PROJECT} INTERFACE "${SCALEFORM_LIB_FOLDER}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Profile>:Release>$<$<CONFIG:Release>:${SCALEFORM_RELEASE_CONFIG}>/")
		target_link_libraries(${THIS_PROJECT} INTERFACE ${SCALEFORM_LIBS})
	endif()
endif()

if (OPTION_SCALEFORM4)
	if (EXISTS ${SDK_DIR}/Scaleform4)
		target_compile_definitions(${THIS_PROJECT} INTERFACE -DINCLUDE_SCALEFORM_SDK $<$<CONFIG:Debug>:GFC_BUILD_DEBUG>)
	endif()
endif()