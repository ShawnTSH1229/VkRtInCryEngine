option(PLUGIN_SCALEFORM_SCHEMATYC "Build the Schematyc nodes plugin for Scaleform UI" ON)

if (NOT PLUGIN_SCALEFORM_SCHEMATYC)
    return()
endif()

if (OPTION_DEDICATED_SERVER)
	message(STATUS "Scaleform Schematyc plugin disabled when building dedicated server.")
    return()
endif()

start_sources()
sources_platform(ALL)
add_sources("CryScaleformSchematyc_uber_0.cpp"
	SOURCE_GROUP "Components"
		"Components/DynamicComponent.cpp"
		"Components/DynamicComponent.h"
		"Components/DynamicFunctions.cpp"
		"Components/DynamicFunctions.h"
		"Components/ElementFunctions.cpp"
		"Components/ElementFunctions.h"
		"Components/Signals.cpp"
		"Components/Signals.h"
	SOURCE_GROUP "Env"
		"Env/DynamicRegistry.h"
		"Env/DynamicRegistry.cpp"
		"Env/EnvTypes.h"
		"Env/FlashRegistry.h"
		"Env/FlashRegistry.cpp"
		"Env/FlashTypes.h"
	SOURCE_GROUP "Env\\\\Elements"
		"Env/Elements/EnvDynamicComponent.h"
		"Env/Elements/EnvDynamicFunction.h"
		"Env/Elements/EnvDynamicSignal.h"
		"Env/Elements/EnvFlashComponent.h"
	SOURCE_GROUP "Env\\\\Utils"
		"Env/Utils/FlashUtils.h"
		"Env/Utils/Utils.h"
	SOURCE_GROUP "Root"
		"FlashModule.cpp"
		"FlashModule.h"
		"Plugin.cpp"
		"Plugin.h"
		"resource.h"
		"StdAfx.cpp"
		"StdAfx.h"
)

add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"StdAfx.cpp"
)

end_sources()

CryEngineModule(CryScaleformSchematyc PCH "StdAfx.cpp" SOLUTION_FOLDER "CryPlugins")

if(WINDOWS)
	target_compile_options(${THIS_PROJECT} PRIVATE /Zc:inline-)
endif()

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
PUBLIC
	.
)

if (ORBIS OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${THIS_PROJECT} PRIVATE
		-Wno-reorder
		-Wno-unknown-pragmas
		-Wno-parentheses
		-Wno-switch
		-Wno-format
		-Wno-char-subscripts
		-Wno-empty-body
		-Wno-multichar
		-Wno-unused-value
		-Wno-null-conversion
		-Wno-unused-value
		-Wno-unused-private-field
		-Wno-unused-function
		-Wno-c++11-narrowing
		-Wno-dynamic-class-memaccess
		-Wno-sometimes-uninitialized
	)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${THIS_PROJECT} PRIVATE
		-Wno-unused-function
		-Wno-unused-value
		-Wno-sign-compare
		-Wno-unused-but-set-variable
		-Wno-misleading-indentation
		-Wno-unknown-pragmas
		-Wno-maybe-uninitialized
		-Wno-array-bounds
		-Wno-unused-result
		-Wno-strict-aliasing
		-Wno-multichar
		-Wno-char-subscripts
		-Wno-conversion-null
		-Wno-reorder
		-Wno-parentheses
		-Wno-format
		-Wno-switch
	)
endif()