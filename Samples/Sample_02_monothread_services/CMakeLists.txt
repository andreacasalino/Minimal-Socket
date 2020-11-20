##### sample 02 #####

	add_executable(
		Sample02_Client_A
		Client_A.cpp
	)
	target_link_libraries( Sample02_Client_A
		PUBLIC
			SCK::Cross-Socket
	)	

	add_executable(
		Sample02_Client_B
		Client_B.cpp
	)
	target_link_libraries( Sample02_Client_B
		PUBLIC
			SCK::Cross-Socket
	)	
	
	add_executable(
		Sample02_Server
		Server.cpp
	)
	target_link_libraries( Sample02_Server
		PUBLIC
			SCK::Cross-Socket
	)		

	add_executable(
		Sample02__Launcher
		Launcher.cpp
	)
	add_dependencies(Sample02__Launcher Sample02_Client_A Sample02_Client_B Sample02_Server)
