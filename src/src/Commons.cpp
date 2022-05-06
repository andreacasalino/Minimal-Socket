/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif

#include "Commons.h"
#include <MinimalSocket/Error.h>

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

std::optional<SocketIp4> makeSocketIp4(const std::string &raw_address,
                                       const Port &port) {
  std::optional<SocketIp4> result;
  auto &result_ref = result.emplace();
  // set everything to 0 first
  ::memset(&result_ref, 0, sizeof(SocketIp4));
  result_ref.sin_family = AF_INET;
  result_ref.sin_port = htons(port);

  // try address conversion
#if !defined(_WIN32)
  in_addr ia;
  if (1 == ::inet_pton(AF_INET, raw_address.c_str(), &ia)) {
    result_ref.sin_addr.s_addr = ia.s_addr;
    return result;
  }
#endif

  addrinfo *res, hints = addrinfo{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = 0;
  hints.ai_protocol = 0;

  int gai_err = ::getaddrinfo(raw_address.c_str(), NULL, &hints, &res);

#if !defined(_WIN32)
  if (gai_err == EAI_SYSTEM) {
    return std::nullopt;
  }
#endif
  if (gai_err != 0) {
    return std::nullopt;
  }

  auto ipv4 = reinterpret_cast<sockaddr_in *>(res->ai_addr);
  result_ref.sin_addr.s_addr = ipv4->sin_addr.s_addr;
  ::freeaddrinfo(res);
  return result;
}

std::optional<SocketIp6> makeSocketIp6(const std::string &raw_address,
                                       const Port &port) {
  std::optional<SocketIp6> result;
  auto &result_ref = result.emplace();
  // set everything to 0 first
  ::memset(&result_ref, 0, sizeof(SocketIp6));
  result_ref.sin6_family = AF_INET6;
  result_ref.sin6_flowinfo = 0;
  result_ref.sin6_port = htons(port);

  // try address conversion
#if !defined(_WIN32)
  in6_addr ia;
  if (1 == ::inet_pton(AF_INET6, raw_address.c_str(), &ia)) {
    result_ref.sin6_addr = ia;
    return result;
  }
#endif

  addrinfo *res, hints = addrinfo{};
  hints.ai_family = AF_INET6;
  hints.ai_socktype = 0;
  hints.ai_protocol = 0;

  int gai_err = ::getaddrinfo(raw_address.c_str(), NULL, &hints, &res);

#if !defined(_WIN32)
  if (gai_err == EAI_SYSTEM) {
    return std::nullopt;
  }
#endif
  if (gai_err != 0) {
    return std::nullopt;
  }

  auto ipv6 = reinterpret_cast<sockaddr_in6 *>(res->ai_addr);
  result_ref.sin6_addr = ipv6->sin6_addr;
  ::freeaddrinfo(res);
  return result;
}

Address make_address(const SocketIp &address) {
  // refer to
  // https://stackoverflow.com/questions/11684008/how-do-you-cast-sockaddr-structure-to-a-sockaddr-in-c-networking-sockets-ubu
  std::string ip;
  Port port;
  if (AF_INET == address.sa_family) {
    // ipv4 address
    // inet_ntoa is deprecated, but using inet_ntop for ipv4 address, leads to
    // an ip that has no sense
    ip = std::string(
        ::inet_ntoa(reinterpret_cast<const SocketIp4 *>(&address)->sin_addr));
    port = ntohs(reinterpret_cast<const SocketIp4 *>(&address)->sin_port);
  } else {
    // ipv6 address
    char temp[INET6_ADDRSTRLEN]; // this is the longest one
    // refer to
    // https://www.gnu.org/software/libc/manual/html_node/Host-Address-Functions.html
    ::memset(temp, 0, INET6_ADDRSTRLEN);
    ::inet_ntop(address.sa_family, &address, temp, INET6_ADDRSTRLEN);
    ip = std::string(temp, INET6_ADDRSTRLEN);
    port = ntohs(reinterpret_cast<const SocketIp6 *>(&address)->sin6_port);
  }
  return Address{ip, port};
}

#ifdef _WIN32
std::size_t Channel::SocketHandlerFactory::handlerCounter = 0;
std::mutex Channel::SocketHandlerFactory::handlerCounterMtx;

void Channel::SocketHandlerFactory::beforeOpen() {
  std::lock_guard<std::mutex> hndLck(handlerCounterMtx);
  ++handlerCounter;
  if (1 == handlerCounter) {
    // first socket opened
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);
  }
}

void Channel::SocketHandlerFactory::afterClose() {
  std::lock_guard<std::mutex> hndLck(handlerCounterMtx);
  --handlerCounter;
  if (0 == handlerCounter) {
    // last socket closed
    WSACleanup();
  }
}
#endif

SocketIdWrapper::~SocketIdWrapper() { close(); }

void SocketIdWrapper::reset(const SocketID &hndl) {
  if (socket_id != SCK_INVALID_SOCKET) {
    close();
  }

  this->socket_id = hndl;
}

namespace {
int to_int(const AddressFamily &family) {
  switch (family) {
  case AddressFamily::IP_V4:
    return static_cast<int>(AF_INET);
  case AddressFamily::IP_V6:
    return static_cast<int>(AF_INET6);
  }
  throw Error("unknown address family type");
}
} // namespace

void SocketIdWrapper::reset(const SocketType &type,
                            const AddressFamily &family) {
  if (socket_id != SCK_INVALID_SOCKET) {
    close();
  }

#ifdef _WIN32
  SocketHandlerFactory::beforeOpen();
#endif

  switch (type) {
  case SocketType::TCP:
    this->socket_id = ::socket(to_int(family), SOCK_STREAM, 0);
    if (this->socket_id == SCK_INVALID_SOCKET) {
      this->close();
      throwWithLastErrorCode("Stream socket could not be created");
    }
    break;
  case SocketType::UDP:
    this->socket_id = ::socket(to_int(family), SOCK_DGRAM, 0);
    if (this->socket_id == SCK_INVALID_SOCKET) {
      this->close();
      throwWithLastErrorCode("DataGram socket could not be created");
    }
    break;
  default:
    throw Error("unknown protocol type");
  }
}

void SocketIdWrapper::close() {
  if (socket_id == SCK_INVALID_SOCKET) {
    return;
  }
#ifdef _WIN32
  shutdown(this->socket_id, 2);
  closesocket(this->socket_id);
#else
  ::shutdown(this->socket_id, SHUT_RDWR);
  ::close(this->socket_id);
#endif
  this->socket_id = SCK_INVALID_SOCKET;
#ifdef _WIN32
  SocketHandlerFactory::afterClose();
#endif
}
} // namespace MinimalSocket