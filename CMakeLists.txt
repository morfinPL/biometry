cmake_minimum_required(VERSION 3.21.1)
include(cmake/coverage.cmake)
project(Biometry)

find_package(OpenCV 4.5.3 REQUIRED)
find_package(Catch2 2.13.7 REQUIRED)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

enable_testing()
add_subdirectory(libs)
add_subdirectory(apps)
SET(BUILD_ENABLE_COVERAGE OFF CACHE BOOL "")
setup_coverage()