![binaries_compilation](https://github.com/andreacasalino/Cross-Platform-Socket/actions/workflows/installArtifacts.yml/badge.svg)
![binaries_compilation](https://github.com/andreacasalino/Cross-Platform-Socket/actions/workflows/runTests.yml/badge.svg)

- [What is this library about](#intro)
- [Features](#features)
- [Usage](#usage)
- [Samples](#samples)
- [CMake support](#cmake-support)

## INTRO

**MinimalSocket** gives you a modern **C++** interface to create, connect and handle **tcp** and **udp** sockets, in a
completely platform independent way. The supported systems are: **Windows**, any **Linux** distro and **MacOS**.

Check [Features](#features) to see details about the various features of **MinimalSocket**. You can refer to [Usage](#usage) and [Samples](#samples) to see how to use **MinimalSocket**.

This is a **CMake** project, check [CMake support](#cmake-support) to see how this library can be integrated.

Remember to leave a **star** in case you have found this library useful.

## FEATURES

Haven't left a **star** already? Do it now ;)!

**MinimalSocket** allows you to build and set up **tcp** and **udp** connections. Messages can be sent and received in terms of both low level buffer of chars or high level string. Indeed, this is actually the only capability you need for a socket, as more complex messages can be encoded and decoded using among the others approaches like [Google Protocol Buffers](https://developers.google.com/protocol-buffers/docs/cpptutorial) or [NanoPb](https://jpa.kapsi.fi/nanopb/).

This are the most notable properties of **MinimalSocket**:
- A modern **C++** interface allows you to set up and build connections in terms of objects. Sockets are not opened as soon as the wrapping object is created, but you after calling a proper method, allowing you to decouple socket creation from socket opening. Sockets are automatically closed (and all relevant information cleaned after destroying the wrapping object).
- You don't need to access low level functions from system modules: let **MinimalSocket** do it for you. Actually, all the system specific modules, functions, linkages are kept completely private.
- Many sockets operations (like for instance receive, accept clients, wait for server acceptance, etc...) are by default blocking. However,
**MinimalSocket** allows you also to opt for non-blocking versions off such operations, specifying the **timeout** to use.
- **MinimalSocket** is tested to be **thread safe**. Morevoer, you can also send while receiving in different dedicated threads.
- **Udp** sockets acn be used both as un-connected or connected, check [here](./samples/udp/README.md) for further details. Moreover, the same **udp** socket can be connected or sconnected during its lifetime.
-Under **Windows** systems, [**WSAStartup**](https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup) is automatically called before using any functionalities. From the outside, you can specify the Windows Sockets specification version.

## USAGE

Haven't left a **star** already? Do it now ;)!

### TCP

#### SERVER

To create a **tcp** server you just need to build a **tcp::TcpServer** object:
```cpp
#include <MinimalSocket/tcp/TcpServer.h>

MinimalSocket::Port port = 15768; // port the server needs to bind
MinimalSocket::tcp::TcpServer tcp_server(port,
                                        MinimalSocket::AddressFamily::IP_V4);
```

open it:
```cpp
// open the server: binds the port and start to listen on the port
bool success = tcp_server.open();
```

and now you are ready to accept new clients:
```cpp
// accepts next client asking connection
MinimalSocket::tcp::TcpConnection accepted_connection =
    tcp_server.acceptNewClient(); // blocing till a client actually asks the
                                // connection
```

you can now receive and send information with the accepted client by simply doing this:
```cpp
// receive a message
std::size_t message_max_size = 1000;
std::string
    received_message // resized to the nunber of bytes actually received
    = accepted_connection.receive(message_max_size);
// send a message
accepted_connection.send("a message to send");
```

#### CLIENT


### UDP

## SAMPLES

Haven't left a **star** already? Do it now ;)!

Instructions about the **tcp** samples are contained [here](./samples/tcp/README.md), while [here](./samples/udp/README.md) the **udp** samples are explained.

ATTENTION!!! The Samples execution might be blocked the first time by your firewall: set up properly your firewall or run the samples with the [administrator privileges](https://www.techopedia.com/definition/4961/administrative-privileges#:~:text=Administrative%20privileges%20are%20the%20ability,as%20a%20database%20management%20system.)

## CMAKE SUPPORT

Haven't left a **star** already? Do it now ;)!
   
To consume this library you can rely on [CMake](https://cmake.org).
More precisely, You can fetch this package and link to the **MinimalSocket** library:
```cmake
include(FetchContent)
FetchContent_Declare(
min_sock
GIT_REPOSITORY https://github.com/andreacasalino/Minimal-Socket
GIT_TAG        master
)
FetchContent_MakeAvailable(min_sock)
```

and then link to the **MinimalSocket** library:
```cmake
target_link_libraries(${TARGET_NAME}
    MinimalSocket
)
```

All the system specific modules are internally inlcluded and don't exposed to the outside.
Moreover, under **Windows**, **wsock32** and **ws2_32** are privately linked and you don't need to link them again when integrating **MinimalSocket**.
