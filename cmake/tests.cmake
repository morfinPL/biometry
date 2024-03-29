function(add_tests PROJECT_NAME TESTS)
    set(PROJECT ${PROJECT_NAME})
	project(${PROJECT_NAME}_tests)
	set(LIBS ${LIBS} Catch2::Catch2WithMain)
	set(LIBS ${LIBS} ${PROJECT})
	add_executable(${PROJECT_NAME} ${TESTS})
    set_output_dirs(${PROJECT_NAME})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
	set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "tests")
	add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endfunction(add_tests)
