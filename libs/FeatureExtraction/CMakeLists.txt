include(../../cmake/tests.cmake)
include(../../cmake/project_management.cmake)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


set(3RD_PARTY_LIBS
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

add_properly_linked_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${3RD_PARTY_LIBS})

add_tests(${PROJECT_NAME} "${TESTS}")

target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${3RD_PARTY_LIBS})
