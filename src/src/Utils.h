/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

#include <future>

namespace MinimalSocket {
template <typename Ipv4Pred, typename Ipv6Pred>
void visitAddress(AddressFamily family, Ipv4Pred ipv4_case,
                  Ipv6Pred ipv6_case) {
  switch (family) {
  case AddressFamily::IP_V4:
    ipv4_case();
    break;
  case AddressFamily::IP_V6:
    ipv6_case();
    break;
  default:
    throw Error{"Unrecognized AddressFamily"};
    break;
  }
}

// rethrow exception if happens
// throw timeout excpetion if timeout reached
template <typename TryAction, typename RecoverAction>
void try_within_timeout(TryAction action_to_try,
                        RecoverAction action_to_recover,
                        const Timeout &timeout) {
  if (NULL_TIMEOUT == timeout) {
    throw Error{"Invalid timeout"};
  }
  auto task = std::async(action_to_try);
  auto task_status = task.wait_for(timeout);
  if (task_status == std::future_status::ready) {
    task.get(); // will throw if ready because an exception throwned
                // before timeout
  } else {
    try {
      action_to_recover();
      task.get();
    } catch (...) {
    }
    throw TimeOutError{};
  }
}

template <typename T> void copy_as(T &recipient, const T &giver) {
  recipient = giver;
}
} // namespace MinimalSocket
