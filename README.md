The classes contained in ./src/Stream_Socket.h,.cpp allows you to create server-client tcp connection from C++ code.
Stream_Socket is cross platform: you can use it in Windows or Linux with no problem.
Check the 4 examples provided in ./Samples for the details, in particular:

-Sample_01 is a simple handshake between a server and a client
-Sample_02 implements a service toward two clients from the same server, which responds to the client in a single thread
-Sample_03 is similar to Sample_02, but the server use two different threads to manage the requests of the two different clients
-Sample_04 in this example there is a client comunicating with a repeater that in turns forwards client's requests to the server

Check also the README.svg (use your favourite browser to open it) contained in the samples folders.

----------------------------------------------------------------------------------

to compile and run the examples:

-Linux: use build.sh and the launch.sh that you can find in the samples folders

-Window: use build.bat and the launch.bat that you can find in the samples folders. Compile is done assuming that you have installed g++ (with MinGw for example). Anyway you can use Stream_Socker.h also in VisualStudio and it should automatically link with winsock2: simply include Stream_Socket.h
