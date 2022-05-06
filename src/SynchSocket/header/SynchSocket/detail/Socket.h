/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <memory>
#include <string>

namespace MinCppSock {
enum Protocol { UDP, TCP };

// class ProtocolAware {
// public:
//   const Protocol &getProtocol() const { return protocol; };

// protected:
//   ProtocolAware(const Protocol &protocol);

// private:
//   const Protocol protocol;
// };

using Buffer = std::string;

class SocketHandler;
class SocketHandlerAware {
public:
  virtual ~SocketHandlerAware();

protected:
  SocketHandlerAware();

  const SocketHandler &socketHandler() const;
  SocketHandler &socketHandler();

private:
  std::unique_ptr<SocketHandler> channel;
};

class Closable {
public:
  virtual ~Closable() = default;

  /**
   * @brief close the object
   */
  virtual void close() = 0;
};

} // namespace MinCppSock
