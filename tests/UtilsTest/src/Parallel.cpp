/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Parallel.h>
#include <omp.h>
#include <stdexcept>

namespace MinimalSocket::test {
void parallel(const Task &master, const Task &slave) {
#pragma omp parallel num_threads(2)
  {
    if (0 == omp_get_thread_num()) {
      master();
    } else {
      slave();
    }
  }
}

void parallel(const std::vector<Task> &tasks) {
  int threads = tasks.size();
  if (threads < 2) {
    throw std::runtime_error{"invalid number of tasks for parallel region"};
  }

#pragma omp parallel num_threads(threads)
  {
    const auto th_id = omp_get_thread_num();
    tasks[th_id]();
  }
}
} // namespace MinimalSocket::test
