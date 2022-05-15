/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/tcp/TcpClient.h>

#include <Args.h>
#include <Ask.h>

using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Client  -----------------------" << endl;
  PARSE_ARGS

  const auto server_host = options->getValue("host", "127.0.0.1");
  const auto server_port = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port").c_str()));
  const auto rate = std::chrono::milliseconds{
      std::atoi(options->getValue("rate", "250").c_str())};

  const MinimalSocket::Address server_address(server_host, server_port);
  MinimalSocket::tcp::TcpClient client(server_address);

  cout << "Connecting to " << MinimalSocket::to_string(server_address) << endl;
  if (!client.open()) {
    cout << "Failed to open connection" << endl;
    return EXIT_FAILURE;
  }
  cout << "Connected" << endl;

  MinimalSocket::samples::ask_forever(client, rate);

  // the connection will be close when destroying the client object
  return EXIT_SUCCESS;
}
