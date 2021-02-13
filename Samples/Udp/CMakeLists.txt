add_executable(UdpClientAsker UdpClientAsker.cpp)
target_link_libraries(UdpClientAsker Sample-Utils)	

add_executable(UdpClientResponder UdpClientResponder.cpp)
target_link_libraries(UdpClientResponder Sample-Utils)	

add_executable(UdpServer UdpServer.cpp)
target_link_libraries(UdpServer Sample-Utils)	

## launchers ##

add_executable(Udp01Launcher Launcher-01-client-client.cpp)
add_dependencies(Udp01Launcher UdpClientAsker UdpClientResponder)

add_executable(Udp02Launcher Launcher-02-client-server.cpp)
add_dependencies(Udp02Launcher UdpClientAsker UdpServer)
