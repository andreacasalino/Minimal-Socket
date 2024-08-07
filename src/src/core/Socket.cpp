/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Socket.h>

#include "../SocketFunctions.h"
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

void Socket::shutDown() { socket_id_wrapper->shutDown(); }

int Socket::getSocketDescriptor() const {
  return static_cast<int>(getHandler().accessId());
}

void Socket::steal(Socket &giver) {
  this->socket_id_wrapper = std::move(giver.socket_id_wrapper);
  giver.resetHandler();
  isBlocking_ = giver.isBlocking_;
}

const SocketHandler &Socket::getHandler() const { return *socket_id_wrapper; }
SocketHandler &Socket::getHandler() { return *socket_id_wrapper; }

void Socket::resetHandler() {
  socket_id_wrapper = std::make_unique<SocketHandler>();
}

void Socket::setUp() {
  if (!isBlocking_) {
    turnToNonBlocking(getHandler().accessId());
  }
}

void OpenableBase::steal(OpenableBase &giver) {
  std::scoped_lock lock(this->open_procedure_mtx, giver.open_procedure_mtx);
  const bool o_value = giver.opened;
  this->opened = o_value;
  giver.opened = false;
  this->Socket::steal(giver);
}

namespace {
template <typename Pred>
std::unique_ptr<Error> doOpen(std::mutex &mtx, std::atomic_bool &opened,
                              Pred pred) {
  if (opened) {
    throw Error{"Already opened"};
  }
  std::scoped_lock lock(mtx);
  std::unique_ptr<Error> exception;
  try {
    pred();
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
  return exception;
}
} // namespace

bool Openable::open() {
  auto excpt = doOpen(open_procedure_mtx, opened, [this]() { this->open_(); });
  if (excpt != nullptr) {
    resetHandler();
    throw *excpt;
  };
  return opened;
}

bool OpenableWithTimeout::open(const Timeout &timeout) {
  auto excpt = doOpen(open_procedure_mtx, opened, [&]() {
    if (NULL_TIMEOUT == timeout) {
      this->open_();
    } else {
      try_within_timeout([this]() { this->open_(); },
                         [this]() { this->resetHandler(); }, timeout);
    }
  });
  if (excpt != nullptr) {
    resetHandler();
    throw *excpt;
  };
  return opened;
}

} // namespace MinimalSocket