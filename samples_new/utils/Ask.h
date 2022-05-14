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
template <typename SocketT>
void ask(SocketT &channel, NamesCircularIterator &iterator) {
  // send name of this person
  std::cout << "Sending: " << iterator.current()->name;
  channel.send(iterator.current()->first);
  // expect to get back the corresponding surname
  auto response = channel.receive(500);
  std::cout << " , got as response: " << response;
  iterator.next();
}
} // namespace

template <typename SocketT>
void ask(SocketT &channel, const std::size_t times) {
  NamesCircularIterator iterator;
  for (std::size_t k = 0; k < times; ++k) {
    ask(channel, iterator);
  }
}

template <typename SocketT> void ask_forever(SocketT &channel) {
  NamesCircularIterator iterator;
  while (true) {
    ask(channel, iterator);
  }
}
} // namespace MinimalSocket::samples
