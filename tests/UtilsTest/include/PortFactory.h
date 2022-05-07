/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>
#include <mutex>

namespace MinimalSocket::test {
class PortFactory {
public:
  PortFactory();

  Port makePort();

private:
  std::mutex port_mtx;
  Port port;
};
} // namespace MinimalSocket::test
