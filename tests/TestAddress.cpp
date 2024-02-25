#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

using namespace MinimalSocket;

namespace {
static constexpr Port TEST_PORT = 100;
}

#ifdef _WIN32
TEST_CASE("Invalid WSA version", "[address]") {
  WSAManager::setWsaVersion({0, 0});
  CHECK_THROWS_AS(Address("127.0.0.1", TEST_PORT), Error);
  WSAManager::setWsaVersion({2, 2});
  CHECK_NOTHROW(Address("127.0.0.1", TEST_PORT));
}
#endif

TEST_CASE("parse valid ipv4 hosts", "[address]") {
  auto host = GENERATE("192.168.125.34", "127.0.0.1", "0.0.0.0");
  Address converted(host, TEST_PORT);
  CHECK(converted.getFamily() == AddressFamily::IP_V4);
  CHECK(converted.getHost() == host);
  CHECK(converted.getPort() == TEST_PORT);
}

TEST_CASE("parse valid ipv6 hosts", "[address]") {
  auto host =
      GENERATE("2001:0db8:85a3:0000:0000:8a2e:0370:7334", "2001:db8::1:0",
               "0000:0000:0000:0000:0000:0000:0000:0001", "::1");
  Address converted(host, TEST_PORT);
  CHECK(converted.getFamily() == AddressFamily::IP_V6);
  CHECK(converted.getHost() == host);
  CHECK(converted.getPort() == TEST_PORT);
}

TEST_CASE("parse invalid hosts", "[address]") {
  auto host = GENERATE("192.125.34.34.34", "10000.0.0.1", "192.125.db8::1:0");
  CHECK_THROWS_AS(Address(host, TEST_PORT), Error);
}
