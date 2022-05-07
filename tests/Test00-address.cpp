#include <gtest/gtest.h>

#include <MinimalSocket/core/Address.h>

using namespace MinimalSocket;

TEST(Address, ParseIpv4) {
  {
    // valid addresses
    std::vector<std::string> addresses = {"192.168.125.34", "127.0.0.1"};
    for (const auto &address : addresses) {
      auto converted = Address::makeAddress(address, 100);
      EXPECT_FALSE(nullptr == converted);
    }
  }
  {
    // invalid addresses
    std::vector<std::string> addresses = {"192.125.34.34.34", "10000.0.0.1"};
    for (const auto &address : addresses) {
      auto converted = Address::makeAddress(address, 100);
      EXPECT_TRUE(nullptr == converted);
    }
  }
}

TEST(Address, ParseIpv6) {
  {
    // valid addresses
    std::vector<std::string> addresses = {
        "2001:0db8:85a3:0000:0000:8a2e:0370:7334", "2001:db8::1:0"};
    for (const auto &address : addresses) {
      auto converted = Address::makeAddress(address, 100);
      EXPECT_FALSE(nullptr == converted);
    }
  }
  {
    // invalid addresses
    std::vector<std::string> addresses = {"192.125.db8::1:0"};
    for (const auto &address : addresses) {
      auto converted = Address::makeAddress(address, 100);
      EXPECT_TRUE(nullptr == converted);
    }
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
