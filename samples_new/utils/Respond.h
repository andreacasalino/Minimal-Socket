/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <Names.h>
#include <iostream>

namespace MinimalSocket::samples {
namespace {
template <typename SocketT> void respond_one(SocketT &channel) {
  // receive name to search
  std::cout << "Receiving ";
  auto request = channel.receive(500);
  std::cout << " , got as request: " << request;
  // respond with corresponding surname
  const auto &response =
      NamesCircularIterator::NAMES_SURNAMES.find(request)->second;
  std::cout << " , sending back: " << response;
  channel.send(response);
}
} // namespace

template <typename SocketT>
void respond(SocketT &channel, const std::size_t times) {
  for (std::size_t k = 0; k < times; ++k) {
    respond_one(channel);
  }
}

template <typename SocketT> void respond_forever(SocketT &channel) {
  while (true) {
    respond_one(channel);
  }
}
} // namespace MinimalSocket::samples
