/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <atomic>
#include <memory>
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

  void stealIDWrapper(Socket &o);

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

class Openable {
public:
  virtual ~Openable() = default;

  bool wasOpened() const { return opened; }
  bool open();

protected:
  Openable() = default;

  virtual bool open_() = 0;

private:
  std::atomic_bool opened = false;
};
} // namespace MinimalSocket
