include(${TOOLS_CMAKE_DIR}/modules/Steamworks.cmake)

if (TARGET Steamworks AND OPTION_ENGINE)
	option(PLUGIN_GAMEPLATFORM_STEAM "Enable GamePlatform integration plug-in for Steam" ON)
else()
	unset(PLUGIN_GAMEPLATFORM_STEAM CACHE)
endif()
	
if (NOT PLUGIN_GAMEPLATFORM_STEAM)
	return()
endif()

start_sources()

sources_platform(ALL)
add_sources("CryGamePlatform_Steam_uber_0.cpp"
	SOURCE_GROUP "Root"
		"resource.h"
		"StdAfx.cpp"
		"StdAfx.h"
)

add_sources("CryGamePlatform_Steam_uber_1.cpp"
	SOURCE_GROUP "Root"
		"SteamService.cpp"
		"SteamService.h"
		"SteamTypes.h"
	SOURCE_GROUP "Profile"
		"SteamStatistics.cpp"
		"SteamStatistics.h"
		"SteamUserIdentifier.h"
		"SteamAccount.cpp"
		"SteamAccount.h"
		"../../Module/Core/PlatformAccountManager.h"
		"../../Module/Core/PlatformAccountManager.inl"
		"SteamAchievement.cpp"
		"SteamAchievement.h"
		"SteamLeaderboards.cpp"
		"SteamLeaderboards.h"
	SOURCE_GROUP "Multiplayer"
		"SteamMatchmaking.cpp"
		"SteamMatchmaking.h"
		"SteamNetworking.cpp"
		"SteamNetworking.h"
		"SteamUserLobby.cpp"
		"SteamUserLobby.h"
		"SteamServer.cpp"
		"SteamServer.h"
	SOURCE_GROUP "RemoteStorage"
		"SteamRemoteFile.cpp"
		"SteamRemoteFile.h"
		"SteamRemoteStorage.cpp"
		"SteamRemoteStorage.h"
		"SteamSharedRemoteFile.cpp"
		"SteamSharedRemoteFile.h"
	SOURCE_GROUP "UserGeneratedContent"
		"UserGeneratedContent.cpp"
		"UserGeneratedContent.h"
		"UserGeneratedContentManager.cpp"
		"UserGeneratedContentManager.h"
)

add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"StdAfx.cpp"
)

end_sources()

CryEngineModule(CryGamePlatformSteam PCH "StdAfx.cpp" SOLUTION_FOLDER "CryPlugins/GamePlatform")

target_include_directories(${THIS_PROJECT}
	PUBLIC
		"../../Interface"
		"../../Module/Core"
	PRIVATE
		"${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
)
target_link_libraries(${THIS_PROJECT}
	PRIVATE
		Steamworks
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${THIS_PROJECT} PRIVATE
		-Wno-unused-variable
		-Wno-reorder
		-Wno-parentheses
		-Wno-unknown-pragmas
		-Wno-format
		-Wno-switch
		-Wno-multichar
		-Wno-unused-private-field
		-Wno-empty-body
		-Wno-unused-value
		-Wno-unused-function
	)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${THIS_PROJECT} PRIVATE
		-Wno-unused-function
		-Wno-unused-value
		-Wno-unused-variable
		-Wno-sign-compare
		-Wno-ignored-attributes
		-Wno-int-in-bool-context
		-Wno-unused-but-set-variable
		-Wno-misleading-indentation
		-Wno-strict-aliasing
		-Wno-maybe-uninitialized
		-Wno-unknown-pragmas
		-Wno-unused-const-variable
		-Wno-multichar
		-Wno-char-subscripts
		-Wno-parentheses
		-Wno-comment
		-Wno-address
		-Wno-format
		-Wno-switch
	)
endif()
