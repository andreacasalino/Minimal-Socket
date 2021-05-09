if (WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)
add_subdirectory(GoogleTest/)

add_subdirectory(UtilsTest)

function(MakeTest NAME)
	add_executable(${NAME} ${NAME}.cpp)

	target_link_libraries(${NAME}
	PUBLIC
		Utils
		UtilsTest
	)
		
	install(TARGETS ${NAME})
endfunction()

MakeTest(Test01-tcp-synch)

MakeTest(Test02-tcp-asynch)

MakeTest(Test01-udp-synch)

MakeTest(Test02-udp-asynch)
