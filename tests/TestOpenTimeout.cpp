#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <MinimalSocket/core/Socket.h>

#include "IsTimeout.h"

#include <thread>

using namespace MinimalSocket;

namespace {
class OpenableTest : public Openable {
public:
  OpenableTest(const Timeout &duration) : open_duration(duration) {}

protected:
  void open_() final { std::this_thread::sleep_for(open_duration); };

private:
  const Timeout open_duration;
};
} // namespace

TEST_CASE("Simulate open with timeout", "[open]") {
  auto open_time = Timeout{500};
  OpenableTest test(open_time);

  SECTION("expected success") {
    CHECK_FALSE(test.open(Timeout{1000}));
    CHECK(test.wasOpened());
  }

  SECTION("expected failure") {
    auto err = test.open(Timeout{250});
    CHECK(err);
    CHECK(test::is_timeout(err.get()));
    CHECK_FALSE(test.wasOpened());
  }
}
