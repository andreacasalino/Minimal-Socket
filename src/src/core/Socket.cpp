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

bool operator==(std::nullptr_t, const Socket &subject) {
  return subject.isNull();
}
bool operator==(const Socket &subject, std::nullptr_t) {
  return subject.isNull();
}

void Socket::transferIDWrapper(Socket &giver, Socket &recipient) {
  recipient.socket_id_wrapper = std::move(giver.socket_id_wrapper);
  giver.socket_id_wrapper = std::make_unique<SocketIdWrapper>();
}

const SocketIdWrapper &Socket::getIDWrapper() const {
  return *socket_id_wrapper;
}
SocketIdWrapper &Socket::getIDWrapper() { return *socket_id_wrapper; }

bool Openable::open() {
  if (opened) {
    throw Error{"Already opened"};
  }
  std::scoped_lock lock(open_procedure_mtx);
  bool success = true;
  try {
    this->open_();
    opened = true;
  } catch (const Error &) {
    getIDWrapper().close();
    success = false;
  }
  return success;
}
} // namespace MinimalSocket