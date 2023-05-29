/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <barrier>
#include <functional>
#include <optional>
#include <thread>
#include <vector>

namespace MinimalSocket::test {
namespace detail {
    struct NullCompletion {
        void operator()() noexcept {}
    };
}
using Barrier = std::barrier<detail::NullCompletion>;

using Task = std::function<void(Barrier &)>;

class ParallelSection {
public:
  ParallelSection() = default;

  template <typename Pred> ParallelSection &add(Pred &&pred) {
    tasks.emplace_back(std::forward<Pred>(pred));
    return *this;
  }

  void run();

  template <typename First, typename Second>
  static void biSection(First &&first, Second &&second) {
    ParallelSection temp;
    temp.add(std::forward<First>(first))
        .add(std::forward<Second>(second))
        .run();
  }

private:
  std::optional<Barrier> barrier;

  std::vector<Task> tasks;
};

} // namespace MinimalSocket::test
