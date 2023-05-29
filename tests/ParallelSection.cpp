/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "ParallelSection.h"
#include <stdexcept>

namespace MinimalSocket::test {
namespace {
std::function<void()> make_thread(Barrier &br, const Task &task) {
  return [&task = task, &br = br]() mutable {
    br.arrive_and_wait();
    task(br);
  };
}
} // namespace

void ParallelSection::run() {
  if (tasks.size() < 2) {
    throw std::runtime_error{"invalid number of tasks for parallel region"};
  }
  barrier.emplace(tasks.size());
  std::vector<std::thread> spinners;
  for (auto it = tasks.begin(); it != tasks.end() - 1; ++it) {
    spinners.emplace_back(make_thread(barrier.value(), *it));
  }
  spinners.emplace_back(make_thread(barrier.value(), tasks.back()));
  for (auto &sp : spinners) {
    sp.join();
  }
}
} // namespace MinimalSocket::test
