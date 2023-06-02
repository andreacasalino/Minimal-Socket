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

  const auto server_host = options->getValue("host", "127.0.0.1");
  const auto server_port =
      static_cast<MinimalSocket::Port>(options->getIntValue("port"));
  const auto rate =
      std::chrono::milliseconds{options->getIntValue<250>("rate")};

  const MinimalSocket::Address server_address(server_host, server_port);
  MinimalSocket::tcp::TcpClient client(server_address);

  cout << "Connecting to " << MinimalSocket::to_string(server_address) << endl;
  if (!client.open()) {
    cerr << "Failed to open connection" << endl;
    return EXIT_FAILURE;
  }
  cout << "Connected" << endl;

  MinimalSocket::samples::ask(client, rate, options->getIntValue<5>("cycles"));

  // the connection will be close when destroying the client object
  return EXIT_SUCCESS;
}
