set(PROJECT_SHORTNAME "UtilsTest")

MakeLibrary(${PROJECT_SHORTNAME} include)

find_package(OpenMP)	
	
target_link_libraries(${PROJECT_SHORTNAME}
  PUBLIC
	Synch-Cross-Socket
	gtest
	OpenMP::OpenMP_CXX
)
