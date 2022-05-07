/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include "../Commons.h"

namespace MinimalSocket::tcp {
TcpServer::TcpServer(TcpServer &&o) : BindedPortAware(o) {
  Socket::transferIDWrapper(o, *this);
}
TcpServer &TcpServer::operator=(TcpServer &&o) {
  static_cast<BindedPortAware &>(*this) = o;
  Socket::transferIDWrapper(o, *this);
  return *this;
}

TcpServer::TcpServer(const Port &port, const AddressFamily &kind_of_client)
    : BindedPortAware(port), kind_of_client_to_accept(kind_of_client) {}

bool TcpServer::open_() {
  auto &socket = getIDWrapper();
  const auto port = getPortToBind();
  bool success = true;
  try {
    socket.reset(TCP, kind_of_client_to_accept);
    bind(socket.access(), kind_of_client_to_accept, port);
    listen(socket.access());
  } catch (const Error &) {
    socket.close();
    success = false;
  }
  return success;
}

TcpConnection TcpServer::acceptNewClient() {
  if (!this->wasOpened()) {
    throw Error("Tcp server was not opened before starting to accept clients");
  }
  SocketIp acceptedClientAddress;
#ifdef _WIN32
  int acceptedAddressLength
#else
  unsigned int acceptedAddressLength
#endif
      = sizeof(SocketIp);
  // accept: wait for a client to call connect and hit this server and get a
  // pointer to this client.
  SocketID accepted_client_socket_id = ::accept(
      getIDWrapper().access(), &acceptedClientAddress, &acceptedAddressLength);
  if (accepted_client_socket_id == SCK_INVALID_SOCKET) {
    throwWithLastErrorCode("Error: accepting new client");
  }

  auto accepted_client_parsed_address = make_address(acceptedClientAddress);
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
