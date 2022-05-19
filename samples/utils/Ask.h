/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/udp/UdpSocket.h>

#include <Names.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace MinimalSocket::samples {
template <typename SocketT>
void ask_forever(SocketT &channel, const std::chrono::milliseconds &rate) {
  NamesCircularIterator iterator;
  while (true) {
    // send name of this person
    std::cout << "Sending: " << iterator.current()->first;
    channel.send(iterator.current()->first);
    // expect to get back the corresponding surname
    auto response = channel.receive(500);
    std::cout << " , got as response: " << response << std::endl;
    iterator.next();
    std::this_thread::sleep_for(rate);
  }
}

void ask_forever(MinimalSocket::udp::UdpBinded &channel,
                 const MinimalSocket::Address &target,
                 const std::chrono::milliseconds &rate) {
  NamesCircularIterator iterator;
  while (true) {
    // send name of this person
    std::cout << "Sending: " << iterator.current()->first;
    channel.sendTo(iterator.current()->first, target);
    // expect to get back the corresponding surname
    auto response = channel.receive(500);
    std::cout << "From " << MinimalSocket::to_string(response->sender)
              << " , got as response: " << response->received_message
              << std::endl;
    iterator.next();
    std::this_thread::sleep_for(rate);
  }
}
} // namespace MinimalSocket::samples
