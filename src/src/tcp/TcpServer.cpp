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
  socket.reset(SocketType::TCP, family);
  auto binded_port =
      MinimalSocket::bind(socket.accessId(), family, port, shallBeFreePort());
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
  auto temp = acceptNewClient(NULL_TIMEOUT);
  return std::move(temp.value());
}

std::optional<TcpConnection>
TcpServer::acceptNewClient(const Timeout &timeout) {
  std::scoped_lock lock(accept_mtx);
  if (!this->wasOpened()) {
    throw Error("Tcp server was not opened before starting to accept clients");
  }

  char acceptedClientAddress[MAX_POSSIBLE_ADDRESS_SIZE];
  SocketAddressLength acceptedClientAddress_length = MAX_POSSIBLE_ADDRESS_SIZE;
  SocketID accepted_client_socket_id = SCK_INVALID_SOCKET;

  auto accept_client = [&]() {
    // accept: wait for a client to call connect and hit this server and get a
    // pointer to this client.
    accepted_client_socket_id =
        ::accept(getIDWrapper().accessId(),
                 reinterpret_cast<SocketAddress *>(&acceptedClientAddress[0]),
                 &acceptedClientAddress_length);
    if (accepted_client_socket_id == SCK_INVALID_SOCKET) {
      auto err = SocketError{"accepting a new client"};
      throw err;
    }
  };

  try {
    if (NULL_TIMEOUT == timeout) {
      accept_client();
    } else {
      try_within_timeout([&]() { accept_client(); },
                         [this]() { this->resetIDWrapper(); }, timeout);
    }
  } catch (const TimeOutError &) {
    TcpServer reopened = TcpServer{getPortToBind(), getRemoteAddressFamily()};
    reopened.open();
    *this = std::move(reopened);
    return std::nullopt;
  } catch (...) {
    std::rethrow_exception(std::current_exception());
  }

  auto accepted_client_parsed_address =
      toAddress(reinterpret_cast<const SocketAddress &>(acceptedClientAddress));
  std::optional<TcpConnection> result;
  auto &accepted =
      result.emplace(TcpConnection{accepted_client_parsed_address});
  accepted.getIDWrapper().reset(accepted_client_socket_id);
  return result;
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
