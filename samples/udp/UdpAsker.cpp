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
#include <MinimalSocket/udp/UdpSocket.h>

// just a bunch of utilities
#include <Args.h>
#include <Ask.h>

using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Udp asker  -----------------------" << endl;
  PARSE_ARGS

  const auto remote_host = options->getValue<std::string>("host", "127.0.0.1");
  const auto remote_port = options->getValue<MinimalSocket::Port>("port");
  const auto port_this = options->getValue<MinimalSocket::Port>("port_this");
  const auto rate = options->getValue<std::chrono::milliseconds>(
      "rate", std::chrono::milliseconds{250});

  const MinimalSocket::Address remote_address(remote_host, remote_port);
  MinimalSocket::udp::Udp<true> asker(port_this, remote_address.getFamily());

  std::this_thread::sleep_for(
      std::chrono::seconds{1}); // just to be sure the responder has already
                                // prepared to receive the requests
  if (!asker.open()) {
    cerr << "Failed to reserve specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Port successfully reserved" << endl;

  MinimalSocket::samples::ask(asker, remote_address, rate,
                              options->getValue<int>("cycles", 5));

  return EXIT_SUCCESS;
}
