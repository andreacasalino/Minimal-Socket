/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/tcp/TcpClient.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include <Args.h>

#include <iostream>
using namespace std;

void repeat(MinimalSocket::tcp::TcpConnection &preceding,
            MinimalSocket::tcp::TcpClient &following) {
  auto request = preceding.receive(500);
  cout << "forwarding request: " << request << endl;
  following.send(request);
  auto response = following.receive(500);
  cout << "backwarding response: " << response << endl;
  preceding.send(response);
}

int main(const int argc, const char **argv) {
  cout << "-----------------------  Repeater  -----------------------" << endl;
  PARSE_ARGS

  const auto following_host = options->getValue("host", "127.0.0.1");
  const auto following_port = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("next_port").c_str()));
  MinimalSocket::Address following_address(following_host, following_port);

  const auto port_to_reserve = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port").c_str()));

  // reserve port
  MinimalSocket::tcp::TcpServer acceptor(port_to_reserve,
                                         following_address.getFamily());
  if (!acceptor.open()) {
    cout << "Failed to bind and listen to specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Listening on port " << port_to_reserve << endl;

  // ask connection to follower
  MinimalSocket::tcp::TcpClient connection_to_following(following_address);
  cout << "Connecting to next on chain at "
       << MinimalSocket::to_string(following_address) << endl;
  if (!connection_to_following.open()) {
    cout << "Unable to connect to next on chain" << endl;
    return EXIT_FAILURE;
  }
  cout << "Connected to next on chain" << endl;

  // wait connection request from preceding
  cout << "Waiting preceding on chain on port " << port_to_reserve << endl;
  auto connection_to_previous = acceptor.acceptNewClient();
  cout << "Connected to preceding on chain" << endl;

  while (true) {
    repeat(connection_to_previous, connection_to_following);
  }

  return EXIT_SUCCESS;
}