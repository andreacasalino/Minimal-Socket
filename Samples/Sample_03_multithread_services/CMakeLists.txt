##### sample 03 #####

	add_executable(
		Sample03_Client_A
		Client_A_slow.cpp
	)
	target_link_libraries( Sample03_Client_A
		PUBLIC
			SSK::Stream-Socket
	)	

	add_executable(
		Sample03_Client_B
		Client_B_fast.cpp
	)
	target_link_libraries( Sample03_Client_B
		PUBLIC
			SSK::Stream-Socket
	)	
	
	add_executable(
		Sample03_Server
		Server.cpp
	)
	find_package( Threads )
	target_link_libraries( Sample03_Server
		PUBLIC
			SSK::Stream-Socket
			${CMAKE_THREAD_LIBS_INIT}
	)	
	
	add_executable(
		Sample03__Launcher
		Launcher.cpp
	)
	add_dependencies(Sample03__Launcher Sample03_Client_A Sample03_Client_B Sample03_Server)