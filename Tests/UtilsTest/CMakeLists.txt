set(PROJECT_SHORTNAME "UtilsTest")

MakeLibrary(${PROJECT_SHORTNAME} include)

target_link_libraries(${PROJECT_SHORTNAME}
  PUBLIC
	Synch-Cross-Socket
)
