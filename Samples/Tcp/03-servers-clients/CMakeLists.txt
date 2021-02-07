add_executable(Sample03_Client Client.cpp)
target_link_libraries( Sample03_Client Sample-Utils)	

add_executable(Sample03_Server Server.cpp)
target_link_libraries( Sample03_Server Sample-Utils)	

add_executable(Sample03Launcher Launcher.cpp)
target_link_libraries(Sample03Launcher)
add_dependencies(Sample03Launcher Sample03_Server Sample03_Client)
