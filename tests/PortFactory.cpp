/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "PortFactory.h"

namespace MinimalSocket::test {
PortFactory &PortFactory::get() {
  static PortFactory res = PortFactory{};
  return res;
}

namespace {
static constexpr std::uint16_t INITIAL_PORT = 9999;
static constexpr std::uint16_t DELTA_PORT = 10;
} // namespace

Port PortFactory::makePort() {
  std::lock_guard<std::mutex> lock(port_mtx);
  auto result = port;
  port += DELTA_PORT;
  return result;
}

PortFactory::PortFactory() : port{INITIAL_PORT} {}
} // namespace MinimalSocket::test
