/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

namespace MinimalSocket {
using Buffer = std::string;

void setZeros(Buffer &subject);

class SocketIdWrapper;
class Socket {
public:
  virtual ~Socket();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  bool isNull() const;

protected:
  Socket();

  static void transferIDWrapper(Socket &giver, Socket &recipient);

  const SocketIdWrapper &getIDWrapper() const;
  SocketIdWrapper &getIDWrapper();

private:
  std::unique_ptr<SocketIdWrapper> socket_id_wrapper;
};

bool operator==(std::nullptr_t, const Socket &subject) {
  return subject.isNull();
}
bool operator==(const Socket &subject, std::nullptr_t) {
  return subject.isNull();
}

class Openable : public virtual Socket {
public:
  bool wasOpened() const { return opened; }
  bool open();

protected:
  Openable() = default;

  virtual void open_() = 0;

private:
  std::mutex open_procedure_mtx;
  std::atomic_bool opened = false;
};
} // namespace MinimalSocket
