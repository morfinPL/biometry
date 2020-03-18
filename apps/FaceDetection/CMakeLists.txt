set(PROJECT_NAME FaceDetection)
project(${PROJECT_NAME})

set(LIBS ${LIBS} opencv_core)
set(LIBS ${LIBS} opencv_highgui)
set(LIBS ${LIBS} Configuration)
set(LIBS ${LIBS} IO)
set(LIBS ${LIBS} FeatureExtraction)
set(LIBS ${LIBS} Classification)

set(SOURCES
	src/main.cpp
)

link_directories(${CMAKE_BINARY_DIR}/bin)
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/Configuration/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/IO/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/FeatureExtraction/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/Classification/include
)
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/apps)
target_link_libraries(${PROJECT_NAME} ${LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")