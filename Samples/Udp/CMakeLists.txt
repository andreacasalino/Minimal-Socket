project("Udp-Samples")

MakeSample(UdpAsker)

MakeSample(UdpResponder)

MakeSample(UdpResponderAsync)

MakeSample(UdpServer)

## samples launchers ##

MakeLaunch(SampleUdp-01-connection UdpAsker UdpResponder)

MakeLaunch(SampleUdp-02-asyncconnection UdpAsker UdpResponderAsync)

MakeLaunch(SampleUdp-03-server UdpAsker UdpServer)
