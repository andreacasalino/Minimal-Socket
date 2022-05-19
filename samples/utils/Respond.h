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
template <typename SocketT> void respond_forever(SocketT &channel) {
  while (true) {
    // receive name to search
    auto request = channel.receive(500);
    // respond with corresponding surname
    const auto &response =
        NamesCircularIterator::NAMES_SURNAMES.find(request)->second;
    channel.send(response);
  }
}

void respond_forever(MinimalSocket::udp::UdpBinded &channel) {
  while (true) {
    // receive name to search
    auto request = channel.receive(500);
    // respond with corresponding surname
    const auto &response =
        NamesCircularIterator::NAMES_SURNAMES.find(request->received_message)
            ->second;
    channel.sendTo(response, request->sender);
  }
}
} // namespace MinimalSocket::samples
