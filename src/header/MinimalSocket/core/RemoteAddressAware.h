/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>

namespace MinimalSocket {
class RemoteAddressAware {
public:
  const Address &getRemoteAddress() const { return remote_address; }

protected:
  RemoteAddressAware(const Address &address) : remote_address(address){};

private:
  const Address remote_address;
};
} // namespace MinimalSocket
