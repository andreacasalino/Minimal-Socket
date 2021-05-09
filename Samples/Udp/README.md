This folder contains some samples showing how to create and use udp connections.
In order to execute each sample more than 1 processes need to be run.
You don't need to manually do that, since this is done running one of the launcher application named SampleUdp-xx-...

**Samples description**

	*SampleUdp-01-connection, run:
	
		* a first UdpConnection reserving a specific port, ready to respond to another UdpConnection
		* a second UdpConnection exchanging messages with the first
		
	*SampleUdp-02-asyncconnection, run:
	
		* a first UdpConnection reserving a specific port, ready to asynchrously respond to another UdpClient
		* a second UdpConnection exchanging messages with the first
		
	*SampleUdp-03-server, run:
		
		* a UdpServer ready to receive an initial message from another peer to detect it
		* a UdpConnection that connects to the server by sending one message and exchange messages
