#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <barrier>

TEST_CASE("foo", "[foo]") { std::barrier br{2}; }
