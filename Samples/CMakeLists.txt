option(BUILD_SAMPLES_TCP "Tcp samples enabled (ON) or not (OFF)" ON)
if(BUILD_SAMPLES_TCP)
	add_subdirectory(Tcp)
endif()

option(BUILD_SAMPLES_UDP "Udp samples enabled (ON) or not (OFF)" ON)
if(BUILD_SAMPLES_UDP)
	add_subdirectory(Udp)
endif()
