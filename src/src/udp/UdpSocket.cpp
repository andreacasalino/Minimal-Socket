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
UdpBinded::UdpBinded(const Port port_to_bind,
                     const AddressFamily &accepted_connection_family)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_connection_family) {}

UdpBinded::UdpBinded(UdpBinded &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  Openable::transfer(*this, o);
}
UdpBinded &UdpBinded::operator=(UdpBinded &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
  Openable::transfer(*this, o);
  return *this;
}

void UdpBinded::open_() {
  getIDWrapper().reset(UDP, getRemoteAddressFamily());
  auto binded_port = MinimalSocket::bind(
      getIDWrapper().accessId(), getRemoteAddressFamily(), getPortToBind());
  setPort(binded_port);
}

UdpConnected UdpBinded::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnected result(remote_address, getPortToBind());
  if (wasOpened()) {
    MinimalSocket::connect(getIDWrapper().accessId(), remote_address);
  }
  Openable::transfer(result, *this);
  return std::move(result);
}

std::optional<UdpConnected> UdpBinded::connect(const Timeout &timeout) {
  auto maybe_received = this->receive(MAX_UDP_RECV_MESSAGE, timeout);
  if (!maybe_received) {
    return std::nullopt;
  }
  return connect(maybe_received->sender);
}

UdpConnected::UdpConnected(const Address &remote_address, const Port &port)
    : PortToBindAware(port), RemoteAddressAware(remote_address) {}

UdpConnected::UdpConnected(UdpConnected &&o)
    : PortToBindAware(o), RemoteAddressAware(o) {
  Openable::transfer(*this, o);
}
UdpConnected &UdpConnected::operator=(UdpConnected &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressAware>(*this, o);
  Openable::transfer(*this, o);
  return *this;
}

void UdpConnected::open_() {
  const auto &socket_id = getIDWrapper().accessId();
  const auto &remote_address = getRemoteAddress();
  getIDWrapper().reset(UDP, remote_address.getFamily());
  auto binded_port = MinimalSocket::bind(socket_id, remote_address.getFamily(),
                                         getPortToBind());
  setPort(binded_port);
  MinimalSocket::connect(socket_id, remote_address);
}

UdpBinded UdpConnected::disconnect() {
  resetIDWrapper();
  UdpBinded result(getPortToBind(), getRemoteAddress().getFamily());
  result.open();
  return std::move(result);
}

UdpConnected makeUdpConnected(const Port &port,
                              const AddressFamily &accepted_connection_family,
                              const Timeout &timeout) {
  UdpBinded primal_socket(port, accepted_connection_family);
  primal_socket.open();
  auto maybe_result = primal_socket.connect(timeout);
  if (!maybe_result) {
    throw Error{"Something went wrong creating a UdpConnected socket"};
  }
  return std::move(maybe_result.value());
}
} // namespace MinimalSocket::udp
