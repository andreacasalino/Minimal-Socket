project("Udp-Samples")

MakeSample(UdpAsker)

MakeSample(UdpResponder)

if(COMPILE_ASYNCH)
	MakeSample(UdpResponderAsync)
endif()

MakeSample(UdpServer)

## samples launchers ##

MakeLaunch(SampleUdp-01-connection UdpAsker UdpResponder)

if(COMPILE_ASYNCH)
	MakeLaunch(SampleUdp-02-asyncconnection UdpAsker UdpResponderAsync)
endif()

MakeLaunch(SampleUdp-03-server UdpAsker UdpServer)
