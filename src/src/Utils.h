/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Socket.h>

#include "SocketId.h"

#include <functional>

namespace MinimalSocket {
void visitAddress(const AddressFamily &family,
                  const std::function<void()> &ipv4_case,
                  const std::function<void()> &ipv6_case);

template <typename T, typename U> void copy_as(U &receiver, const U &giver) {
  T &receiver_ref = receiver;
  const T &giver_ref = giver;
  receiver_ref = giver_ref;
}

class TimeOutError : public Error {
public:
  TimeOutError() : Error("Timeout"){};
};

// rethrow exception if happens
// throw timeout excpetion if timeout reached
void try_within_timeout(const std::function<void()> &action_to_try,
                        const std::function<void()> &action_to_abort,
                        const Timeout &timeout);
} // namespace MinimalSocket
