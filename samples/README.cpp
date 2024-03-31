// tcp server
#include <MinimalSocket/tcp/TcpServer.h>
int main() {
  MinimalSocket::Port port = 15768; // the port to bind
  MinimalSocket::tcp::TcpServer<true> tcp_server(
      port, MinimalSocket::AddressFamily::IP_V4);

  // Open the server. This will bind the port and the server will start to
  // listen for connection requests.
  bool success = tcp_server.open();

  // accepts the next client that will ask the connection
  MinimalSocket::tcp::TcpConnectionBlocking accepted_connection =
      tcp_server.acceptNewClient(); // blocking till a client actually asks the
                                    // connection

  // receive a message
  std::size_t message_max_size = 1000;
  std::string
      received_message // resized to the nunber of bytes actually received
      = accepted_connection.receive(message_max_size);
  // send a message
  accepted_connection.send("a message to send");
}

// tcp client
#include <MinimalSocket/tcp/TcpClient.h>
int main() {
  MinimalSocket::Port server_port = 15768;
  std::string server_address = "192.168.125.85";
  MinimalSocket::tcp::TcpClient<true> tcp_client(
      MinimalSocket::Address{server_address, server_port});

  // Open the server. Here, the client will ask the connection to specified
  // server. After that, the client will be actually connected.
  bool success =
      tcp_client.open(); // blocking till the connection is actually established

  // send a message
  tcp_client.send("a message to send");
  // receive a message
  std::size_t message_max_size = 1000;
  std::string
      received_message // resized to the nunber of bytes actually received
      = tcp_client.receive(message_max_size);
}

// udp socket
#include <MinimalSocket/udp/UdpSocket.h>
int main() {
  MinimalSocket::Port this_socket_port = 15768;
  MinimalSocket::udp::Udp<true> udp_socket(this_socket_port,
                                           MinimalSocket::AddressFamily::IP_V6);

  // Open the server. This will bind the specified port.
  bool success = udp_socket.open();

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

  MinimalSocket::Address permanent_sender_udp =
      MinimalSocket::Address{"192.168.125.85", 15768};
  MinimalSocket::udp::UdpConnected<true> udp_connected_socket =
      udp_socket.connect(
          permanent_sender_udp); // ownership of the underlying socket is
                                 // transfered from udp_socket to
                                 // udp_connected_socket, meaning that you can't
                                 // use anymore udp_socket (unless you re-open
                                 // it)

  // receive a message
  std::size_t message_max_size = 1000;
  std::string
      received_message // resized to the nunber of bytes actually received
      = udp_connected_socket.receive(message_max_size);
  // send a message
  udp_connected_socket.send("a message to send");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// tcp server, non blocking
int main() {
  MinimalSocket::Port port = 15768; // the port to bind
  MinimalSocket::tcp::TcpServer<false> tcp_server(
      port, MinimalSocket::AddressFamily::IP_V4);
  tcp_server.open();

  // check if a client asked for the connection. If no, the function immediately
  // returns a nullopt. On the contrary, the returned optional contains the
  // handler to the connected client
  std::optional<MinimalSocket::tcp::TcpConnectionBlocking>
      maybe_accepted_connection = tcp_server.acceptNewClient();

  MinimalSocket::tcp::TcpConnectionNonBlocking accepted_connection_nn_block =
      maybe_accepted_connection->turnToNonBlocking();
}

// tcp client, non blocking
int main() {
  MinimalSocket::Port server_port = 15768;
  std::string server_address = "192.168.125.85";
  MinimalSocket::tcp::TcpClient<false> tcp_client(
      MinimalSocket::Address{server_address, server_port});
  tcp_client.open();

  std::size_t message_max_size = 1000;
  // non blocking receive: returns immediately with an empty message in case no
  // new data were available, or with a non empty message in the contrary case
  std::string received_message = tcp_client.receive(message_max_size);
}

// udp socket, non blocking
int main() {
  MinimalSocket::Port this_socket_port = 15768;
  MinimalSocket::udp::Udp<false> udp_socket(
      this_socket_port, MinimalSocket::AddressFamily::IP_V6);
  udp_socket.open();

  std::size_t message_max_size = 1000;
  // non blocking receive: returns immediately with an empty message in case no
  // new data were available, or with a non empty message in the contrary case
  //
  // struct ReceiveStringResult {
  //   Address sender;
  //   std::string received_message;
  // };
  std::optional<MinimalSocket::ReceiveStringResult> received_message =
      udp_socket.receive(message_max_size);
}
