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

  // polls one by one the pollables at each iteration.
  // If there is no progress across a certain time window, the function
  // completes
  void loop(const std::chrono::seconds &timeout);

private:
  std::list<Pollable> pollables_;
};

} // namespace MinimalSocket::samples
