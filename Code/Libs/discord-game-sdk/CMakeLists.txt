set(LIBROOT ${SDK_DIR}/discord-game-sdk)

if (EXISTS "${LIBROOT}")
	if (WIN32)
		set(DISCORD_GAME_SDK_DLL ${LIBROOT}/lib/x86_64/discord_game_sdk.dll)
		set(DISCORD_GAME_SDK_LIB ${LIBROOT}/lib/x86_64/discord_game_sdk.dll.lib)
	else()
		message(WARNING "Platform does not support Discord Game SDK")
		return()
	endif()

	if (EXISTS ${DISCORD_GAME_SDK_LIB} AND EXISTS ${DISCORD_GAME_SDK_DLL})
		deploy_runtime_files(${DISCORD_GAME_SDK_DLL})
	else()
		message(WARNING "Required libraries not found for Discord Game SDK")
		return()
	endif()

	start_sources()

	sources_platform(ALL)
	add_sources("NoUberFile"
		SOURCE_GROUP "Root"
			"${LIBROOT}/cpp/discord/achievement_manager.cpp"
			"${LIBROOT}/cpp/discord/achievement_manager.h"
			"${LIBROOT}/cpp/discord/activity_manager.cpp"
			"${LIBROOT}/cpp/discord/activity_manager.h"
			"${LIBROOT}/cpp/discord/application_manager.cpp"
			"${LIBROOT}/cpp/discord/application_manager.h"
			"${LIBROOT}/cpp/discord/core.cpp"
			"${LIBROOT}/cpp/discord/core.h"
			"${LIBROOT}/cpp/discord/discord.h"
			"${LIBROOT}/cpp/discord/event.h"
			"${LIBROOT}/cpp/discord/ffi.h"
			"${LIBROOT}/cpp/discord/image_manager.cpp"
			"${LIBROOT}/cpp/discord/image_manager.h"
			"${LIBROOT}/cpp/discord/lobby_manager.cpp"
			"${LIBROOT}/cpp/discord/lobby_manager.h"
			"${LIBROOT}/cpp/discord/network_manager.cpp"
			"${LIBROOT}/cpp/discord/network_manager.h"
			"${LIBROOT}/cpp/discord/overlay_manager.cpp"
			"${LIBROOT}/cpp/discord/overlay_manager.h"
			"${LIBROOT}/cpp/discord/relationship_manager.cpp"
			"${LIBROOT}/cpp/discord/relationship_manager.h"
			"${LIBROOT}/cpp/discord/storage_manager.cpp"
			"${LIBROOT}/cpp/discord/storage_manager.h"
			"${LIBROOT}/cpp/discord/store_manager.cpp"
			"${LIBROOT}/cpp/discord/store_manager.h"
			"${LIBROOT}/cpp/discord/types.cpp"
			"${LIBROOT}/cpp/discord/types.h"
			"${LIBROOT}/cpp/discord/user_manager.cpp"
			"${LIBROOT}/cpp/discord/user_manager.h"
			"${LIBROOT}/cpp/discord/voice_manager.cpp"
			"${LIBROOT}/cpp/discord/voice_manager.h"
	) # sources_platform(ALL)

	end_sources()

	CryEngineStaticModule(discord-game-sdk SOLUTION_FOLDER "Libs")

	target_include_directories(${THIS_PROJECT} PUBLIC "${LIBROOT}/cpp")
	target_link_libraries(${THIS_PROJECT} PRIVATE ${DISCORD_GAME_SDK_LIB})

	if (MSVC)
		# Disable C4530 (warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc)
		target_compile_options(${THIS_PROJECT} PRIVATE "/wd4530")
	endif()
endif()
