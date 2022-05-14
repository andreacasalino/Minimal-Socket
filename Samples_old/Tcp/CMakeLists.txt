project("Tcp-Samples")

MakeSample(TcpClient)

MakeSample(TcpServer)

if(COMPILE_ASYNCH)
	MakeSample(TcpServerAsync)
endif()

MakeSample(TcpRepeater)

## samples launchers ##

MakeLaunch(SampleTcp-01-server-client TcpClient TcpServer)

MakeLaunch(SampleTcp-02-server-clients TcpClient TcpServer)

if(COMPILE_ASYNCH)
	MakeLaunch(SampleTcp-03-asyncserver-clients TcpClient TcpServerAsync)
endif()

MakeLaunch(SampleTcp-04-repeater TcpClient TcpServer TcpRepeater)
