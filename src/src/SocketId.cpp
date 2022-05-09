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
WSALazyInitializer::WSALazyInitializer() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);
}

WSALazyInitializer::~WSALazyInitializer() {
    WSACleanup();
}

std::mutex WSALazyInitializer::lazy_proxy_mtx = std::mutex{};
std::unique_ptr<WSALazyInitializer> WSALazyInitializer::lazy_proxy = nullptr;

void WSALazyInitializer::lazyInit() {
    std::scoped_lock lock(WSALazyInitializer::lazy_proxy_mtx);
    if (nullptr == WSALazyInitializer::lazy_proxy) {
        WSALazyInitializer::lazy_proxy.reset(new WSALazyInitializer{});
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
  WSALazyInitializer::lazyInit();
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