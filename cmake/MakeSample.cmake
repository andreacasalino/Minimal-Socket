function(MakeSample NAME)
	add_executable(${NAME} ${NAME}.cpp)

	target_link_libraries(${NAME}
	PUBLIC
		Utils
	)	
	
	set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_INSTALL_PREFIX}/bin/")
	
	install(TARGETS ${NAME})
endfunction()

function(MakeLaunch NAME)
	add_executable(${NAME} ${NAME}.cpp)
	
	add_dependencies(${NAME} ${ARGV})
	
	install(TARGETS ${NAME})
endfunction()
