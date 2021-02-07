add_executable(Sample02_Client Client.cpp)
target_link_libraries( Sample02_Client Sample-Utils)	

add_executable(Sample02_Server Server.cpp)
target_link_libraries( Sample02_Server Sample-Utils)	

add_executable(Sample02Launcher Launcher.cpp)
target_link_libraries(Sample02Launcher)
add_dependencies(Sample02Launcher Sample02_Server Sample02_Client)
