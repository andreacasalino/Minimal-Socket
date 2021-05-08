This repository contains the minimal functionalities to create and use from **C++** both **tcp** and **udp** sockets in a
completely platform independent way.

**Content**

  * The core packages are contained in the ./CrossSocket folder:
  	* SynchSocket implements the functionalities to create and use standard tcp and udp connections
  	* AsynchSocket contains implementation of asynchronous sockets, i.e. objects having a private thread service that can use the socket.
  
  * Samples showing the library usage are contained in the ./Samples folder. ATTENTION!!! The Samples execution might be blocked the first time by your firewall.
 
**Build from sources**

Use [CMake](https://cmake.org) to configure and compile the library and the samples.

**Run the Samples**

Check the *README.md* inside Samples/Tcp/ and Samples/Udp/

**Download compiled binaries**

You can simply download and use the binaries of the latest master version [here](https://github.com/andreacasalino/Cross-Platform-Socket/actions/runs/640613596)

**What Else?**

Have fun and leave a **star**.

