MakeSample(UdpAsker)
MakeSample(UdpResponder)

MakeLauncher(UdpScriptsGenerator)
add_dependencies(UdpScriptsGenerator UdpAsker UdpResponder)
