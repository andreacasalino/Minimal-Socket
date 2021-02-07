##### sample 02 #####

	add_executable(
		Sample02_Client
		Client.cpp
	)
	target_link_libraries( Sample02_Client
		PUBLIC
			Utils
	)	
	
	add_executable(
		Sample02_Server
		Server.cpp
	)
	target_link_libraries( Sample02_Server
		PUBLIC
			Utils
	)		

	add_executable(
		Sample02__Launcher
		Launcher.cpp
	)
	target_link_libraries( Sample02__Launcher
		PUBLIC
			Utils
	)
	add_dependencies(Sample02__Launcher Sample02_Client Sample02_Server)
