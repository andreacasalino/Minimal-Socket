project("Tcp-Samples")

MakeSample(TcpClient)

MakeSample(TcpServer)

MakeSample(TcpServerAsync)

MakeSample(TcpRepeater)

## samples launchers ##

add_executable(SampleTcp-01-server-client SampleTcp-01-server-client.cpp)
add_dependencies(SampleTcp-01-server-client TcpClient TcpServer)

add_executable(SampleTcp-02-server-clients SampleTcp-02-server-clients.cpp)
add_dependencies(SampleTcp-02-server-clients TcpClient TcpServer)

add_executable(SampleTcp-03-asyncserver-client SampleTcp-03-asyncserver-client.cpp)
add_dependencies(SampleTcp-03-asyncserver-client TcpClient TcpServerAsync)

add_executable(SampleTcp-04-repeater SampleTcp-04-repeater.cpp)
add_dependencies(SampleTcp-04-repeater TcpClient TcpServer TcpRepeater)
