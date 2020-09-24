include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)
project(AudioFile)

set(HEADERS
	include/AudioFile.h
)

set(SOURCES
	src/AudioFile.cpp
)

set(TESTS
	test/AudioFile_tests.cpp
)

add_shared_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES} ${HEADERS} ${LIBS})
add_tests(${PROJECT_NAME} "${TESTS}")