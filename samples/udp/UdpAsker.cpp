/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/udp/UdpSocket.h>

#include <Args.h>
#include <Ask.h>

using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Udp asker  -----------------------" << endl;
  PARSE_ARGS

  const auto remote_host = options->getValue("host", "127.0.0.1");
  const auto remote_port = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port").c_str()));
  const auto port_this = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port_this").c_str()));
  const auto rate = std::chrono::milliseconds{
      std::atoi(options->getValue("rate", "100").c_str())};
  const auto cycles = std::atoi(options->getValue("cycles", "0").c_str());

  const MinimalSocket::Address remote_address(remote_host, remote_port);
  MinimalSocket::udp::UdpBinded asker(port_this, remote_address.getFamily());

  if (!asker.open()) {
    cout << "Failed to reserve port " << port_this << endl;
    return EXIT_FAILURE;
  }

  // if (0 == cycles) {
  //   MinimalSocket::samples::ask_forever(client, rate);
  // } else {
  //   MinimalSocket::samples::ask(client, cycles, rate);
  // }

  // the connection will be close when destroying the client object
  return EXIT_SUCCESS;
}
