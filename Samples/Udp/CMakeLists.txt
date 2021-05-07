project("Udp-Samples")

MakeSample(UdpAsker)

MakeSample(UdpResponder)

MakeSample(UdpServerResponder)

## samples launchers ##

MakeLaunch(SampleUdp-01-connection UdpAsker UdpResponder)

MakeLaunch(SampleUdp-02-server UdpAsker UdpServerResponder)
