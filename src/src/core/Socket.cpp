/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Socket.h>

#include "../Commons.h"

namespace MinimalSocket {
Socket::~Socket() = default;

Socket::Socket() { socket_id_wrapper = std::make_unique<SocketIdWrapper>(); }

void Socket::stealIDWrapper(Socket &o) {
  this->socket_id_wrapper = std::move(o.socket_id_wrapper);
  o.socket_id_wrapper = std::make_unique<SocketIdWrapper>();
}

const SocketIdWrapper &Socket::getIDWrapper() const {
  return *socket_id_wrapper;
}
SocketIdWrapper &Socket::getIDWrapper() { return *socket_id_wrapper; }
} // namespace MinimalSocket