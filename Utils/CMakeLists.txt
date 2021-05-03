set(PROJECT_SHORTNAME "Utils")

MakeLibrary(${PROJECT_SHORTNAME} include)

if(COMPILE_ASYNCH)
	target_link_libraries(${PROJECT_SHORTNAME}
	  PUBLIC
		Asynch-Cross-Socket
	)
	target_compile_definitions(${PROJECT_SHORTNAME} PUBLIC ASYNCH_ENABLED)
else()
	target_link_libraries(${PROJECT_SHORTNAME}
	  PUBLIC
		Synch-Cross-Socket
	)
endif()
