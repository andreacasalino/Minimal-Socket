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
#include <TimeOfDay.h>

#include <vector>

using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Udp responder  -----------------------"
       << endl;
  PARSE_ARGS

  const auto port_A = options->getValue<MinimalSocket::Port>("port_A");
  const auto port_B = options->getValue<MinimalSocket::Port>("port_B");
  const auto family = options->getValue<MinimalSocket::AddressFamily>(
      "family", MinimalSocket::AddressFamily::IP_V4);

  vector<MinimalSocket::udp::Udp<false>> responders;
  responders.emplace_back(port_A, family);
  responders.emplace_back(port_B, family);
  for (auto &socket : responders) {
    if (!socket.open()) {
      cerr << "Failed to reserve one of the port" << endl;
      return EXIT_FAILURE;
    }
  }
  cout << "Ports successfully reserved" << endl;

  MinimalSocket::samples::Pollables pollables;

  auto make_pollable_responder = [](MinimalSocket::udp::Udp<false> &responder) {
    return [&responder]() {
      auto request = responder.receive(500);
      if (!request.has_value()) {
        return MinimalSocket::samples::PollableStatus::NOT_ADVANCED;
      }
      const auto &response =
          MinimalSocket::samples::NamesCircularIterator::NAMES_SURNAMES
              .find(request->received_message)
              ->second;
      cout << MinimalSocket::samples::TimeOfDay{}
           << " received: " << request->received_message
           << " from: " << MinimalSocket::to_string(request->sender)
           << " ; sending: " << response << endl;
      responder.sendTo(response, request->sender);
      return MinimalSocket::samples::PollableStatus::ADVANCED;
    };
  };

  for (auto &socket : responders) {
    pollables.emplace(make_pollable_responder(socket));
  }

  pollables.loop(std::chrono::seconds{5});

  return EXIT_SUCCESS;
}
