/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "SocketError.h"
#include "SocketId.h"

namespace MinimalSocket {
int getLastErrorCode() {
#ifdef _WIN32
  return WSAGetLastError();
#else
  return static_cast<int>(errno);
#endif
}

void throwWithLastErrorCode(const std::string &what) {
  throw Error(what, " , error code: ", getLastErrorCode());
}
} // namespace MinimalSocket
