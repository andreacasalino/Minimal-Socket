add_executable(Sample04_Client Client.cpp)
target_link_libraries( Sample04_Client Sample-Utils)	

add_executable(Sample04_Server Server.cpp)
target_link_libraries( Sample04_Server Sample-Utils)	

add_executable(Sample04_Repeater Repeater.cpp)
target_link_libraries( Sample04_Repeater Sample-Utils)

add_executable(Sample04Launcher Launcher.cpp)
target_link_libraries(Sample04Launcher)
add_dependencies(Sample04Launcher Sample04_Server Sample04_Client Sample04_Repeater)