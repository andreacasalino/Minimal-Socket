/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Socket.h>

#include "../SocketHandler.h"
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

Socket::Socket() { resetHandler(); }

int Socket::getSocketDescriptor() const {
  return static_cast<int>(getHandler().accessId());
}

void Socket::steal(Socket &giver) {
  this->socket_id_wrapper = std::move(giver.socket_id_wrapper);
  giver.resetHandler();
}

const SocketHandler &Socket::getHandler() const { return *socket_id_wrapper; }
SocketHandler &Socket::getHandler() { return *socket_id_wrapper; }

void Socket::resetHandler() {
  socket_id_wrapper = std::make_unique<SocketHandler>();
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
                         [this]() { this->resetHandler(); }, timeout);
    }
    opened = true;
  } catch (const SocketError &e) {
    exception = std::make_unique<SocketError>(e);
  } catch (const TimeOutError &) {
    // assert and do nothing
  } catch (const Error &e) {
    exception = std::make_unique<Error>(e);
  } catch (...) {
    exception = std::make_unique<Error>("Not opened for an unknown reason");
  }
  if (nullptr != exception) {
    this->resetHandler();
    throw *exception;
  }
  return opened;
}

void Openable::steal(Openable &giver) {
  std::scoped_lock lock(this->open_procedure_mtx, giver.open_procedure_mtx);
  const bool o_value = giver.opened;
  this->opened = o_value;
  giver.opened = false;
  this->Socket::steal(giver);
}

} // namespace MinimalSocket