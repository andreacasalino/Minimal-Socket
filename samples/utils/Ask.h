/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/udp/UdpSocket.h>

#include <Names.h>
#include <TimeOfDay.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace MinimalSocket::samples {
template <typename SocketT>
void ask(SocketT &channel, const std::chrono::milliseconds &rate,
         std::size_t cycles) {
  NamesCircularIterator iterator;
  for (std::size_t k = 0; k < cycles * NamesCircularIterator::size(); ++k) {
    // send name of this person
    std::cout << TimeOfDay{} << "Sending: " << iterator.current()->first
              << std::endl;
    channel.send(iterator.current()->first);
    // expect to get back the corresponding surname
    auto response = channel.receive(500);
    std::cout << TimeOfDay{} << "Got response: " << response << std::endl;
    iterator.next();
    std::this_thread::sleep_for(rate);
  }
}

void ask(MinimalSocket::udp::UdpBinded &channel,
         const MinimalSocket::Address &target,
         const std::chrono::milliseconds &rate, std::size_t cycles) {
  NamesCircularIterator iterator;
  for (std::size_t k = 0; k < cycles * NamesCircularIterator::size(); ++k) {
    // send name of this person
    std::cout << TimeOfDay{} << "Sending: " << iterator.current()->first
              << std::endl;
    channel.sendTo(iterator.current()->first, target);
    // expect to get back the corresponding surname
    auto response = channel.receive(500);
    std::cout << TimeOfDay{} << "From "
              << MinimalSocket::to_string(response->sender)
              << " , got as response: " << response->received_message
              << std::endl;
    iterator.next();
    std::this_thread::sleep_for(rate);
  }
}
} // namespace MinimalSocket::samples
