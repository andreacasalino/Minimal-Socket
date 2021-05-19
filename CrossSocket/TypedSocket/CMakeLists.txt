set(PROJECT_SHORTNAME "Typed-Cross-Socket")

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

target_compile_definitions(${PROJECT_SHORTNAME} 
PUBLIC 
	TYPED_ENABLED
)
