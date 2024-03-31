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
#include <Respond.h>
using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Udp responder  -----------------------"
       << endl;
  PARSE_ARGS

  const auto port_this = options->getValue<MinimalSocket::Port>("port_this");
  const auto family = options->getValue<MinimalSocket::AddressFamily>(
      "family", MinimalSocket::AddressFamily::IP_V4);
  const bool connect = options->getValue<bool>("connect", false);

  MinimalSocket::udp::Udp<true> responder(port_this, family);

  if (!responder.open()) {
    cerr << "Failed to reserve specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Port successfully reserved" << endl;

  if (connect) {
    // connect to first sending a request
    std::string first_request;
    auto connected_responder = responder.connect(&first_request);

    cout << "Connected to "
         << MinimalSocket::to_string(connected_responder.getRemoteAddress())
         << endl;

    const auto &first_response =
        MinimalSocket::samples::NamesCircularIterator::NAMES_SURNAMES
            .find(first_request)
            ->second;
    connected_responder.send(first_response);

    MinimalSocket::samples::respond(connected_responder);
  } else {
    // use as un-connected udp
    MinimalSocket::samples::respond(responder);
  }

  return EXIT_SUCCESS;
}
