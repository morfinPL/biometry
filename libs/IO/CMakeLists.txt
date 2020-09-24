include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)
project(IO)

set(LIBS
	opencv_highgui
	opencv_imgcodecs
	opencv_imgproc
	AudioFile
)

set(HEADERS
	include/IO.h
)

set(SOURCES
	src/IO.cpp
)

set(TESTS
	test/IO_tests.cpp
)

add_shared_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES} ${HEADERS} ${LIBS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../AudioFile/include)
add_tests(${PROJECT_NAME} "${TESTS}")
target_link_libraries(${PROJECT_NAME}_tests opencv_core)