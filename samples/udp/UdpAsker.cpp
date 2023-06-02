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

  const auto remote_host = options->getValue("host", "127.0.0.1");
  const auto remote_port =
      static_cast<MinimalSocket::Port>(options->getIntValue("port"));
  const auto port_this =
      static_cast<MinimalSocket::Port>(options->getIntValue("port_this"));
  const auto rate =
      std::chrono::milliseconds{options->getIntValue<250>("rate")};

  const MinimalSocket::Address remote_address(remote_host, remote_port);
  MinimalSocket::udp::UdpBinded asker(port_this, remote_address.getFamily());

  std::this_thread::sleep_for(
      std::chrono::seconds{1}); // just to be sure the responder has already
                                // prepared to receive the requests
  if (!asker.open()) {
    cerr << "Failed to reserve specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Port successfully reserved" << endl;

  MinimalSocket::samples::ask(asker, remote_address, rate,
                              options->getIntValue<5>("cycles"));

  return EXIT_SUCCESS;
}
