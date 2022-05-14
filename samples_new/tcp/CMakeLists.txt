MakeSample(TcpClient)
MakeSample(TcpServer)
MakeSample(TcpRepeater)

MakeLauncher(TcpScriptsGenerator)
add_dependencies(TcpScriptsGenerator TcpClient TcpServer TcpRepeater)
