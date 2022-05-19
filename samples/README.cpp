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
