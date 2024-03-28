/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "ParallelSection.h"

#include <stdexcept>
#include <algorithm>

namespace MinimalSocket::test {
namespace {
std::function<void()> make_task(Barrier &br, const Task &task) {
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
  auto &br = barrier.emplace(tasks.size());
  std::vector<std::thread> spinners;
  std::for_each(tasks.begin() + 1, tasks.end(), [&](const Task &t) {
    spinners.emplace_back(make_task(br, t));
  });
  make_task(br, tasks.front())();
  for (auto &sp : spinners) {
    sp.join();
  }
}
} // namespace MinimalSocket::test
