#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <barrier>
#include <thread>

TEST_CASE("foo", "[foo]") {
  std::barrier br{2};

  std::thread t([&br]() { br.arrive_and_wait(); });

  br.arrive_and_wait();

  t.join();
}
