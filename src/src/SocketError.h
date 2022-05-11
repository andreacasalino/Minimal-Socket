/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/Error.h>

#include <string>

namespace MinimalSocket {
class SocketError : public Error {
public:
  /**
   * @brief last error code raised by the socket API is automatically retrieved
   */
  SocketError(const std::string &what);

  template <typename... Args>
  SocketError(const Args &...args) : SocketError{merge(args...)} {};
};
} // namespace MinimalSocket
