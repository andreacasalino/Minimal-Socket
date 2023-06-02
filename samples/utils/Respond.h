/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/udp/UdpSocket.h>

#include <Names.h>

namespace MinimalSocket::samples {
template <typename SocketT> void respond(SocketT &channel) {
  while (true) {
    // receive name to search
    auto request = channel.receive(500, std::chrono::seconds{5});
    // respond with corresponding surname
    if constexpr (std::is_same<SocketT, MinimalSocket::udp::UdpBinded>::value) {
      if (!request.has_value()) {
        break;
      }
      const auto &response =
          NamesCircularIterator::NAMES_SURNAMES.find(request->received_message)
              ->second;
      channel.sendTo(response, request->sender);
    } else {
      if (request.empty()) {
        break;
      }
      const auto &response =
          NamesCircularIterator::NAMES_SURNAMES.find(request)->second;
      channel.send(response);
    }
  }
}
} // namespace MinimalSocket::samples
