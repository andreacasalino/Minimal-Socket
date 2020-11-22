##### sample 03 #####

	add_executable(
		Sample03_Client
		Client.cpp
	)
	target_link_libraries( Sample03_Client
		PUBLIC
			Utils
	)	
	
	add_executable(
		Sample03_Server
		Server.cpp
	)
	find_package( Threads )
	target_link_libraries( Sample03_Server
		PUBLIC
			Utils
			${CMAKE_THREAD_LIBS_INIT}
	)	
	
	add_executable(
		Sample03__Launcher
		Launcher.cpp
	)
	target_link_libraries( Sample03__Launcher
		PUBLIC
			Utils
	)	
	add_dependencies(Sample03__Launcher Sample03_Client Sample03_Server)
