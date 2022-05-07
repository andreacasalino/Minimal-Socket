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
  auto result = Address::makeAddress(ip, port);
  if (nullptr == result) {
    throw Error{"Invalid address"};
  }
  return *result;
}

void address_case(const AddressFamily &family,
                  const std::function<void()> &ipv4_case,
                  const std::function<void()> &ipv6_case) {
  switch (family) {
  case AddressFamily::IP_V4:
    ipv4_case();
    break;
  case AddressFamily::IP_V6:
    ipv6_case();
    break;
  default:
    throw Error{"Unrecognized AddressFamily"};
    break;
  }
}

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

SocketIdWrapper::~SocketIdWrapper() { close(); }

void SocketIdWrapper::reset(const SocketID &hndl) {
  if (socket_id != SCK_INVALID_SOCKET) {
    close();
  }

  this->socket_id = hndl;
}

namespace {
int domain_number(const AddressFamily &family) {
  int result;
  address_case(
      family, [&]() { result = static_cast<int>(AF_INET); },
      [&]() { result = static_cast<int>(AF_INET6); });
  return result;
}
} // namespace

void SocketIdWrapper::reset(const SocketType &type,
                            const AddressFamily &family) {
  if (socket_id != SCK_INVALID_SOCKET) {
    close();
  }

#ifdef _WIN32
  SocketIDFactory::beforeOpen();
#endif

  switch (type) {
  case SocketType::TCP:
    this->socket_id = ::socket(domain_number(family), SOCK_STREAM, 0);
    if (this->socket_id == SCK_INVALID_SOCKET) {
      this->close();
      throwWithLastErrorCode("Stream socket could not be created");
    }
    break;
  case SocketType::UDP:
    this->socket_id = ::socket(domain_number(family), SOCK_DGRAM, 0);
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
  SocketIDFactory::afterClose();
#endif
}

namespace {
#ifdef _WIN32
#define REBIND_OPTION SO_REUSEADDR
#else
#define REBIND_OPTION SO_REUSEPORT
#endif
} // namespace

void bind(const SocketID &socket_id, const AddressFamily &family,
          const Port &port) {
  int reusePortOptVal = 1;
  ::setsockopt(socket_id, SOL_SOCKET, REBIND_OPTION,
               reinterpret_cast<const
#ifdef _WIN32
                                char * // not sure it would work with void* also
                                       // in Windows
#else
                                void *
#endif
                                >(&reusePortOptVal),
               sizeof(int));

  // bind the socket to the port
  address_case(
      family,
      [&]() {
        SocketIp4 addr;
        ::memset(&addr, 0, sizeof(SocketIp4));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
#ifdef _WIN32
        addr.sin_addr.s_addr = ADDR_ANY;
#else
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
        if (::bind(socket_id, reinterpret_cast<SocketIp *>(&addr),
                   sizeof(SocketIp4)) == SCK_SOCKET_ERROR) {
          throwWithLastErrorCode("can't bind localhost on port: " +
                                 std::to_string(port));
        }
      },
      [&]() {
        SocketIp6 addr;
        ::memset(&addr, 0, sizeof(SocketIp6));
        addr.sin6_family = AF_INET6;
        addr.sin6_flowinfo = 0;
        addr.sin6_addr =
            IN6ADDR_ANY_INIT; // apparently, there is no such a
                              // cross-system define for ipv6 addresses
        addr.sin6_port = htons(port);
        if (::bind(socket_id, reinterpret_cast<SocketIp *>(&addr),
                   sizeof(SocketIp6)) == SCK_SOCKET_ERROR) {
          throwWithLastErrorCode("can't bind localhost on port: " +
                                 std::to_string(port));
        }
      });
}

namespace {
static constexpr std::size_t LISTEN_BACKLOG = 50;
}

void listen(const SocketID &socket_id) {
  if (::listen(socket_id, LISTEN_BACKLOG) == SCK_SOCKET_ERROR) {
    throwWithLastErrorCode("Error: listening on reserved port");
  }
}

void connect(const SocketID &socket_id, const Address &remote_address) {
  address_case(
      remote_address.getFamily(),
      [&]() {
        // v4 family
        auto addr =
            makeSocketIp4(remote_address.getHost(), remote_address.getPort());
        if (!addr) {
          throw Error(to_string(remote_address),
                      " is an invalid server address");
        }
        if (::connect(socket_id, reinterpret_cast<SocketIp *>(&(*addr)),
                      sizeof(SocketIp4)) == SCK_SOCKET_ERROR) {
          throwWithLastErrorCode("Connection can't be established");
        }
      },
      [&]() {
        // v6 family
        auto addr =
            makeSocketIp6(remote_address.getHost(), remote_address.getPort());
        if (!addr) {
          throw Error(to_string(remote_address),
                      " is an invalid server address");
        }
        if (::connect(socket_id, reinterpret_cast<SocketIp *>(&(*addr)),
                      sizeof(SocketIp6)) == SCK_SOCKET_ERROR) {
          throwWithLastErrorCode("Connection can't be established");
        }
      });
}
} // namespace MinimalSocket