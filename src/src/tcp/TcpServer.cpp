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

namespace MinimalSocket::tcp {
TcpServer::TcpServer(TcpServer &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  Socket::transferIDWrapper(o, *this);
}
TcpServer &TcpServer::operator=(TcpServer &&o) {
  static_cast<PortToBindAware &>(*this) = o;
  static_cast<RemoteAddressFamilyAware &>(*this) = o;
  Socket::transferIDWrapper(o, *this);
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
  MinimalSocket::bind(socket.accessId(), family, port);
  MinimalSocket::listen(socket.accessId());
}

TcpConnection TcpServer::acceptNewClient() {
  if (!this->wasOpened()) {
    throw Error("Tcp server was not opened before starting to accept clients");
  }
  SocketAddress acceptedClientAddress;
#ifdef _WIN32
  int acceptedAddressLength
#else
  unsigned int acceptedAddressLength
#endif
      = sizeof(SocketAddress);
  // accept: wait for a client to call connect and hit this server and get a
  // pointer to this client.
  SocketID accepted_client_socket_id =
      ::accept(getIDWrapper().accessId(), &acceptedClientAddress,
               &acceptedAddressLength);
  if (accepted_client_socket_id == SCK_INVALID_SOCKET) {
    throwWithLastErrorCode("Error: accepting new client");
  }

  auto accepted_client_parsed_address = toAddress(acceptedClientAddress);
  TcpConnection result(accepted_client_parsed_address);
  result.getIDWrapper().reset(accepted_client_socket_id);
  return std::move(result);
}

TcpConnection::TcpConnection(const Address &remote_address)
    : RemoteAddressAware(remote_address) {}

TcpConnection::TcpConnection(TcpConnection &&o) : RemoteAddressAware(o) {
  Socket::transferIDWrapper(o, *this);
}
TcpConnection &TcpConnection::operator=(TcpConnection &&o) {
  static_cast<RemoteAddressAware &>(*this) = o;
  Socket::transferIDWrapper(o, *this);
  return *this;
}
} // namespace MinimalSocket::tcp
