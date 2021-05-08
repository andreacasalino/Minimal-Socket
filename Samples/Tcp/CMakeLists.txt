project("Tcp-Samples")

MakeSample(TcpClient)

MakeSample(TcpServer)

MakeSample(TcpServerAsync)

MakeSample(TcpRepeater)

## samples launchers ##

MakeLaunch(SampleTcp-01-server-client TcpClient TcpServer)

MakeLaunch(SampleTcp-02-server-clients TcpClient TcpServer)

MakeLaunch(SampleTcp-03-asyncserver-clients TcpClient TcpServerAsync)

MakeLaunch(SampleTcp-04-repeater TcpClient TcpServer TcpRepeater)
