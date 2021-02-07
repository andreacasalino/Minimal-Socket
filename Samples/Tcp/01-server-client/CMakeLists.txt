add_executable(Sample01_Client Client.cpp)
target_link_libraries( Sample01_Client Sample-Utils)	

add_executable(Sample01_Server Server.cpp)
target_link_libraries( Sample01_Server Sample-Utils)	

add_executable(Sample01Launcher Launcher.cpp)
target_link_libraries(Sample01Launcher)
add_dependencies(Sample01Launcher Sample01_Server Sample01_Client)
