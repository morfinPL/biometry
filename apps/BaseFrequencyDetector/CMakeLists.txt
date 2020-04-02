set(PROJECT_NAME BaseFrequencyDetector)
project(${PROJECT_NAME})
set(LIBS ${LIBS} Configuration)
set(LIBS ${LIBS} AudioFile)

set(SOURCES
	src/main.cpp
)

link_directories(${LINK_DIRECTORIES} ${CMAKE_BINARY_DIR}/bin)
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/Configuration/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/AudioFile/include
)
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/apps)
target_link_libraries(${PROJECT_NAME} ${LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")