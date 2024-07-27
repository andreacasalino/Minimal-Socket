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
UdpBinded::UdpBinded(Port port_to_bind,
                     AddressFamily accepted_connection_family)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_connection_family) {}

UdpBinded::UdpBinded(UdpBinded &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  this->steal(o);
}
UdpBinded &UdpBinded::operator=(UdpBinded &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
  this->steal(o);
  return *this;
}

void UdpBinded::open_() {
  getHandler().reset(SocketType::UDP, getRemoteAddressFamily());
  auto binded_port =
      MinimalSocket::bind(getHandler().accessId(), getRemoteAddressFamily(),
                          getPortToBind(), shallBeFreePort());
  setPort(binded_port);
}

UdpConnected UdpBinded::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnected result(remote_address, getPortToBind());
  if (wasOpened()) {
    MinimalSocket::connect(getHandler().accessId(), remote_address);
  }
  this->transfer(result);
  return std::move(result);
}

UdpConnected UdpBinded::connect(std::string *initial_message) {
  auto result = this->connect(NULL_TIMEOUT, initial_message);
  return std::move(result.value());
}

std::optional<UdpConnected> UdpBinded::connect(const Timeout &timeout,
                                               std::string *initial_message) {
  auto maybe_received = this->receive(MAX_UDP_RECV_MESSAGE, timeout);
  if (!maybe_received) {
    return std::nullopt;
  }
  if (nullptr != initial_message) {
    *initial_message = std::move(maybe_received->received_message);
  }
  return connect(maybe_received->sender);
}

UdpConnected::UdpConnected(const Address &remote_address, Port port)
    : PortToBindAware(port), RemoteAddressAware(remote_address) {}

UdpConnected::UdpConnected(UdpConnected &&o)
    : PortToBindAware(o), RemoteAddressAware(o) {
  this->steal(o);
}
UdpConnected &UdpConnected::operator=(UdpConnected &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressAware>(*this, o);
  this->steal(o);
  return *this;
}

void UdpConnected::open_() {
  const auto &remote_address = getRemoteAddress();
  getHandler().reset(SocketType::UDP, remote_address.getFamily());
  auto socket_id = getHandler().accessId();
  auto binded_port = MinimalSocket::bind(socket_id, remote_address.getFamily(),
                                         getPortToBind(), shallBeFreePort());
  setPort(binded_port);
  MinimalSocket::connect(socket_id, remote_address);
}

UdpBinded UdpConnected::disconnect() {
  resetHandler();
  UdpBinded result(getPortToBind(), getRemoteAddress().getFamily());
  result.open();
  return std::move(result);
}

UdpConnected makeUdpConnectedToUnknown(Port port,
                                       AddressFamily accepted_connection_family,
                                       std::string *initial_message) {
  auto result = makeUdpConnectedToUnknown(port, accepted_connection_family,
                                          NULL_TIMEOUT, initial_message);
  return std::move(result.value());
}

std::optional<UdpConnected>
makeUdpConnectedToUnknown(Port port, AddressFamily accepted_connection_family,
                          const Timeout &timeout,
                          std::string *initial_message) {
  UdpBinded primal_socket(port, accepted_connection_family);
  auto success = primal_socket.open();
  if (!success) {
    return std::nullopt;
  }
  return primal_socket.connect(timeout, initial_message);
}
} // namespace MinimalSocket::udp
