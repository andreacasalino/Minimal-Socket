/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/udp/UdpSocket.h>

#include "../SocketFunctions.h"
#include "../Utils.h"

namespace MinimalSocket::udp {
UdpSender::UdpSender(const AddressFamily &accepted_connection_family)
    : RemoteAddressFamilyAware(accepted_connection_family) {
  getIDWrapper().reset(UDP, accepted_connection_family);
}

UdpSender::UdpSender(UdpSender &&o)
    : RemoteAddressFamilyAware(o.getRemoteAddressFamily()) {
  Socket::transfer(*this, o);
}
UdpSender &UdpSender::operator=(UdpSender &&o) {
  copy_as<RemoteAddressFamilyAware>(*this, o);
  Socket::transfer(*this, o);
  return *this;
}

UdpBindable UdpSender::bind(const Port port_to_bind) {
  UdpBindable result(port_to_bind, getRemoteAddressFamily());
  result.open();
  return std::move(result);
}

UdpBindable::UdpBindable(const Port port_to_bind,
                         const AddressFamily &accepted_connection_family)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_connection_family) {}

UdpBindable::UdpBindable(UdpBindable &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  Openable::transfer(*this, o);
}
UdpBindable &UdpBindable::operator=(UdpBindable &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
  Openable::transfer(*this, o);
  return *this;
}

void UdpBindable::open_() {
  getIDWrapper().reset(UDP, getRemoteAddressFamily());
  MinimalSocket::bind(getIDWrapper().accessId(), getRemoteAddressFamily(),
                      getPortToBind());
}

UdpConnectable UdpBindable::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnectable result(getPortToBind(), remote_address);
  if (wasOpened()) {
    MinimalSocket::connect(getIDWrapper().accessId(), remote_address);
  }
  Openable::transfer(result, *this);
  return std::move(result);
}

UdpConnectable UdpBindable::connect() {
  std::string buffer;
  buffer.resize(MAX_UDP_RECV_MESSAGE);
  auto sender_address = this->receive(buffer);
  return connect(sender_address);
}

UdpConnectable::UdpConnectable(const Port &port, const Address &remote_address)
    : PortToBindAware(port), RemoteAddressAware(remote_address) {}

UdpConnectable::UdpConnectable(UdpConnectable &&o)
    : PortToBindAware(o), RemoteAddressAware(o) {
  Openable::transfer(*this, o);
}
UdpConnectable &UdpConnectable::operator=(UdpConnectable &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressAware>(*this, o);
  Openable::transfer(*this, o);
  return *this;
}

void UdpConnectable::open_() {
  const auto &socket_id = getIDWrapper().accessId();
  const auto &remote_address = getRemoteAddress();
  getIDWrapper().reset(UDP, remote_address.getFamily());
  MinimalSocket::bind(socket_id, remote_address.getFamily(), getPortToBind());
  MinimalSocket::connect(socket_id, remote_address);
}

UdpBindable UdpConnectable::disconnect() {
  resetIDWrapper();
  UdpBindable result(getPortToBind(), getRemoteAddress().getFamily());
  result.open();
  return std::move(result);
}
} // namespace MinimalSocket::udp
