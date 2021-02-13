This folder contains some samples using tcp connections.
In order to execute each sample more than 1 processes needs to be run.
You don't need to manually do that, since some appropriate launcher scripts
can be run to do all the work. To create the launchers you need to compile and run:
**Tcp01Launcher**, **Tcp02Launcher** or **Tcp03Launcher**
according to the sample you are interested in.
After running one of the above application a *.bat* or *.sh* script (according to
which is your system) is created: you can execute that script from the command line
to run the corresponding example

**Samples description**

	*Sample01, run:
	
		* a Tcp server ready to accept 1 client
		* a Tcp client that connects to the server and exchange messages
		
	*Sample02, run:
	
		* a Tcp server ready to accept 2 client
		* a first Tcp client that connects to the server and exchange messages with an high frequency
		* a second Tcp client that connects to the server and exchange messages with a low frequency

	*Sample03, run:
	
		* a Tcp server ready to accept 1 client
		* a Tcp client that connects to an intermediate repeater
		* a repeater creating 2 connections in order to receive the request of the client and forward them to the server, sending back the response
		
	*Sample04, run:
	
		* a Tcp server asynchronous accepting 1 single client
		* a Tcp client that connects to the async server and exchange messages
		
