set(PROJECT_SHORTNAME "Asynch-Cross-Socket")

MakeLibrary(${PROJECT_SHORTNAME} include)

target_link_libraries(${PROJECT_SHORTNAME}
PUBLIC
	Synch-Cross-Socket    
)

target_compile_definitions(${PROJECT_SHORTNAME} 
PUBLIC 
	ASYNCH_ENABLED
)
