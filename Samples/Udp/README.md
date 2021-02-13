This folder contains some samples using udp connections.
In order to execute each sample more than 1 processes needs to be run.
You don't need to manually do that, since some appropriate launcher scripts
can be run to do all the work. To create the launchers you need to compile and run:
**Udp01Launcher** or **Udp02Launcher**
according to the sample you are interested in.
After running one of the above application a *.bat* or *.sh* script (according to
which is your system) is created: you can execute that script from the command line
to run the corresponding example

**Samples description**

	*Sample01, run:
	
		* a first UdpClient reserve a specific port and it is ready to respond to another UdpClient
		* a second UdpClient exchanging messages with the first
		
	*Sample02, run:
		
		* a UdpServer ready to receive an initial message from a client to detect it
		* a UdpClient that connects to the server by sending one message and exchange messages