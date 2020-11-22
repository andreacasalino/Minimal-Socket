##### sample 06 #####

	add_executable(
		Sample06_Client
		Client.cpp
	)
	target_link_libraries( Sample06_Client
		PUBLIC
			Utils
	)	
	
	add_executable(
		Sample06_Server
		Server.cpp
	)
	find_package( Threads )
	target_link_libraries( Sample06_Server
		PUBLIC
			Utils
			${CMAKE_THREAD_LIBS_INIT}
	)	
	
	add_executable(
		Sample06__Launcher
		Launcher.cpp
	)
	target_link_libraries( Sample06__Launcher
		PUBLIC
			Utils
	)	
	add_dependencies(Sample06__Launcher Sample06_Client Sample06_Server)
