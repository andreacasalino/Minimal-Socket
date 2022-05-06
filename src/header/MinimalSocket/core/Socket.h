/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <memory>
#include <string>

namespace MinimalSocket {
using Buffer = std::string;

class SocketIdWrapper;
class Socket {
public:
  virtual ~Socket();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

protected:
  Socket();

  void stealIDWrapper(Socket &o);

  const SocketIdWrapper &getIDWrapper() const { return *socket_id_wrapper; };
  SocketIdWrapper &getIDWrapper() { return *socket_id_wrapper; };

private:
  std::unique_ptr<SocketIdWrapper> socket_id_wrapper;
};
} // namespace MinimalSocket
