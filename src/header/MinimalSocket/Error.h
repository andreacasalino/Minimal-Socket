/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace MinimalSocket {
class Error : public std::runtime_error {
public:
  Error(const std::string &what) : std::runtime_error(what){};

  template <typename... Args> Error(Args... args) : Error(merge(args...)) {}

protected:
  template <typename... Args> static std::string merge(Args... args) {
    std::stringstream stream;
    merge(stream, args...);
    return stream.str();
  };

  template <typename T, typename... Args>
  static void merge(std::stringstream &stream, const T &current,
                    Args... remaining) {
    stream << current;
    merge(stream, remaining...);
  };

  template <typename T, typename... Args>
  static void merge(std::stringstream &stream, const T &back) {
    stream << back;
  };
};

class ErrorCodeAware {
public:
  int getErrorCode() const { return error_code; }

protected:
  ErrorCodeAware();

private:
  int error_code;
};
class SocketError : public ErrorCodeAware, public Error {
public:
  /**
   * @brief last error code raised by the socket API is automatically retrieved
   */
  SocketError(const std::string &what);

  template <typename... Args>
  SocketError(const Args &...args) : SocketError{merge(args...)} {};
};
} // namespace MinimalSocket
