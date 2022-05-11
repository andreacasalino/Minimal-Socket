/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "SocketError.h"
#include "SocketId.h"

namespace MinimalSocket {
namespace {
int getLastErrorCode() {
#ifdef _WIN32
  return WSAGetLastError();
#else
  return static_cast<int>(errno);
#endif
}
} // namespace

SocketError::SocketError(const std::string &what)
    : Error(what, " , error code: ", getLastErrorCode()) {}
} // namespace MinimalSocket
