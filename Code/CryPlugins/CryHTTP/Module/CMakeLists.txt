if(ORBIS)
	set(PLUGIN_HTTP_SCE TRUE)
elseif(TARGET curl)
	set(PLUGIN_HTTP_CURL TRUE)
elseif(TARGET libHttpClient)
	set(PLUGIN_HTTP_HC TRUE)
endif()

if ((PLUGIN_HTTP_CURL OR PLUGIN_HTTP_SCE OR PLUGIN_HTTP_HC) AND OPTION_ENGINE)
	option(PLUGIN_HTTP "Enable HTTP support" ON)
else()
	set(PLUGIN_HTTP OFF CACHE BOOL "Enable HTTP support" FORCE)
endif()
	
if (NOT PLUGIN_HTTP)
	return()
endif()

start_sources()
# -- cryhttp.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Interface"
		"../Interface/HttpRequest.h"
		"../Interface/HttpTypes.h"
		"../Interface/IHttpPlugin.h"
		"../Interface/IHttpRequestImpl.h"
	SOURCE_GROUP "Root"
		"resource.h"
		"StdAfx.cpp"
		"StdAfx.h"
)

if(PLUGIN_HTTP_CURL)
add_sources("CryHTTP_cURL_uber_0.cpp"
	SOURCE_GROUP "Implementation"
		"HttpImplementation_cURL.cpp"
		"HttpImplementation_cURL.h"
		"HttpRequestImpl_cURL.cpp"
		"HttpRequestImpl_cURL.h"
)

elseif(PLUGIN_HTTP_SCE)
add_sources("CryHTTP_sce_uber_0.cpp"
	SOURCE_GROUP "Implementation"
		"HttpImplementation_SCE.cpp"
		"HttpImplementation_SCE.h"
		"HttpRequestImpl_SCE.cpp"
		"HttpRequestImpl_SCE.h"
)

elseif(PLUGIN_HTTP_HC)
add_sources("CryHTTP_HC_uber_0.cpp"
	SOURCE_GROUP "Implementation"
		"HttpImplementation_HC.cpp"
		"HttpImplementation_HC.h"
		"HttpRequestImpl_HC.cpp"
		"HttpRequestImpl_HC.h"
)
endif()

end_sources()

CryEngineModule(CryHTTP PCH "StdAfx.cpp" SOLUTION_FOLDER "CryPlugins")

target_include_directories(${THIS_PROJECT}
PRIVATE 
	"${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
PUBLIC
	../Interface
)

if(PLUGIN_HTTP_HC)
	target_link_libraries( ${THIS_PROJECT} PRIVATE libHttpClient )
endif()
if(PLUGIN_HTTP_CURL)
	target_link_libraries( ${THIS_PROJECT} PRIVATE curl )
endif()

if(LINUX OR ORBIS)
	target_compile_options(${THIS_PROJECT} PRIVATE -w)
endif()
