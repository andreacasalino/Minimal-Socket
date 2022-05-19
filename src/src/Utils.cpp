/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "Utils.h"

#include <future>

namespace MinimalSocket {
void visitAddress(const AddressFamily &family,
                  const std::function<void()> &ipv4_case,
                  const std::function<void()> &ipv6_case) {
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

void try_within_timeout(const std::function<void()> &action_to_try,
                        const std::function<void()> &action_to_abort,
                        const Timeout &timeout) {
  if (NULL_TIMEOUT == timeout) {
    throw Error{"Invalid timeout"};
  }
  auto open_task = std::async([&]() { action_to_try(); });
  auto open_task_status = open_task.wait_for(timeout);
  if (open_task_status == std::future_status::ready) {
    open_task.get(); // will throw if ready because an exception throwned
                     // before timeout
  } else {
    try {
      action_to_abort();
      open_task.get();
    } catch (...) {
    }
    throw TimeOutError{};
  }
}
} // namespace MinimalSocket
