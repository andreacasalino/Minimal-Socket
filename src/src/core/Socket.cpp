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

int Socket::accessSocketID() const {
  return static_cast<int>(getIDWrapper().accessId());
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

bool Openable::open(const Timeout &timeout) {
  if (opened) {
    throw Error{"Already opened"};
  }
  std::scoped_lock lock(open_procedure_mtx);
  std::unique_ptr<Error> exception;
  try {
    if (NULL_TIMEOUT == timeout) {
      this->open_();
    } else {
      try_within_timeout([this]() { this->open_(); },
                         [this]() { this->resetIDWrapper(); }, timeout);
    }
    opened = true;
  } catch (const SocketError &e) {
    exception = std::make_unique<SocketError>(e);
  } catch (const TimeOutError &) {
    // assert and do nothing
  } catch (const Error &e) {
    exception = std::make_unique<Error>(e);
  } catch (...) {
    exception = std::make_unique<Error>("Not opened for an unkown reason");
  }
  if (nullptr != exception) {
    this->resetIDWrapper();
    throw *exception;
  }
  return opened;
}

void Openable::transfer(Openable &receiver, Openable &giver) {
  std::scoped_lock lock(receiver.open_procedure_mtx, giver.open_procedure_mtx);
  const bool o_value = giver.opened;
  receiver.opened = o_value;
  giver.opened = false;
  Socket::transfer(receiver, giver);
}

} // namespace MinimalSocket