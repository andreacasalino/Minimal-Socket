/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/SocketContext.h>

namespace MinimalSocket {
PortToBindAware::PortToBindAware(const PortToBindAware &o) { *this = o; }

PortToBindAware &PortToBindAware::operator=(const PortToBindAware &o) {
  this->port_to_bind = o.port_to_bind;
  this->must_be_free_port = o.shallBeFreePort();
  return *this;
}

RemoteAddressAware::RemoteAddressAware(const Address &address)
    : remote_address(address) {
  if (nullptr == getRemoteAddress()) {
    throw Error{"Invalid address"};
  }
};
} // namespace MinimalSocket
