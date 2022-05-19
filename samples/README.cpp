// tcp server
#include <MinimalSocket/tcp/TcpServer.h>
int main() {
  MinimalSocket::Port port = 15768; // port the server needs to bind
  MinimalSocket::tcp::TcpServer tcp_server(port,
                                           MinimalSocket::AddressFamily::IP_V4);

  // open the server: binds the port and start to listen on the port
  bool success = tcp_server.open();

  // accepts next client asking connection
  MinimalSocket::tcp::TcpConnection accepted_connection =
      tcp_server.acceptNewClient(); // blocing till a client actually asks the
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

  // open the client: asks connection to server
  bool success = tcp_client.open();

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

  // open the client: reserve port for this cocket
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
