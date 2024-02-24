// tcp server
#include <MinimalSocket/tcp/TcpServer.h>
int main() {
  MinimalSocket::Port port = 15768; // the port to bind
  MinimalSocket::tcp::TcpServer tcp_server(port,
                                           MinimalSocket::AddressFamily::IP_V4);

  // Open the server. This will bind the port and the server will start to
  // listen for connection requests.
  bool success = tcp_server.open();

  // accepts the next client that will ask the connection
  MinimalSocket::tcp::TcpConnection accepted_connection =
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
  MinimalSocket::tcp::TcpClient tcp_client(
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
  MinimalSocket::udp::UdpBinded udp_socket(this_socket_port,
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
  MinimalSocket::udp::UdpConnected udp_connected_socket = udp_socket.connect(
      permanent_sender_udp); // ownership of the underlying socket is transfered
                             // from udp_socket to udp_connected_socket, meaning
                             // that you can't use anymore udp_socket (unless
                             // you re-open it)

  // receive a message
  std::size_t message_max_size = 1000;
  std::string
      received_message // resized to the nunber of bytes actually received
      = udp_connected_socket.receive(message_max_size);
  // send a message
  udp_connected_socket.send("a message to send");
}
