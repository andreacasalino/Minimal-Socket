/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "SocketHandler.h"
#include "Utils.h"

#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#endif

namespace MinimalSocket {
#ifdef _WIN32
WSALazyInitializer::WSALazyInitializer(const WSAVersion &version)
    : configured_version(version) {
  WSADATA wsa;
  const BYTE version_major = static_cast<BYTE>(version[0]);
  const BYTE version_minor = static_cast<BYTE>(version[1]);
  auto result = WSAStartup(MAKEWORD(version_major, version_minor), &wsa);
  if (0 == result) {
    return;
  }
  std::string message;
  switch (result) {
  case WSASYSNOTREADY:
    message = " , system not ready";
    break;
  case WSAVERNOTSUPPORTED:
    message = " , version not supported";
    break;
  case WSAEINPROGRESS:
    message = " , blocking operation in progress";
    break;
  case WSAEPROCLIM:
    message = " , maximum supported tasks reached";
    break;
  case WSAEFAULT:
    message = " , invalid WSADATA";
    break;
  default:
    throw Error{"Not able to initialize WSA, reason unknown"};
    break;
  }
  auto err =
      Error{"Not able to initialize WSA, error code: ", std::to_string(result),
            message};
  throw err;
}

WSALazyInitializer::~WSALazyInitializer() { WSACleanup(); }

std::mutex WSALazyInitializer::lazy_proxy_mtx = std::mutex{};
std::unique_ptr<WSALazyInitializer> WSALazyInitializer::lazy_proxy = nullptr;

void WSALazyInitializer::lazyInit() {
  auto version = WSAManager::getWsaVersion();
  std::scoped_lock lock(WSALazyInitializer::lazy_proxy_mtx);
  if ((nullptr != WSALazyInitializer::lazy_proxy) &&
      (WSALazyInitializer::lazy_proxy->configured_version == version)) {
    return;
  }
  try {
    WSALazyInitializer::lazy_proxy.reset(new WSALazyInitializer{version});
  } catch (const Error &e) {
    WSALazyInitializer::lazy_proxy = nullptr;
    throw e;
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

SocketHandler::~SocketHandler() { 
  MinimalSocket::close(socket_id); 
}

void SocketHandler::shutDown() {
  MinimalSocket::close(socket_id); 
}

void SocketHandler::reset(SocketID hndl) {
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

void SocketHandler::reset(SocketType type, AddressFamily family) {
  if (socket_id != SCK_INVALID_SOCKET) {
    MinimalSocket::close(socket_id);
  }

#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  switch (type) {
  case SocketType::TCP:
    this->socket_id = ::socket(domain_number(family), SOCK_STREAM, 0);
    break;
  case SocketType::UDP:
    this->socket_id = ::socket(domain_number(family), SOCK_DGRAM, 0);
    break;
  default:
    throw Error("Unknown socket type");
  }
  if (this->socket_id == SCK_INVALID_SOCKET) {
    MinimalSocket::close(socket_id);
    auto err = SocketError{"Stream socket could not be created"};
    throw err;
  }
}

} // namespace MinimalSocket