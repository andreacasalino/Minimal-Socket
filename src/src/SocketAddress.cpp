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

#include <MinimalSocket/Error.h>

#include "SocketAddress.h"
#include "SocketError.h"

namespace MinimalSocket {
std::optional<SocketAddressIpv4> toSocketAddressIpv4(const std::string &host,
                                                     const Port &port) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  std::optional<SocketAddressIpv4> result;
  auto &result_ref = result.emplace();
  // set everything to 0 first
  ::memset(&result_ref, 0, sizeof(SocketAddressIpv4));
  result_ref.sin_family = AF_INET;
  result_ref.sin_port = htons(port);

  // try address conversion
#if !defined(_WIN32)
  in_addr ia;
  if (1 == ::inet_pton(AF_INET, host.c_str(), &ia)) {
    result_ref.sin_addr.s_addr = ia.s_addr;
    return result;
  }
#endif

  addrinfo *res, hints = addrinfo{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = 0;
  hints.ai_protocol = 0;

  int gai_err = ::getaddrinfo(host.c_str(), NULL, &hints, &res);

#if !defined(_WIN32)
  if (gai_err == EAI_SYSTEM) {
    return std::nullopt;
  }
#endif
  if (gai_err != 0) {
    return std::nullopt;
  }

  const auto &ipv4 = reinterpret_cast<const SocketAddressIpv4 &>(res->ai_addr);
  result_ref.sin_addr.s_addr = ipv4.sin_addr.s_addr;
  ::freeaddrinfo(res);
  return result;
}

std::optional<SocketAddressIpv6> toSocketAddressIpv6(const std::string &host,
                                                     const Port &port) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  std::optional<SocketAddressIpv6> result;
  auto &result_ref = result.emplace();
  // set everything to 0 first
  ::memset(&result_ref, 0, sizeof(SocketAddressIpv6));
  result_ref.sin6_family = AF_INET6;
  result_ref.sin6_flowinfo = 0;
  result_ref.sin6_port = htons(port);

  // try address conversion
#if !defined(_WIN32)
  in6_addr ia;
  if (1 == ::inet_pton(AF_INET6, host.c_str(), &ia)) {
    result_ref.sin6_addr = ia;
    return result;
  }
#endif

  addrinfo *res, hints = addrinfo{};
  hints.ai_family = AF_INET6;
  hints.ai_socktype = 0;
  hints.ai_protocol = 0;

  int gai_err = ::getaddrinfo(host.c_str(), NULL, &hints, &res);

#if !defined(_WIN32)
  if (gai_err == EAI_SYSTEM) {
    return std::nullopt;
  }
#endif
  if (gai_err != 0) {
    return std::nullopt;
  }

  const auto &ipv6 = reinterpret_cast<const SocketAddressIpv6 &>(res->ai_addr);
  result_ref.sin6_addr = ipv6.sin6_addr;
  ::freeaddrinfo(res);
  return result;
}

std::optional<Port> toPort(const SocketAddress &address) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  switch (address.sa_family) {
  case AF_INET: {
    return ntohs(reinterpret_cast<const SocketAddressIpv4 &>(address).sin_port);

  } break;
  case AF_INET6: {
    return ntohs(
        reinterpret_cast<const SocketAddressIpv6 &>(address).sin6_port);

  } break;
  }
  return std::nullopt;
}

std::optional<std::string> toHost(const SocketAddress &address) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  // refer to
  // https://stackoverflow.com/questions/11684008/how-do-you-cast-sockaddr-structure-to-a-sockaddr-in-c-networking-sockets-ubu
  switch (address.sa_family) {
  case AF_INET: {
    // ipv4 address
    // inet_ntoa is deprecated, but using inet_ntop for ipv4 address, leads to
    // an ip that has no sense
    return std::string(::inet_ntoa(
        reinterpret_cast<const SocketAddressIpv4 &>(address).sin_addr));

  } break;
  case AF_INET6: {
    // ipv6 address
    char temp[INET6_ADDRSTRLEN]; // this is the longest one
    // refer to
    // https://www.gnu.org/software/libc/manual/html_node/Host-Address-Functions.html
    ::memset(temp, 0, INET6_ADDRSTRLEN);
    ::inet_ntop(address.sa_family, &address, temp, INET6_ADDRSTRLEN);
    return std::string(temp, INET6_ADDRSTRLEN);
  } break;
  }
  return std::nullopt;
}

Address toAddress(const SocketAddress &address) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  auto host = toHost(address);
  auto port = toPort(address);
  if (host && port) {
    return Address{*host, *port};
  }

  return Address{"", 0};
}
} // namespace MinimalSocket
