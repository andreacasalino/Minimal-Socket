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
#ifdef _WIN32
std::mutex WSAManager::wsa_version_mtx = std::mutex{};
WSAVersion WSAManager::wsa_version = WSAVersion{2, 2};

void WSAManager::setWsaVersion(const WSAVersion &version) {
  std::scoped_lock lock(wsa_version_mtx);
  wsa_version = version;
}

WSAVersion WSAManager::getWsaVersion() {
  std::scoped_lock lock(wsa_version_mtx);
  return wsa_version;
}
#endif

Socket::~Socket() = default;

Socket::Socket() { resetIDWrapper(); }

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
  giver.resetIDWrapper();
}

const SocketIdWrapper &Socket::getIDWrapper() const {
  return *socket_id_wrapper;
}
SocketIdWrapper &Socket::getIDWrapper() { return *socket_id_wrapper; }

void Socket::resetIDWrapper() {
  socket_id_wrapper = std::make_unique<SocketIdWrapper>();
}

std::unique_ptr<Error> Openable::open(const Timeout &timeout) {
  if (opened) {
    throw Error{"Already opened"};
  }
  std::scoped_lock lock(open_procedure_mtx);
  std::unique_ptr<Error> result;
  try {
    if (NULL_TIMEOUT == timeout) {
      this->open_();
    } else {
      try_within_timeout([this]() { this->open_(); },
                         [this]() { this->resetIDWrapper(); }, timeout);
    }
    opened = true;
  } catch (const SocketError &e) {
    result = std::make_unique<SocketError>(e);
  } catch (const Error &e) {
    result = std::make_unique<Error>(e);
  } catch (...) {
    result = std::make_unique<Error>("Not opened for an unkown reason");
  }
  if (nullptr != result) {
    this->resetIDWrapper();
  }
  return result;
}

void Openable::transfer(Openable &receiver, Openable &giver) {
  std::scoped_lock lock(receiver.open_procedure_mtx, giver.open_procedure_mtx);
  const bool o_value = giver.opened;
  receiver.opened = o_value;
  giver.opened = false;
  Socket::transfer(receiver, giver);
}

} // namespace MinimalSocket