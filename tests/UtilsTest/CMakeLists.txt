set(PROJECT_SHORTNAME "UtilsTest")

MakeLibrary(${PROJECT_SHORTNAME} include)

find_package(OpenMP)	
	
target_link_libraries(${PROJECT_SHORTNAME} PUBLIC
	MinimalSocket
	OpenMP::OpenMP_CXX
	Catch2::Catch2
	Catch2::Catch2WithMain
)
