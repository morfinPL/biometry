cmake_minimum_required(VERSION 3.21.1)

include(cmake/install.cmake)
include(cmake/static_analysis.cmake)
include(cmake/coverage.cmake)
include(cmake/tests.cmake)
include(cmake/utils.cmake)

setup_system_specific_variables()

get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(LIBRARY_NAME ${LIBRARY_NAME} CACHE STRING "" FORCE)
project(${LIBRARY_NAME})

find_3rdparty_libraries("${3RDPARTY_LIBRARIES}" "${3RDPARTY_LIBRARIES_VERSIONS}")

if (NOT DEFINED LINKAGE)
    message(FATAL_ERROR "You must specify linakge type STATIC or SHARED!")
endif (NOT DEFINED LINKAGE)

set_property(CACHE LINKAGE PROPERTY STRINGS SHARED STATIC)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    find_package(OpenMP)
endif (CMAKE_SYSTEM_NAME STREQUAL Linux)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_definitions(__PRETTY_FUNCTION__=__FUNCTION__)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

enable_testing()

add_subdirectory(libs)
add_subdirectory(apps)

option(BUILD_ENABLE_COVERAGE "" OFF)
option(BUILD_ENABLE_CPD "" OFF)

if (BUILD_ENABLE_CPD)
    find_tool(cpd.bat)
    add_cpd_target()
endif (BUILD_ENABLE_CPD)

if (BUILD_ENABLE_COVERAGE)
    find_tool(OpenCppCoverage)
    add_coverage_target()
endif (BUILD_ENABLE_COVERAGE)

setup_install_phase()
