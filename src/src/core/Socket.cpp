/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Socket.h>

#include "../SocketId.h"
#include "../Utils.h"

namespace MinimalSocket {
Socket::~Socket() = default;

Socket::Socket() { socket_id_wrapper = std::make_unique<SocketIdWrapper>(); }

bool Socket::empty() const {
  return socket_id_wrapper->accessId() == SCK_INVALID_SOCKET;
}

int Socket::accessSocketID() const {
  return static_cast<int>(getIDWrapper().accessId());
}

bool operator==(std::nullptr_t, const Socket &subject) {
  return subject.empty();
}
bool operator==(const Socket &subject, std::nullptr_t) {
  return subject.empty();
}

void Socket::transfer(Socket &receiver, Socket &giver) {
  receiver.socket_id_wrapper = std::move(giver.socket_id_wrapper);
}

const SocketIdWrapper &Socket::getIDWrapper() const {
  return *socket_id_wrapper;
}
SocketIdWrapper &Socket::getIDWrapper() { return *socket_id_wrapper; }

void Socket::destroyIDWrapper() { socket_id_wrapper.reset(); }

bool Openable::open() {
  if (opened) {
    throw Error{"Already opened"};
  }
  if (nullptr == static_cast<const Socket &>(*this)) {
    throw Error{"Can't open invalidated socket"};
  }
  std::scoped_lock lock(open_procedure_mtx);
  bool success = true;
  try {
    this->open_();
    opened = true;
  } catch (const Error &) {
    destroyIDWrapper();
    success = false;
  }
  return success;
}

void Openable::transfer(Openable &receiver, Openable &giver) {
  std::scoped_lock lock(receiver.open_procedure_mtx, giver.open_procedure_mtx);
  const bool o_value = giver.opened;
  receiver.opened = o_value;
  giver.opened = false;
  Socket::transfer(receiver, giver);
}

} // namespace MinimalSocket