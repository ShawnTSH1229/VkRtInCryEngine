add_subdirectory("${CRYENGINE_DIR}/Code/Libs/discord-game-sdk" "${CMAKE_BINARY_DIR}/Code/Libs/discord-game-sdk")

if (TARGET discord-game-sdk AND OPTION_ENGINE)
	option(PLUGIN_GAMEPLATFORM_DISCORD "Enable GamePlatform integration plug-in for Discord" ON)
else()
	unset(PLUGIN_GAMEPLATFORM_DISCORD CACHE)
endif()

if (NOT PLUGIN_GAMEPLATFORM_DISCORD)
	return()
endif()

start_sources()

# -- discord.waf_files --
sources_platform(WINDOWS)
add_sources("CryGamePlatform_Discord_uber_0.cpp"
	SOURCE_GROUP "Root"
		"resource.h"
		"StdAfx.cpp"
		"StdAfx.h"
)

add_sources("CryGamePlatform_Discord_uber_1.cpp"
	SOURCE_GROUP "Root"
		"DiscordService.cpp"
		"DiscordService.h"
		"DiscordHelper.cpp"
		"DiscordHelper.h"
		"DiscordTypes.h"
	SOURCE_GROUP "Profile"
		"DiscordUserIdentifier.h"
		"DiscordAccount.cpp"
		"DiscordAccount.h"
		"../../Module/Core/PlatformAccountManager.h"
		"../../Module/Core/PlatformAccountManager.inl"
)

add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"StdAfx.cpp"
)

end_sources()

CryEngineModule(CryGamePlatformDiscord PCH "StdAfx.cpp" SOLUTION_FOLDER "CryPlugins/GamePlatform")

target_include_directories(${THIS_PROJECT}
	PUBLIC
		"$../../Interface"
	PRIVATE
		"${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
)
target_link_libraries(${THIS_PROJECT}
	PUBLIC
		CryGamePlatform
	PRIVATE
		discord-game-sdk
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
