function(add_tests PROJECT_NAME TESTS)
    set(PROJECT ${PROJECT_NAME})
	set(PROJECT_NAME ${PROJECT_NAME}_tests)
	project(${PROJECT_NAME})
	if(CI)
		add_definitions(-DCI)
	endif(CI)
	set(LIBS ${LIBS} Catch2::Catch2)
	set(LIBS ${LIBS} ${PROJECT})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
	link_directories(${CMAKE_BINARY_DIR}/bin)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tests)
	add_executable(${PROJECT_NAME} ${TESTS})
	set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)
	target_link_libraries(${PROJECT_NAME} ${LIBS})
	set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
	add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endfunction(add_tests)