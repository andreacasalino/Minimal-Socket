![binaries_compilation](https://github.com/andreacasalino/Cross-Platform-Socket/actions/workflows/runTests.yml/badge.svg)

- [What is this library about](#intro)
- [Features](#features)
- [Usage](#usage)
- [Samples](#samples)
- [CMake support](#cmake-support)

![Sockets](./sockets.png)

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
- **AF_INET** (**ip v4**) and **AF_INET6** (**ip v6**), refer to [this](https://www.ibm.com/docs/en/i/7.1?topic=characteristics-socket-address-family) link, are both supported
- Many sockets operations are by default blocking. However, **MinimalSocket** allows you also to opt for non-blocking versions off such operations, specifying a **timeout** to use, after which the operation terminates in any case. In particular, the operations allowing for such possibility are:
    - non blocking receive (send are always intrinsically non blocking)
    - acceptance of a new client from the tcp server side
- **MinimalSocket** is tested to be **thread safe**. Morevoer, you can also send while receiving in different dedicated threads. This allows you to easily create your own asynchronous sockets, building upon the classes offered by this library.
- **Udp** sockets can be used both as un-connected or connected, check [here](./samples/udp/README.md) for further details. Moreover, the same **udp** socket can be connected or sconnected during its lifetime.
- Under **Windows** systems, [**WSAStartup**](https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup) is automatically called before using any functionalities. From the outside, you can specify the Windows Sockets specification version.

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

To create a **tcp** client you just need to build a **tcp::TcpClient** object:
```cpp
#include <MinimalSocket/tcp/TcpClient.h>

MinimalSocket::Port server_port = 15768;
std::string server_address = "192.168.125.85";
MinimalSocket::tcp::TcpClient tcp_client(
    MinimalSocket::Address{server_address, server_port});
```

open it:
```cpp
// open the client: asks connection to server
bool success = tcp_client.open();
```

you can now receive and send information with the remote server by simply doing this:
```cpp
// send a message
tcp_client.send("a message to send");
// receive a message
std::size_t message_max_size = 1000;
std::string
    received_message // resized to the nunber of bytes actually received
    = tcp_client.receive(message_max_size);
```

### UDP

To create a normal **udp** socket you just need to build a **udp::UdpBinded** object:
```cpp
#include <MinimalSocket/udp/UdpSocket.h>

MinimalSocket::Port this_socket_port = 15768;
MinimalSocket::udp::UdpBinded udp_socket(this_socket_port,
                                        MinimalSocket::AddressFamily::IP_V6);
```

open it:
```cpp
// open the client: reserve port for this cocket
bool success = udp_socket.open();
```

you can now receive and send information with any other opened **udp** socket:
```cpp
// send a message to another udp
MinimalSocket::Address other_recipient_udp =
    MinimalSocket::Address{"192.168.125.85", 15768};
udp_socket.sendTo("a message to send", other_recipient_udp);
// receive a message from another udp reaching this one
std::size_t message_max_size = 1000;
auto received_message = udp_socket.receive(message_max_size);
// check the sender address
MinimalSocket::Address other_sender_udp = received_message->sender;
// access the received message
std::string received_message_content // resized to the nunber of bytes
                                    // actually received
    = received_message->received_message;
```

you can also decide to connect an opened **udp** socket to a specific address. This simply means that messages incoming from other peers will be filtered out, as **udp** sockets are not connection oriented:
```cpp
MinimalSocket::Address permanent_sender_udp =
    MinimalSocket::Address{"192.168.125.85", 15768};
MinimalSocket::udp::UdpConnected udp_connected_socket = udp_socket.connect(
    permanent_sender_udp); // ownership of the underlying socket is transfered
                            // from udp_socket to udp_connected_socket, meaning
                            // that you can't use anymore udp_socket (unless
                            // you re-open it)
```

Now you can send and receive data without having to specify the recpient/sender:
```cpp
// receive a message
std::size_t message_max_size = 1000;
std::string
    received_message // resized to the nunber of bytes actually received
    = udp_connected_socket.receive(message_max_size);
// send a message
udp_connected_socket.send("a message to send");
```

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
set(BUILD_MinimalCppSocket_SAMPLES OFF CACHE BOOL "" FORCE) # you don't want the samples in this case
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
