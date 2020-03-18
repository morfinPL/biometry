include(../../cmake/tests.cmake)
project(FeatureExtraction)

set(LIBS
	opencv_core
)

set(HEADERS
	include/FeatureExtraction.h
)

set(SOURCES
	src/FeatureExtraction.cpp
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)
link_directories(${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT_NAME} ${LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs)