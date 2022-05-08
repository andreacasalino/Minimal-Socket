/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/udp/UdpSocket.h>

#include "../SocketFunctions.h"

namespace MinimalSocket::udp {
UdpSender::UdpSender(const AddressFamily &accepted_connection_family)
    : RemoteAddressFamilyAware(accepted_connection_family) {
  getIDWrapper().reset(UDP, accepted_connection_family);
}

UdpBindable UdpSender::bind(const Port port_to_bind) {
  UdpBindable result(port_to_bind, getRemoteAddressFamily());
  Socket::transferIDWrapper(*this, result);
  result.open();
  return std::move(result);
}

UdpSender::UdpSender(UdpSender &&o)
    : RemoteAddressFamilyAware(o.getRemoteAddressFamily()) {
  Socket::transferIDWrapper(o, *this);
}
UdpSender &UdpSender::operator=(UdpSender &&o) {
  Socket::transferIDWrapper(o, *this);
  static_cast<RemoteAddressFamilyAware &>(*this) = o;
  return *this;
}

UdpBindable::UdpBindable(const Port port_to_bind,
                         const AddressFamily &accepted_connection_family)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_connection_family) {
  getIDWrapper().reset(UDP, accepted_connection_family);
}

void UdpBindable::open_() {
  MinimalSocket::bind(getIDWrapper().accessId(), getRemoteAddressFamily(),
                      getPortToBind());
}

UdpConnectable UdpBindable::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnectable result(getPortToBind(), remote_address);
  Socket::transferIDWrapper(*this, result);
  MinimalSocket::connect(getIDWrapper().accessId(), remote_address);
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

UdpBindable UdpConnectable::disconnect() {
  destroyIDWrapper();
  UdpBindable result(getPortToBind(), getRemoteAddress().getFamily());
  result.open();
  return std::move(result);
}

UdpConnectable::UdpConnectable(UdpConnectable &&o)
    : PortToBindAware(o), RemoteAddressAware(o) {
  Socket::transferIDWrapper(o, *this);
}
UdpConnectable &UdpConnectable::operator=(UdpConnectable &&o) {
  Socket::transferIDWrapper(o, *this);
  static_cast<PortToBindAware &>(*this) = o;
  static_cast<RemoteAddressAware &>(*this) = o;
  return *this;
}

void UdpConnectable::open_() {
  const auto &socket_id = getIDWrapper().accessId();
  const auto &remote_address = getRemoteAddress();
  MinimalSocket::bind(socket_id, remote_address.getFamily(), getPortToBind());
  MinimalSocket::connect(socket_id, remote_address);
}
} // namespace MinimalSocket::udp
