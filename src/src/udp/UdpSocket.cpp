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
UdpBase::UdpBase(Port port_to_bind, AddressFamily accepted_connection_family,
                 bool blockMode)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_connection_family) {
  if (!blockMode) {
    setNonBlocking();
  }
}

UdpBase::UdpBase(UdpBase &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  this->steal(o);
}

void UdpBase::stealBase(UdpBase &o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
  this->steal(o);
}

void UdpBase::open_() {
  getHandler().reset(SocketType::UDP, getRemoteAddressFamily());
  auto binded_port =
      MinimalSocket::bind(getHandler().accessId(), getRemoteAddressFamily(),
                          getPortToBind(), shallBeFreePort());
  setPort(binded_port);
  this->Socket::setUp();
}

UdpConnected<true> UdpBlocking::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnected<true> result(remote_address, getPortToBind());
  if (wasOpened()) {
    MinimalSocket::connect(getHandler().accessId(), remote_address);
  }
  this->transfer(result);
  return result;
}

UdpConnected<true> UdpBlocking::connect(std::string *initial_message) {
  auto result = this->connect(NULL_TIMEOUT, initial_message);
  return std::move(result.value());
}

std::optional<UdpConnected<true>>
UdpBlocking::connect(const Timeout &timeout, std::string *initial_message) {
  auto maybe_received = this->receive(MAX_UDP_RECV_MESSAGE, timeout);
  if (!maybe_received) {
    return std::nullopt;
  }
  if (nullptr != initial_message) {
    *initial_message = std::move(maybe_received->received_message);
  }
  return connect(maybe_received->sender);
}

UdpConnected<false> UdpNonBlocking::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnected<false> result(remote_address, getPortToBind());
  if (wasOpened()) {
    MinimalSocket::connect(getHandler().accessId(), remote_address);
  }
  this->transfer(result);
  return result;
}

std::optional<UdpConnected<false>>
UdpNonBlocking::connect(std::string *initial_message) {
  auto maybe_received = this->receive(MAX_UDP_RECV_MESSAGE);
  if (!maybe_received) {
    return std::nullopt;
  }
  if (nullptr != initial_message) {
    *initial_message = std::move(maybe_received->received_message);
  }
  return connect(maybe_received->sender);
}

UdpConnectedBase::UdpConnectedBase(const Address &remote_address, Port port,
                                   bool blockMode)
    : PortToBindAware(port), RemoteAddressAware(remote_address) {
  if (!blockMode) {
    setNonBlocking();
  }
}

UdpConnectedBase::UdpConnectedBase(UdpConnectedBase &&o)
    : PortToBindAware(o), RemoteAddressAware(o) {
  this->steal(o);
}

void UdpConnectedBase::stealBase(UdpConnectedBase &o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressAware>(*this, o);
  this->steal(o);
}

void UdpConnectedBase::open_() {
  const auto &remote_address = getRemoteAddress();
  getHandler().reset(SocketType::UDP, remote_address.getFamily());
  auto socket_id = getHandler().accessId();
  auto binded_port = MinimalSocket::bind(socket_id, remote_address.getFamily(),
                                         getPortToBind(), shallBeFreePort());
  setPort(binded_port);
  this->Socket::setUp();
  MinimalSocket::connect(socket_id, remote_address);
}

UdpConnected<true>
makeUdpConnectedToUnknown(Port port, AddressFamily accepted_connection_family,
                          std::string *initial_message) {
  Udp<true> primal_socket(port, accepted_connection_family);
  auto success = primal_socket.open();
  if (!success) {
    throw Error{"Unable to open the primal upd socket"};
  }
  return primal_socket.connect(initial_message);
}

} // namespace MinimalSocket::udp
