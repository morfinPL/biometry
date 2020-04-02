set(PROJECT_NAME AudioFile)
project(${PROJECT_NAME})

set(HEADERS
	include/AudioFile.h
)

set(SOURCES
	src/AudioFile.cpp
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)
link_directories(${CMAKE_BINARY_DIR}/bin)
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")
target_link_libraries(${PROJECT_NAME} ${LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs)