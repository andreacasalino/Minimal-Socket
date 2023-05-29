#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <iostream>
#include <mutex>
#include <omp.h>

TEST_CASE("foo", "[foo]") {
  std::mutex cout_mtx;

#pragma omp parallel num_threads(2)
  {
    std::scoped_lock lk(cout_mtx);
    if (0 == omp_get_thread_num()) {
      std::cout << "from main" << std::endl;
    } else {
      std::cout << "from slave" << std::endl;
    }
  }
}
