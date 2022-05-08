/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include "SocketId.h"

#include <functional>

namespace MinimalSocket {
void visitAddress(const AddressFamily &family,
                  const std::function<void()> &ipv4_case,
                  const std::function<void()> &ipv6_case);

template <typename T, typename Giver, typename Receiver>
void move_as(Giver &giver, Receiver &receiver) {
  static_cast<T &>(receiver) = std::move(static_cast<T &>(giver));
}

template <typename T, typename Giver, typename Receiver>
void copy_as(const Giver &giver, Receiver &receiver) {
  static_cast<T &>(receiver) = static_cast<const T &>(giver);
}
} // namespace MinimalSocket
