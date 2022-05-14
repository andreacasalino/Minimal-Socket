/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <Names.h>
#include <chrono>
#include <iostream>
#include <thread>

namespace MinimalSocket::samples {
namespace {
template <typename SocketT>
void ask(SocketT &channel, NamesCircularIterator &iterator,
         const std::chrono::milliseconds &rate) {
  // send name of this person
  std::cout << "Sending: " << iterator.current()->first;
  channel.send(iterator.current()->first);
  // expect to get back the corresponding surname
  auto response = channel.receive(500);
  std::cout << " , got as response: " << response << std::endl;
  iterator.next();
  std::this_thread::sleep_for(rate);
}
} // namespace

template <typename SocketT>
void ask(SocketT &channel, const std::size_t times,
         const std::chrono::milliseconds &rate) {
  NamesCircularIterator iterator;
  for (std::size_t k = 0; k < times; ++k) {
    ask(channel, iterator, rate);
  }
}

template <typename SocketT>
void ask_forever(SocketT &channel, const std::chrono::milliseconds &rate) {
  NamesCircularIterator iterator;
  while (true) {
    ask(channel, iterator, rate);
  }
}
} // namespace MinimalSocket::samples
