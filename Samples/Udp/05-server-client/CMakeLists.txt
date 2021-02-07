add_executable(Sample05_Client Client.cpp)
target_link_libraries( Sample05_Client Sample-Utils)	

add_executable(Sample05_Server Server.cpp)
target_link_libraries( Sample05_Server Sample-Utils)	

add_executable(Sample05Launcher Launcher.cpp)
target_link_libraries(Sample05Launcher)
add_dependencies(Sample05Launcher Sample05_Server Sample05_Client)
