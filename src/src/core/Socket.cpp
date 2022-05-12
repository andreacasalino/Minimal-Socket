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

#include <future>

namespace MinimalSocket {
void clear(Buffer &subject) {
  ::memset(subject.buffer, 0, subject.buffer_size);
}

Buffer makeStringBuffer(std::string &subject) {
  return Buffer{subject.data(), subject.size()};
}

ConstBuffer makeStringConstBuffer(const std::string &subject) {
  return ConstBuffer{subject.data(), subject.size()};
}

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
  try {
    if (NULL_TIMEOUT == timeout) {
      this->open_();
    } else {
      auto open_task = std::async([&]() { this->open_(); });
      auto open_task_status = open_task.wait_for(timeout);
      if (open_task_status == std::future_status::ready) {
        open_task.get(); // will throw if ready because an exception throwned
                         // immediately
      } else {
        resetIDWrapper();
        std::unique_ptr<TimeoutError> result;
        result.reset(new TimeoutError{});
        return result;
      }
    }
    opened = true;
  } catch (const SocketError &e) {
    resetIDWrapper();
    return std::make_unique<SocketError>(e);
  } catch (const Error &e) {
    resetIDWrapper();
    return std::make_unique<Error>(e);
  } catch (...) {
    resetIDWrapper();
    return std::make_unique<Error>("Not opened for an unkown reason");
  }
  return nullptr;
}

void Openable::transfer(Openable &receiver, Openable &giver) {
  std::scoped_lock lock(receiver.open_procedure_mtx, giver.open_procedure_mtx);
  const bool o_value = giver.opened;
  receiver.opened = o_value;
  giver.opened = false;
  Socket::transfer(receiver, giver);
}

} // namespace MinimalSocket