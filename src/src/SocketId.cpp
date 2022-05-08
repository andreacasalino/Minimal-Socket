/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "SocketError.h"
#include "Utils.h"

namespace MinimalSocket {
#ifdef _WIN32
std::size_t SocketIdWrapper::SocketIDFactory::handlerCounter = 0;
std::mutex SocketIdWrapper::SocketIDFactory::handlerCounterMtx;

void SocketIdWrapper::SocketIDFactory::beforeOpen() {
  std::lock_guard<std::mutex> hndLck(handlerCounterMtx);
  ++handlerCounter;
  if (1 == handlerCounter) {
    // first socket opened
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);
  }
}

void SocketIdWrapper::SocketIDFactory::afterClose() {
  std::lock_guard<std::mutex> hndLck(handlerCounterMtx);
  --handlerCounter;
  if (0 == handlerCounter) {
    // last socket closed
    WSACleanup();
  }
}
#endif

namespace {
void close(SocketID &socket_id) {
  if (socket_id == SCK_INVALID_SOCKET) {
    return;
  }
#ifdef _WIN32
  shutdown(socket_id, 2);
  closesocket(socket_id);
#else
  ::shutdown(socket_id, SHUT_RDWR);
  ::close(socket_id);
#endif
  socket_id = SCK_INVALID_SOCKET;
#ifdef _WIN32
  SocketIDFactory::afterClose();
#endif
}
} // namespace

SocketIdWrapper::~SocketIdWrapper() { MinimalSocket::close(socket_id); }

void SocketIdWrapper::reset(const SocketID &hndl) {
  if (socket_id != SCK_INVALID_SOCKET) {
    MinimalSocket::close(socket_id);
  }
  this->socket_id = hndl;
}

namespace {
int domain_number(const AddressFamily &family) {
  int result;
  visitAddress(
      family, [&]() { result = static_cast<int>(AF_INET); },
      [&]() { result = static_cast<int>(AF_INET6); });
  return result;
}
} // namespace

void SocketIdWrapper::reset(const SocketType &type,
                            const AddressFamily &family) {
  if (socket_id != SCK_INVALID_SOCKET) {
    MinimalSocket::close(socket_id);
  }

#ifdef _WIN32
  SocketIDFactory::beforeOpen();
#endif

  switch (type) {
  case SocketType::TCP:
    this->socket_id = ::socket(domain_number(family), SOCK_STREAM, 0);
    if (this->socket_id == SCK_INVALID_SOCKET) {
      MinimalSocket::close(socket_id);
      throwWithLastErrorCode("Stream socket could not be created");
    }
    break;
  case SocketType::UDP:
    this->socket_id = ::socket(domain_number(family), SOCK_DGRAM, 0);
    if (this->socket_id == SCK_INVALID_SOCKET) {
      MinimalSocket::close(socket_id);
      throwWithLastErrorCode("DataGram socket could not be created");
    }
    break;
  default:
    throw Error("unknown protocol type");
  }
}
} // namespace MinimalSocket