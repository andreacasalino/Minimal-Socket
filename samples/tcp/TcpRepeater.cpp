/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

///////////////////////////////////////////////////////////////////////////
//                     Have a look to README.md                          //
///////////////////////////////////////////////////////////////////////////

// elements from the MinimalSocket library
#include <MinimalSocket/tcp/TcpClient.h>
#include <MinimalSocket/tcp/TcpServer.h>

// just a bunch of utilities
#include <Args.h>
#include <Names.h>
#include <TimeOfDay.h>

#include <iostream>
using namespace std;

void repeat(MinimalSocket::tcp::TcpConnectionBlocking &preceding,
            MinimalSocket::tcp::TcpClient<true> &following) {
  while (true) {
    auto request = preceding.receive(500, std::chrono::seconds{5});
    if (request.empty()) {
      break;
    }
    cout << MinimalSocket::samples::TimeOfDay{}
         << "forwarding request: " << request << endl;
    following.send(request);
    auto response = following.receive(500, std::chrono::seconds{5});
    if (request.empty()) {
      break;
    }
    cout << MinimalSocket::samples::TimeOfDay{}
         << "backwarding response: " << response << endl;
    preceding.send(response);
  }
}

int main(const int argc, const char **argv) {
  cout << "-----------------------  Repeater  -----------------------" << endl;
  PARSE_ARGS

  const auto following_host =
      options->getValue<std::string>("host", "127.0.0.1");
  const auto following_port =
      options->getValue<MinimalSocket::Port>("next_port");
  MinimalSocket::Address following_address(following_host, following_port);

  const auto port_to_reserve = options->getValue<MinimalSocket::Port>("port");

  // reserve port
  MinimalSocket::tcp::TcpServer<true> acceptor(port_to_reserve,
                                               following_address.getFamily());
  if (!acceptor.open()) {
    cerr << "Failed to bind and listen to specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Listening on port " << port_to_reserve << endl;

  // ask connection to follower
  MinimalSocket::tcp::TcpClient<true> connection_to_following(
      following_address);
  cout << "Connecting to next on chain at "
       << MinimalSocket::to_string(following_address) << endl;
  if (!connection_to_following.open()) {
    cerr << "Unable to connect to next on chain" << endl;
    return EXIT_FAILURE;
  }
  cout << "Connected to next on chain" << endl;

  // wait connection request from preceding
  cout << "Waiting preceding on chain on port " << port_to_reserve << endl;
  auto connection_to_previous = acceptor.acceptNewClient();
  cout << "Connected to preceding on chain" << endl;

  repeat(connection_to_previous, connection_to_following);

  return EXIT_SUCCESS;
}