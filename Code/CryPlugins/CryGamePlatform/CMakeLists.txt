option(PLUGIN_GAMEPLATFORM "Enable CryGamePlatform plugin?" ON)

if (PLUGIN_GAMEPLATFORM)
    set(GamePlatformRoot "${CRYENGINE_DIR}/Code/CryPlugins/CryGamePlatform")
    add_subdirectory(Module/Core)
    add_subdirectory(Module/Discord)

    add_subdirectory(Module/Steam)

    set(PSN_MODULE_DIR "Module/PSN")
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${PSN_MODULE_DIR}")
        add_subdirectory(${PSN_MODULE_DIR})
    endif()

    set(XBOX_MODULE_DIR "Module/Xbox")
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${XBOX_MODULE_DIR}")
        add_subdirectory(${XBOX_MODULE_DIR})
    endif()
endif()
