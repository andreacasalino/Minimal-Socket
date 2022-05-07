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
  static Port makePort();

private:
  static std::mutex port_mtx;
  static Port port;
};
} // namespace MinimalSocket::test
