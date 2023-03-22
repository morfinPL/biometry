include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(LIBS
	AudioFile
	Configuration
	IO
	SoundProcessing
)

set(3RD_PARTY_LIBS
	opencv_core
)

set(SOURCES
	src/main.cpp
)

add_app(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${3RD_PARTY_LIBS})
