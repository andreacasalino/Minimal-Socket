/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "../src/SocketHandler.h"

namespace MinimalSocket {
namespace {
int getLastErrorCode() {
  int res =
#ifdef _WIN32
      WSAGetLastError();
#else
      static_cast<int>(errno);
#endif
  return res;
}
} // namespace

ErrorCodeHolder::ErrorCodeHolder() : errorCode{getLastErrorCode()} {}

SocketError::SocketError(const std::string &what)
    : ErrorCodeHolder{}, Error(what, " , error code: ", getErrorCode()) {}
} // namespace MinimalSocket
