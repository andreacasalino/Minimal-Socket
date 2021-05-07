project("Udp-Samples")

MakeSample(UdpAsker)

MakeSample(UdpResponder)

MakeSample(UdpServerResponder)

## samples launchers ##

add_executable(SampleUdp-01-client-client SampleUdp-01-client-client.cpp)
add_dependencies(SampleUdp-01-client-client UdpAsker UdpResponder)

add_executable(SampleUdp-02-client-server SampleUdp-02-client-server.cpp)
add_dependencies(SampleUdp-02-client-server UdpAsker UdpServerResponder)
