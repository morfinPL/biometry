include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)
project(Classification)

set(HEADERS
	include/Classification.h
)

set(SOURCES
	src/Classification.cpp
)

set(TESTS
	test/Classification_tests.cpp
)

add_shared_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES} ${HEADERS} ${LIBS})
add_tests(${PROJECT_NAME} "${TESTS}")