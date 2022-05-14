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
  cout << "-----------------------  Udp responder  -----------------------"
       << endl;
  PARSE_ARGS

  const auto port_this = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port_this").c_str()));
  const auto family =
      MinimalSocket::samples::to_family(options->getValue("family", "v4"));
  const bool connect = options->getValue("connect", "no") == "yes";

  MinimalSocket::udp::UdpBinded responder(port_this, family);

  if (!responder.open()) {
    cout << "Failed to reserve port " << port_this << endl;
    return EXIT_FAILURE;
  }

  if (connect) {
    // connect to first sending a request

  } else {
  }

  return EXIT_SUCCESS;
}
