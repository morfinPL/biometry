include(../../cmake/tests.cmake)
project(IO)

set(LIBS
	opencv_imgcodecs
	opencv_imgproc
)

set(HEADERS
	include/IO.h
)

set(SOURCES
	src/IO.cpp
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)
link_directories(${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT_NAME} ${LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs)