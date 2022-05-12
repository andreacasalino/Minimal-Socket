/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "SocketId.h"

namespace MinimalSocket {
ErrorCodeAware::ErrorCodeAware() {
  error_code =
#ifdef _WIN32
      WSAGetLastError();
#else
      static_cast<int>(errno);
#endif
}

SocketError::SocketError(const std::string &what)
    : ErrorCodeAware(), Error(what, " , error code: ", getErrorCode()) {}
} // namespace MinimalSocket
