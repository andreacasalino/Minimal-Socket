#pragma once

#include <chrono>
#include <functional>
#include <list>

namespace MinimalSocket::samples {
enum class PollableStatus { NOT_ADVANCED, ADVANCED, COMPLETED };

using Pollable = std::function<PollableStatus()>;

class Pollables {
public:
  Pollables() = default;

  template <typename Pred> void emplace(Pred &&pred) {
    pollables_.emplace_back(std::forward<Pred>(pred));
  }

  // At each iteration, it polls one by one all the pollables.
  // If there is no progress for a time period higher than the specified
  // timeout, the function completes and return to the caller.
  void loop(const std::chrono::seconds &timeout);

private:
  std::list<Pollable> pollables_;
};

} // namespace MinimalSocket::samples
