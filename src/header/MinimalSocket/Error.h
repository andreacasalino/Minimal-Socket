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

  template <typename... Args>
  Error(const Args &...args) : Error(merge(args...)) {}

protected:
  template <typename... Args> static std::string merge(const Args &...args) {
    std::stringstream stream;
    (merge_(stream, args), ...);
    return stream.str();
  };

  template <typename T>
  static void merge_(std::stringstream &stream, const T &arg) {
    stream << arg;
  };
};

class ErrorCodeHolder {
public:
  ErrorCodeHolder();

  int getErrorCode() const { return errorCode; }

private:
  int errorCode;
};

class SocketError : public ErrorCodeHolder, public Error {
public:
  /**
   * @brief The last raised error code is automatically retrieved
   * and included in the error message
   */
  SocketError(const std::string &what);

  template <typename... Args>
  SocketError(const Args &...args) : SocketError{merge(args...)} {};
};

class TimeOutError : public Error {
public:
  TimeOutError() : Error("Timeout reached"){};
};
} // namespace MinimalSocket
