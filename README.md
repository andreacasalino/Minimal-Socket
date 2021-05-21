![binaries_compilation](https://github.com/andreacasalino/Cross-Platform-Socket/actions/workflows/installArtifacts.yml/badge.svg)
![binaries_compilation](https://github.com/andreacasalino/Cross-Platform-Socket/actions/workflows/runTests.yml/badge.svg)

This repository contains the minimal functionalities to create and use from **C++** both **tcp** and **udp** sockets in a
completely platform independent way. You can decide to compile just the very core package named SynchSocket, with the raw socket implementations or compile also the more advanced ones.

**Content**

  * The core packages are contained in the ./CrossSocket folder:
  	* SynchSocket implements the minimal functionalities to create and use raw tcp and udp connections, sending and receiving buffer of bytes
  	* AsynchSocket contains implementation of asynchronous sockets, i.e. sockets storing a private service constantly receiving new messages. It is possible to subscribe to the received messages (and react to it) by attaching a listener to the asynchronous socket. You can decide to not compile this package by setting to OFF the CMake option named COMPILE_ASYNCH
	* TypedSocket contains usefull functionalities to build typed messangers, i.e. sockets exchanging complex data structures. These functionalities can be used in combination with [Google Protocol Buffers](https://developers.google.com/protocol-buffers/docs/cpptutorial) or similar implementations like [NanoPb](https://jpa.kapsi.fi/nanopb/) to automatically encode and decode data. You can decide to not compile this package by setting to OFF the CMake option named COMPILE_TYPED
  
  * Samples showing the library usage are contained in the ./Samples folder. ATTENTION!!! The Samples execution might be blocked the first time by your firewall: set up properly your firewall or run the samples with the [administrator privileges](https://www.techopedia.com/definition/4961/administrative-privileges#:~:text=Administrative%20privileges%20are%20the%20ability,as%20a%20database%20management%20system.)
 
**Build from sources**

Use [CMake](https://cmake.org) to configure and compile the library and the samples. By setting to ON the CMake option LIB_OPT, the above 3 packages will be compiled as shared libraries, otherwise as static.

**Run the Samples**

Check the *README.md* inside Samples/Tcp/ and Samples/Udp/ to understand the samples purpose and how to run them

**Download compiled binaries**

You can simply download and use the binaries of the latest master version [here](https://github.com/andreacasalino/Cross-Platform-Socket/actions/runs/640613596)

**What Else?**

Have fun and leave a **star**.

