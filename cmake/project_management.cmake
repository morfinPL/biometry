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

function(add_shared_library LIB_NAME DIR SOURCES HEADERS)
    list(LENGTH ARGN OPTIONAL_ARGUMENTS_COUNT)
    add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})
    include_directories(
	    ${DIR}/include
	    ${DIR}/src
    )
    link_directories(${CMAKE_BINARY_DIR}/bin)
    if(OPTIONAL_ARGUMENTS_COUNT GREATER 0)
        SET(LIBS ${ARGN})
        target_link_libraries(${PROJECT_NAME} ${LIBS})
    endif()
    set_target_properties(${LIB_NAME} PROPERTIES FOLDER "libs")
    set_output_dirs(${LIB_NAME})
    STRING(TOUPPER ${LIB_NAME} LIB_NAME_UPPER)
    if(MSVC)
        target_compile_definitions(${LIB_NAME} PUBLIC ${LIB_NAME_UPPER}_DLL_API=__declspec\(dllexport\))
    else()
        target_compile_definitions(${LIB_NAME} PUBLIC ${LIB_NAME_UPPER}_DLL_API=)
    endif()
endfunction()