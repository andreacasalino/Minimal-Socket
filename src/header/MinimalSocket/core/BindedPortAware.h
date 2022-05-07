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
  Port getPortToBind() const { return binded_port; }

  BindedPortAware(const BindedPortAware &) = default;
  BindedPortAware &operator=(const BindedPortAware &) = default;

protected:
  BindedPortAware(const Port &port) : binded_port(port){};

private:
  Port binded_port;
};
} // namespace MinimalSocket
