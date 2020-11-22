##### sample 05 #####

	add_executable(
		Sample05_Client
		Client.cpp
	)
	target_link_libraries( Sample05_Client
		PUBLIC
			Utils
	)	

	add_executable(
		Sample05_Server
		Server.cpp
	)
	target_link_libraries( Sample05_Server
		PUBLIC
			Utils
	)	

	add_executable(
		Sample05__Launcher
		Launcher.cpp
	)
	target_link_libraries( Sample05__Launcher
		PUBLIC
			Utils
	)
	add_dependencies(Sample05__Launcher Sample05_Server Sample05_Client)
