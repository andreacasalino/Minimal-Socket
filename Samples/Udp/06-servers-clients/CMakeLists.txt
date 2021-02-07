add_executable(Sample06_Client Client.cpp)
target_link_libraries( Sample06_Client Sample-Utils)	

add_executable(Sample06_Server Server.cpp)
target_link_libraries( Sample06_Server Sample-Utils)	

add_executable(Sample06Launcher Launcher.cpp)
target_link_libraries(Sample06Launcher)
add_dependencies(Sample06Launcher Sample06_Server Sample06_Client)
