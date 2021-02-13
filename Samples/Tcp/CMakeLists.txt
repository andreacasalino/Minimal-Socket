add_executable(TcpClient TcpClient.cpp)
target_link_libraries(TcpClient Sample-Utils)	

add_executable(TcpServer TcpServer.cpp)
target_link_libraries(TcpServer Sample-Utils)

add_executable(TcpServerAsync TcpServerAsync.cpp)
target_link_libraries(TcpServerAsync Sample-Utils)	

add_executable(TcpRepeater TcpRepeater.cpp)
target_link_libraries(TcpRepeater Sample-Utils)	

## launchers ##

add_executable(Tcp01Launcher Launcher-01-server-client.cpp)
add_dependencies(Tcp01Launcher TcpClient TcpServer)

add_executable(Tcp02Launcher Launcher-02-server-clients.cpp)
add_dependencies(Tcp02Launcher TcpClient TcpServer)

add_executable(Tcp03Launcher Launcher-03-repeater.cpp)
add_dependencies(Tcp03Launcher TcpClient TcpServer TcpRepeater)

add_executable(Tcp04Launcher Launcher-04-asyncserver-client.cpp)
add_dependencies(Tcp04Launcher TcpClient TcpServerAsync)
