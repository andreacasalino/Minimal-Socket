/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include "../SocketAddress.h"
#include "../SocketFunctions.h"
#include "../Utils.h"

namespace MinimalSocket::tcp {
TcpConnectionBlocking::TcpConnectionBlocking(const Address &remote_address)
    : RemoteAddressAware{remote_address} {}

TcpConnectionBlocking::TcpConnectionBlocking(TcpConnectionBlocking &&o)
    : RemoteAddressAware(o) {
  this->steal(o);
}

TcpConnectionBlocking &
TcpConnectionBlocking::operator=(TcpConnectionBlocking &&o) {
  this->steal(o);
  copy_as<RemoteAddressAware>(*this, o);
  return *this;
}

TcpConnectionNonBlocking TcpConnectionBlocking::turnToNonBlocking() {
  return TcpConnectionNonBlocking{std::move(*this)};
}

TcpConnectionNonBlocking::TcpConnectionNonBlocking(TcpConnectionNonBlocking &&o)
    : RemoteAddressAware{o} {
  this->steal(o);
}

TcpConnectionNonBlocking &
TcpConnectionNonBlocking::operator=(TcpConnectionNonBlocking &&o) {
  this->steal(o);
  copy_as<RemoteAddressAware>(*this, o);
  return *this;
}

TcpConnectionNonBlocking::TcpConnectionNonBlocking(
    TcpConnectionBlocking &&connection)
    : RemoteAddressAware{connection} {
  this->steal(connection);
  turnToNonBlocking(getHandler().accessId());
}

TcpServerBase::TcpServerBase(TcpServerBase &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  this->steal(o);
}

void TcpServerBase::stealBase(TcpServerBase &o) {
  this->steal(o);
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
}

TcpServerBase::TcpServerBase(Port port_to_bind,
                             AddressFamily accepted_client_family,
                             bool block_mode)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_client_family) {
  if (!block_mode) {
    setNonBlocking();
  }
}

void TcpServerBase::open_() {
  auto &socket = getHandler();
  const auto port = getPortToBind();
  const auto family = getRemoteAddressFamily();
  socket.reset(SocketType::TCP, family);
  auto binded_port =
      MinimalSocket::bind(socket.accessId(), family, port, shallBeFreePort());
  setPort(binded_port);
  this->Socket::setUp();
  MinimalSocket::listen(socket.accessId(), client_queue_size);
}

void TcpServerBase::setClientQueueSize(const std::size_t queue_size) {
  if (wasOpened()) {
    throw Error{"Can't set client queue size of an alrady opened tcp server"};
  }
  client_queue_size = queue_size;
}

struct TcpServerBase::AcceptedSocket {
  SocketID fd = SCK_INVALID_SOCKET;
  SocketAddressLength address_length = MAX_POSSIBLE_ADDRESS_SIZE;
  char address[MAX_POSSIBLE_ADDRESS_SIZE];
};

void TcpServerBase::acceptClient_(AcceptedSocket &recipient) {
  auto &[accepted_client_socket_id, acceptedClientAddress_length,
         acceptedClientAddress] = recipient;

  // accept: wait for a client to call connect and hit this server and get a
  // pointer to this client.
  accepted_client_socket_id =
      ::accept(getHandler().accessId(),
               reinterpret_cast<SocketAddress *>(&acceptedClientAddress[0]),
               &acceptedClientAddress_length);
  checkResult(static_cast<int>(accepted_client_socket_id), SCK_INVALID_SOCKET,
              "accepting a new client", !isBlocking());
}

TcpConnectionBlocking
TcpServerBase::makeClient(const AcceptedSocket &acceptedSocket) {
  auto accepted_client_parsed_address = toAddress(
      reinterpret_cast<const SocketAddress &>(acceptedSocket.address));
  TcpConnectionBlocking result{accepted_client_parsed_address};
  result.getHandler().reset(acceptedSocket.fd);
  return result;
}

TcpConnectionBlocking AcceptorBlocking::acceptNewClient() {
  std::scoped_lock lock(accept_mtx);
  if (!this->wasOpened()) {
    throw Error("Tcp server was not opened before starting to accept clients");
  }

  AcceptedSocket acceptedSocket;
  acceptClient_(acceptedSocket);

  return makeClient(acceptedSocket);
}

std::optional<TcpConnectionBlocking> AcceptorNonBlocking::acceptNewClient() {
  std::scoped_lock lock(accept_mtx);
  if (!this->wasOpened()) {
    throw Error("Tcp server was not opened before starting to accept clients");
  }

  AcceptedSocket acceptedSocket;
  acceptClient_(acceptedSocket);

  if (acceptedSocket.fd == SCK_INVALID_SOCKET) {
    return std::nullopt;
  }
  return makeClient(acceptedSocket);
}

} // namespace MinimalSocket::tcp
