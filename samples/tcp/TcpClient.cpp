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

// just a bunch of utilities
#include <Args.h>
#include <Ask.h>

using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Client  -----------------------" << endl;
  PARSE_ARGS

  const auto server_host = options->getValue<std::string>("host", "127.0.0.1");
  const auto server_port = options->getValue<MinimalSocket::Port>("port");
  const auto rate = options->getValue<std::chrono::milliseconds>(
      "rate", std::chrono::milliseconds{250});

  const MinimalSocket::Address server_address(server_host, server_port);
  MinimalSocket::tcp::TcpClient<true> client(server_address);

  cout << "Connecting to " << MinimalSocket::to_string(server_address) << endl;
  if (!client.open()) {
    cerr << "Failed to open connection" << endl;
    return EXIT_FAILURE;
  }
  cout << "Connected" << endl;

  MinimalSocket::samples::ask(client, rate,
                              options->getValue<int>("cycles", 5));

  // the connection will be close when destroying the client object
  return EXIT_SUCCESS;
}
