set(PROJECT_SHORTNAME "Typed-Cross-Socket")

MakeLibrary(${PROJECT_SHORTNAME} include)

target_link_libraries(${PROJECT_SHORTNAME}
PUBLIC
	Synch-Cross-Socket    
)
