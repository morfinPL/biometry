include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)
project(Configuration)

set(HEADERS
	include/Configuration.h
)

set(SOURCES
	src/Configuration.cpp
)

set(TESTS
	test/Configuration_tests.cpp
)

add_shared_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES} ${HEADERS})
add_tests(${PROJECT_NAME} "${TESTS}")