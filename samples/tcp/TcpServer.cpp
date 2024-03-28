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
#include <MinimalSocket/tcp/TcpServer.h>

// just a bunch of utilities
#include <Args.h>
#include <Respond.h>

#include <thread>
#include <vector>
using namespace std;

std::thread accept_new_client(MinimalSocket::tcp::TcpServer<true> &server) {
  MinimalSocket::tcp::TcpConnectionBlocking accepted_connection =
      server.acceptNewClient();
  cout << "New client accepted" << endl;
  return std::thread([connection = std::move(accepted_connection)]() mutable {
    MinimalSocket::samples::respond(connection);
  });
}

int main(const int argc, const char **argv) {
  cout << "-----------------------  Server  -----------------------" << endl;
  PARSE_ARGS

  const auto server_port = options->getValue<MinimalSocket::Port>("port");
  const auto max_clients = options->getValue<int>("clients", 0);
  const auto family = options->getValue<MinimalSocket::AddressFamily>(
      "family", MinimalSocket::AddressFamily::IP_V4);

  MinimalSocket::tcp::TcpServer<true> server(server_port, family);

  if (!server.open()) {
    cerr << "Failed to bind and listen to specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Listening for new clients on port " << server_port << endl;

  std::vector<std::thread> active;
  if (0 == max_clients) {
    while (true) {
      active.emplace_back(accept_new_client(server));
    }
  } else {
    for (int c = 0; c < max_clients; ++c) {
      active.emplace_back(accept_new_client(server));
    }
  }

  for (auto &th : active) {
    th.join();
  }

  return EXIT_SUCCESS;
}
