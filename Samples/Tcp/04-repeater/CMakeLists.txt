##### sample 04 #####

	add_executable(
		Sample04_Client
		Client.cpp
	)
	target_link_libraries( Sample04_Client
		PUBLIC
			Utils
	)	

	add_executable(
		Sample04_Repeater
		Repeater.cpp
	)
	target_link_libraries( Sample04_Repeater
		PUBLIC
			Utils
	)	
	
	add_executable(
		Sample04_Server
		Server.cpp
	)
	find_package( Threads )
	target_link_libraries( Sample04_Server
		PUBLIC
			Utils
	)	

	add_executable(
		Sample04__Launcher
		Launcher.cpp
	)	
	target_link_libraries( Sample04__Launcher
		PUBLIC
			Utils
	)	
	add_dependencies(Sample04__Launcher Sample04_Client Sample04_Repeater Sample04_Server)
