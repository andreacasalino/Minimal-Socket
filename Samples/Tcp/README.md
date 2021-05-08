This folder contains some samples showing how to create and use tcp sockets (with server or client roles).
In order to execute each sample more than 1 processes need to be run.
You don't need to manually do that, since this is done running one of the launcher application named SampleTcp-xx-...

**Samples description**

	*SampleTcp-01-server-client, run:
	
		* a Tcp server ready to accept 1 client
		* a Tcp client that connects to the server and exchange messages
		
	*SampleTcp-02-server-clients, run:
	
		* a Tcp server ready to accept 2 client
		* a first Tcp client that connects to the server and exchange messages with an high frequency
		* a second Tcp client that connects to the server and exchange messages with a low frequency

	*SampleTcp-03-asyncserver-clients, run:
	
		* a Tcp server asynchronous accepting 1 single client
		* a Tcp client that connects to the async server and exchange messages
			
	*SampleTcp-04-repeater, run:
	
		* a Tcp server ready to accept 1 client
		* a Tcp client that connects to an intermediate repeater
		* a repeater creating 2 connections in order to receive the request of the client and forward them to the server, sending back the response
		
