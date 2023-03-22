function(set_output_dirs TARGET_NAME)
    foreach(CONFIGURATION ${CMAKE_CONFIGURATION_TYPES})
        set(OUTPUT_BIN_PATH ${CMAKE_BINARY_DIR}/bin/${CONFIGURATION})
        set(OUTPUT_LIB_PATH ${CMAKE_BINARY_DIR}/lib/${CONFIGURATION})
        string(TOUPPER ${CONFIGURATION} CONFIGURATION_TO_UPPER)
        set_target_properties(${TARGET_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY_${CONFIGURATION_TO_UPPER} ${OUTPUT_BIN_PATH}
            RUNTIME_OUTPUT_DIRECTORY_${CONFIGURATION_TO_UPPER} ${OUTPUT_BIN_PATH}
            ARCHIVE_OUTPUT_DIRECTORY_${CONFIGURATION_TO_UPPER} ${OUTPUT_LIB_PATH}
            PDB_OUTPUT_DIRECTORY_${CONFIGURATION_TO_UPPER} ${OUTPUT_BIN_PATH}
            COMPILE_PDB_OUTPUT_DIRECTORY_${CONFIGURATION_TO_UPPER} ${OUTPUT_LIB_PATH}
        )
    endforeach()
endfunction(set_output_dirs)

function(setup_target_install_phase TARGET_NAME)
    foreach(CONFIGURATION ${CMAKE_CONFIGURATION_TYPES})
        if(CMAKE_BUILD_TYPE STREQUAL CONFIGURATION)
            set(INSTALL_BIN_PATH bin/${CONFIGURATION})
            set(INSTALL_LIB_PATH lib/${CONFIGURATION})
            install(TARGETS ${TARGET_NAME}
                EXPORT ${LIBRARY_NAME}
                CONFIGURATIONS ${CONFIGURATION}
                LIBRARY DESTINATION ${INSTALL_BIN_PATH}
                RUNTIME DESTINATION ${INSTALL_BIN_PATH}
                ARCHIVE DESTINATION ${INSTALL_LIB_PATH}
                BUNDLE DESTINATION ${INSTALL_BIN_PATH}
                PUBLIC_HEADER DESTINATION include/${TARGET_NAME}/${LIBRARY_NAME}/${TARGET_NAME})
        endif(CMAKE_BUILD_TYPE STREQUAL CONFIGURATION)
    endforeach()
endfunction(setup_target_install_phase)

function(create_dll_api_macro TARGET_NAME TARGET_TYPE)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${TARGET_TYPE})
    if(TARGET_TYPE STREQUAL "libs")
        STRING(TOUPPER ${TARGET_NAME} TARGET_NAME_UPPER)
        if(MSVC AND LINKAGE_TO_UPPER STREQUAL "SHARED")
            target_compile_definitions(${TARGET_NAME} PUBLIC ${TARGET_NAME_UPPER}_DLL_API=__declspec\(dllexport\))
        else(MSVC AND LINKAGE_TO_UPPER STREQUAL "SHARED")
            target_compile_definitions(${TARGET_NAME} PUBLIC ${TARGET_NAME_UPPER}_DLL_API=)
        endif(MSVC AND LINKAGE_TO_UPPER STREQUAL "SHARED")
    endif(TARGET_TYPE STREQUAL "libs")
endfunction(create_dll_api_macro)

function(add_properly_linked_library LIB_NAME SOURCES HEADERS)
    string(TOUPPER ${LINKAGE} LINKAGE_TO_UPPER)
    add_library(${LIB_NAME} ${LINKAGE_TO_UPPER} ${SOURCES} ${HEADERS})
    set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
    target_include_directories(${LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${LIB_NAME}>)
    target_link_directories(${LIB_NAME} PRIVATE ${CMAKE_BINARY_DIR}/bin)
	target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set_output_dirs(${LIB_NAME})
    create_dll_api_macro(${LIB_NAME} "libs")
    setup_target_install_phase(${LIB_NAME})
endfunction(add_properly_linked_library)

function(add_app APP_NAME SOURCES)
    add_executable(${APP_NAME} ${SOURCES})
    target_link_directories(${APP_NAME} PRIVATE ${CMAKE_BINARY_DIR}/bin)
    target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set_output_dirs(${APP_NAME})
    create_dll_api_macro(${APP_NAME} "apps")
    setup_target_install_phase(${APP_NAME})
endfunction(add_app)
