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

namespace MinimalSocket {
std::optional<SocketAddressIpv4> toSocketAddressIpv4(const std::string &host,
                                                     Port port) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  std::optional<SocketAddressIpv4> result;
  result.emplace();
  // set everything to 0 first
  ::memset(&result.value(), 0, sizeof(SocketAddressIpv4));
  result->sin_family = AF_INET;
  result->sin_port = htons(port);

  // try address conversion with inet_pton first
#ifdef _WIN32
  in_addr ia;
  if (1 == ::inet_pton(AF_INET, host.c_str(), &ia)) {
    ::memcpy(&result->sin_addr, &ia, sizeof(in_addr));
    return result;
  }
#else
  in_addr ia;
  if (1 == ::inet_pton(AF_INET, host.c_str(), &ia)) {
    result->sin_addr.s_addr = ia.s_addr;
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

  const auto *ipv4 = reinterpret_cast<const SocketAddressIpv4 *>(res->ai_addr);
  result->sin_addr.s_addr = ipv4->sin_addr.s_addr;
  ::freeaddrinfo(res);
  return result;
}

std::optional<SocketAddressIpv6> toSocketAddressIpv6(const std::string &host,
                                                     Port port) {
#ifdef _WIN32
  WSALazyInitializer::lazyInit();
#endif

  std::optional<SocketAddressIpv6> result;
  result.emplace();
  // set everything to 0 first
  ::memset(&result.value(), 0, sizeof(SocketAddressIpv6));
  result->sin6_family = AF_INET6;
  result->sin6_flowinfo = 0;
  result->sin6_port = htons(port);

  // try address conversion with inet_pton first
#ifdef _WIN32
  in6_addr ia;
  if (1 == ::inet_pton(AF_INET6, host.c_str(), &ia)) {
    ::memcpy(&result->sin6_addr, &ia, sizeof(in6_addr));
    return result;
  }
#else
  in6_addr ia;
  if (1 == ::inet_pton(AF_INET6, host.c_str(), &ia)) {
    result->sin6_addr = ia;
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

  const auto *ipv6 = reinterpret_cast<const SocketAddressIpv6 *>(res->ai_addr);
  result->sin6_addr = ipv6->sin6_addr;
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

  std::optional<std::string> result;
  switch (address.sa_family) {
  case AF_INET: {
    {
      const auto &as_addr_in =
          reinterpret_cast<const SocketAddressIpv4 &>(address).sin_addr;
      char host_converted[INET_ADDRSTRLEN];
      ::memset(&host_converted[0], 0, INET_ADDRSTRLEN);
      const char *result_raw = ::inet_ntop(AF_INET, &as_addr_in,
                                           &host_converted[0], INET_ADDRSTRLEN);
      if (NULL != result_raw) {
        result.emplace(result_raw);
        break;
      }
    }
    {
      // try also inet_ntoa
      result.emplace(::inet_ntoa(
          reinterpret_cast<const SocketAddressIpv4 &>(address).sin_addr));
      if ("0" == result.value()) {
        result.reset();
      }
    }
  } break;
  case AF_INET6: {
    const auto &as_addr_in_6 =
        reinterpret_cast<const SocketAddressIpv6 &>(address).sin6_addr;
    char host_converted[INET6_ADDRSTRLEN];
    ::memset(&host_converted[0], 0, INET6_ADDRSTRLEN);
    const char *result_raw = ::inet_ntop(AF_INET6, &as_addr_in_6,
                                         &host_converted[0], INET6_ADDRSTRLEN);
    if (NULL != result_raw) {
      result.emplace(result_raw);
    }
  } break;
  }
  return result;
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
