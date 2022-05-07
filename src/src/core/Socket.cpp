/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Socket.h>

#include "../Commons.h"

namespace MinimalSocket {
Socket::~Socket() = default;

Socket::Socket() { socket_id_wrapper = std::make_unique<SocketIdWrapper>(); }

bool Socket::isNull() const {
  return socket_id_wrapper->access() == SCK_INVALID_SOCKET;
}

void Socket::stealIDWrapper(Socket &o) {
  this->socket_id_wrapper = std::move(o.socket_id_wrapper);
  o.socket_id_wrapper = std::make_unique<SocketIdWrapper>();
}

const SocketIdWrapper &Socket::getIDWrapper() const {
  return *socket_id_wrapper;
}
SocketIdWrapper &Socket::getIDWrapper() { return *socket_id_wrapper; }

bool Openable::open() {
  if (opened) {
    throw Error{"Already opened"};
  }
  const bool success = open_();
  opened = success;
  return success;
}
} // namespace MinimalSocket