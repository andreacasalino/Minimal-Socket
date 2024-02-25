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
#include <Pollables.h>
#include <Respond.h>
using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Udp responder  -----------------------"
       << endl;
  PARSE_ARGS

  const auto port_this = options->getValue<MinimalSocket::Port>("port_this");
  const auto family = options->getValue<MinimalSocket::AddressFamily>(
      "family", MinimalSocket::AddressFamily::IP_V4);

  MinimalSocket::udp::Udp<true> responder(port_this, family);

  if (!responder.open()) {
    cerr << "Failed to reserve specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Port successfully reserved" << endl;

  MinimalSocket::samples::Pollables pollables;

  pollables.emplace([&responder]() {
    auto request = responder.receive(500);
    if (!request.has_value()) {
      return MinimalSocket::samples::PollableStatus::NOT_ADVANCED;
    }
    const auto &response =
        MinimalSocket::samples::NamesCircularIterator::NAMES_SURNAMES
            .find(request->received_message)
            ->second;
    responder.sendTo(response, request->sender);
    return MinimalSocket::samples::PollableStatus::ADVANCED;
  });

  pollables.loop(std::chrono::seconds{5});

  return EXIT_SUCCESS;
}
