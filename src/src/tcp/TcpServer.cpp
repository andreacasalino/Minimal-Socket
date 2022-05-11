/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include "../SocketAddress.h"
#include "../SocketError.h"
#include "../SocketFunctions.h"
#include "../Utils.h"

namespace MinimalSocket::tcp {
TcpServer::TcpServer(TcpServer &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  Openable::transfer(*this, o);
}
TcpServer &TcpServer::operator=(TcpServer &&o) {
  Openable::transfer(*this, o);
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
  return *this;
}

TcpServer::TcpServer(const Port port_to_bind,
                     const AddressFamily &accepted_client_family)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_client_family) {}

void TcpServer::open_() {
  auto &socket = getIDWrapper();
  const auto port = getPortToBind();
  const auto family = getRemoteAddressFamily();
  socket.reset(TCP, family);
  auto binded_port = MinimalSocket::bind(socket.accessId(), family, port);
  setPort(binded_port);
  MinimalSocket::listen(socket.accessId(), client_queue_size);
}

void TcpServer::setClientQueueSize(const std::size_t queue_size) {
  if (wasOpened()) {
    throw Error{"Can't set client queue size of an alrady opened tcp server"};
  }
  client_queue_size = queue_size;
}

TcpConnection TcpServer::acceptNewClient() {
  if (!this->wasOpened()) {
    throw Error("Tcp server was not opened before starting to accept clients");
  }

  char acceptedClientAddress[MAX_POSSIBLE_ADDRESS_SIZE];
  SocketAddressLength acceptedClientAddress_length = MAX_POSSIBLE_ADDRESS_SIZE;

  // accept: wait for a client to call connect and hit this server and get a
  // pointer to this client.
  SocketID accepted_client_socket_id =
      ::accept(getIDWrapper().accessId(),
               reinterpret_cast<SocketAddress *>(&acceptedClientAddress[0]),
               &acceptedClientAddress_length);
  if (accepted_client_socket_id == SCK_INVALID_SOCKET) {
    throwWithLastErrorCode("Error: accepting new client");
  }

  auto accepted_client_parsed_address =
      toAddress(reinterpret_cast<const SocketAddress &>(acceptedClientAddress));
  TcpConnection result(accepted_client_parsed_address);
  result.getIDWrapper().reset(accepted_client_socket_id);
  return std::move(result);
}

TcpConnection::TcpConnection(const Address &remote_address)
    : RemoteAddressAware(remote_address) {}

TcpConnection::TcpConnection(TcpConnection &&o) : RemoteAddressAware(o) {
  Socket::transfer(*this, o);
}
TcpConnection &TcpConnection::operator=(TcpConnection &&o) {
  copy_as<RemoteAddressAware>(*this, o);
  Socket::transfer(*this, o);
  return *this;
}
} // namespace MinimalSocket::tcp
