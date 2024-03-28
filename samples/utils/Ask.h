/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>

#include <Names.h>
#include <TimeOfDay.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace MinimalSocket::samples {
void ask_connected(ReceiverBlocking &receiver, Sender &sender,
                   const std::chrono::milliseconds &rate, std::size_t cycles) {
  NamesCircularIterator iterator;
  for (std::size_t k = 0;
       k < cycles * NamesCircularIterator::NAMES_SURNAMES.size(); ++k) {
    // send name of this person
    std::cout << TimeOfDay{} << "Sending: " << iterator.current()->first
              << std::endl;
    sender.send(iterator.current()->first);
    // expect to get back the corresponding surname
    auto response = receiver.receive(500);
    std::cout << TimeOfDay{} << "Got response: " << response << std::endl;
    iterator.next();
    std::this_thread::sleep_for(rate);
  }
}

void ask_disconnected(ReceiverUnkownSenderBlocking &receiver, SenderTo &sender,
                      const MinimalSocket::Address &target,
                      const std::chrono::milliseconds &rate,
                      std::size_t cycles) {
  NamesCircularIterator iterator;
  for (std::size_t k = 0;
       k < cycles * NamesCircularIterator::NAMES_SURNAMES.size(); ++k) {
    // send name of this person
    std::cout << TimeOfDay{} << "Sending: " << iterator.current()->first
              << std::endl;
    sender.sendTo(iterator.current()->first, target);
    // expect to get back the corresponding surname
    auto response = receiver.receive(500);
    std::cout << TimeOfDay{} << "From "
              << MinimalSocket::to_string(response->sender)
              << " , got as response: " << response->received_message
              << std::endl;
    iterator.next();
    std::this_thread::sleep_for(rate);
  }
}

template <typename SocketT>
void ask(SocketT &socket, const std::chrono::milliseconds &rate,
         std::size_t cycles) {
  ask_connected(socket, socket, rate, cycles);
}

template <typename SocketT>
void ask(SocketT &socket, const MinimalSocket::Address &target,
         const std::chrono::milliseconds &rate, std::size_t cycles) {
  ask_disconnected(socket, socket, target, rate, cycles);
}

} // namespace MinimalSocket::samples
