add_executable(Sample01_Client Client.cpp)
target_link_libraries( Sample01_Client Cross-Socket)	

add_executable(Sample01_Server Server.cpp)
target_link_libraries( Sample01_Server Cross-Socket)	

add_executable(Sample01_Launcher Launcher.cpp)
target_link_libraries(Sample01_Launcher Cross-Socket)

add_dependencies(Sample01_Launcher Sample01_Server Sample01_Client)
