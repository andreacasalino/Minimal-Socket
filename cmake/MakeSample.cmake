function(MakeSample NAME)
	add_executable(${NAME} ${NAME}.cpp)

	target_link_libraries(${NAME}
	PUBLIC
		Utils
	)	
endfunction()

# function(MakeInstallableSample NAME)
	# MakeSample(${NAME})
	
	# install(TARGETS ${NAME})
# endfunction()
