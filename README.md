The classes contained in ./StreamSocket allows you to create server-client tcp connection from C++ code.
The library is cross platform: let CMake handle the dependencies.
Check the 4 examples provided in ./Samples for the details, in particular:

-Sample_01 is a simple handshake between a server and a client
-Sample_02 implements a service toward two clients from the same server, which responds to the client in a single thread
-Sample_03 is similar to Sample_02, but the server use two different threads to manage the requests of the two different clients
-Sample_04 in this example there is a client comunicating with a repeater that in turns forwards client's requests to the server

Check also the README.svg (use your favourite browser to open it) contained in the samples folders.

----------------------------------------------------------------------------------

to run each example, compile the corresponding Launcher and use it
