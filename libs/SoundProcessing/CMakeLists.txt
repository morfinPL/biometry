include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)
project(SoundProcessing)

set(LIBS
	opencv_core
	opencv_highgui
	opencv_imgproc
	AudioFile
)

set(HEADERS
	include/SoundProcessing.h
)

set(SOURCES
	src/SoundProcessing.cpp
)

set(TESTS
	test/SoundProcessing_tests.cpp
)

add_shared_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES} ${HEADERS} ${LIBS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../AudioFile/include)
add_tests(${PROJECT_NAME} "${TESTS}")