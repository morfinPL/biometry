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

set(TESTS
	test/FeatureExtraction_tests.cpp
)

add_shared_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES} ${HEADERS} ${LIBS})
add_tests(${PROJECT_NAME} "${TESTS}")