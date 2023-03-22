include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


set(HEADERS
	include/Configuration.h
)

set(SOURCES
	src/Configuration.cpp
)

set(TESTS
	test/Configuration_tests.cpp
)

add_properly_linked_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_tests(${PROJECT_NAME} "${TESTS}")
