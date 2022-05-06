/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>

namespace MinimalSocket {
class BindedPortAware {
public:
  Port getBindedPort() const { return binded_port; }

protected:
  BindedPortAware(const Port &port) : binded_port(port){};

private:
  const Port binded_port;
};
} // namespace MinimalSocket
