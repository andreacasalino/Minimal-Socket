/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/SocketContext.h>

namespace MinimalSocket {
Address RemoteAddressAware::getRemoteAddress() const {
  std::scoped_lock lock(remote_address_mtx);
  return remote_address;
}

RemoteAddressAware::RemoteAddressAware(const RemoteAddressAware &o)
    : remote_address(o.getRemoteAddress()) {}

RemoteAddressAware &RemoteAddressAware::operator=(const RemoteAddressAware &o) {
  this->remote_address = o.getRemoteAddress();
  return *this;
}

PortToBindAware::PortToBindAware(const PortToBindAware &o) { *this = o; }

PortToBindAware &PortToBindAware::operator=(const PortToBindAware &o) {
  this->port_to_bind = o.getPortToBind();
  this->must_be_free_port = o.shallBeFreePort();
  return *this;
}

RemoteAddressAware::RemoteAddressAware(const Address &address)
    : remote_address(address) {
  if (nullptr == getRemoteAddress()) {
    throw Error{"Invalid address"};
  }
}

RemoteAddressFamilyAware::RemoteAddressFamilyAware(
    const RemoteAddressFamilyAware &o) {
  *this = o;
}

RemoteAddressFamilyAware &
RemoteAddressFamilyAware::operator=(const RemoteAddressFamilyAware &o) {
  this->remote_address_family = o.getRemoteAddressFamily();
  return *this;
}
} // namespace MinimalSocket
