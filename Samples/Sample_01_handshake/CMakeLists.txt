##### sample 01 #####

	add_executable(
		Sample01_Client
		Client.cpp
	)
	target_link_libraries( Sample01_Client
		PUBLIC
			SSK::Stream-Socket
	)	

	add_executable(
		Sample01_Server
		Server.cpp
	)
	target_link_libraries( Sample01_Server
		PUBLIC
			SSK::Stream-Socket
	)	

	add_executable(
		Sample01__Launcher
		Launcher.cpp
	)
	add_dependencies(Sample01__Launcher Sample01_Server Sample01_Client)