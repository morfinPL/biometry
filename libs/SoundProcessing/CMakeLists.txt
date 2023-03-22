include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


set(LIBS
	AudioFile
)

set(3RD_PARTY_LIBS
	opencv_core
	opencv_highgui
	opencv_imgproc
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

add_properly_linked_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${3RD_PARTY_LIBS})

add_tests(${PROJECT_NAME} "${TESTS}")
