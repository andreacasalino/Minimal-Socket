set(PROJECT_SHORTNAME "Utils")

MakeLibrary(${PROJECT_SHORTNAME} include)

if(COMPILE_ASYNCH)
	target_link_libraries(${PROJECT_SHORTNAME}
	  PUBLIC
		Asynch-Cross-Socket
	)
else()
	target_link_libraries(${PROJECT_SHORTNAME}
	  PUBLIC
		Synch-Cross-Socket
	)
endif()

if(COMPILE_TYPED)
	target_link_libraries(${PROJECT_SHORTNAME}
	  PUBLIC
		Typed-Cross-Socket
	)
endif()
