/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "SocketAddress.h"
#include "SocketFunctions.h"
#include "Utils.h"

namespace MinimalSocket {
namespace {
#ifdef _WIN32
#define REBIND_OPTION SO_REUSEADDR
#else
#define REBIND_OPTION SO_REUSEPORT
#endif
} // namespace

Port bind(const SocketID &socket_id, const AddressFamily &family,
          const Port &port, const bool must_be_free_port) {
  if (!must_be_free_port) {
    int reusePortOptVal = 1;
    ::setsockopt(socket_id, SOL_SOCKET, REBIND_OPTION,
                 reinterpret_cast<const
#ifdef _WIN32
                                  char * // not sure it would work with void*
                                         // also in Windows
#else
                                  void *
#endif
                                  >(&reusePortOptVal),
                 sizeof(int));
  }

  // bind the socket to the port
  visitAddress(
      family,
      [&]() {
        SocketAddressIpv4 addr;
        ::memset(&addr, 0, sizeof(SocketAddressIpv4));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
#ifdef _WIN32
        addr.sin_addr.s_addr = ADDR_ANY;
#else
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
        if (::bind(socket_id, reinterpret_cast<SocketAddress *>(&addr),
                   sizeof(SocketAddressIpv4)) == SCK_SOCKET_ERROR) {
          auto err = SocketError{"Can't bind localhost on port: ",
                                 std::to_string(port)};
          throw err;
        }
      },
      [&]() {
        SocketAddressIpv6 addr;
        ::memset(&addr, 0, sizeof(SocketAddressIpv6));
        addr.sin6_family = AF_INET6;
        addr.sin6_flowinfo = 0;
        addr.sin6_addr =
            IN6ADDR_ANY_INIT; // apparently, there is no such a
                              // cross-system define for ipv6 addresses
        addr.sin6_port = htons(port);
        if (::bind(socket_id, reinterpret_cast<SocketAddress *>(&addr),
                   sizeof(SocketAddressIpv6)) == SCK_SOCKET_ERROR) {
          auto err = SocketError{"Can't bind localhost on port: ",
                                 std::to_string(port)};
          throw err;
        }
      });

  Port binded_port = port;
  if (ANY_PORT == port) {
    char binded_address[MAX_POSSIBLE_ADDRESS_SIZE];
    SocketAddressLength binded_address_length = MAX_POSSIBLE_ADDRESS_SIZE;
    if (::getsockname(socket_id,
                      reinterpret_cast<SocketAddress *>(&binded_address[0]),
                      &binded_address_length) == SCK_SOCKET_ERROR) {
      auto err = SocketError{"Wasn't able to deduce the binded port"};
      throw err;
    }
    auto maybe_port =
        toPort(reinterpret_cast<const SocketAddress &>(binded_address));
    if (maybe_port) {
      binded_port = *maybe_port;
    } else {
      throw Error{"Wasn't able to deduce the binded port"};
    }
  }
  return binded_port;
}

void listen(const SocketID &socket_id, const std::size_t backlog_size) {
  if (::listen(socket_id, static_cast<int>(backlog_size)) == SCK_SOCKET_ERROR) {
    auto err = SocketError{"Error: listening on reserved port"};
    throw err;
  }
}

void connect(const SocketID &socket_id, const Address &remote_address) {
  visitAddress(
      remote_address.getFamily(),
      [&]() {
        // v4 family
        auto addr = toSocketAddressIpv4(remote_address.getHost(),
                                        remote_address.getPort());
        if (!addr) {
          throw Error(to_string(remote_address),
                      " is an invalid server address");
        }
        if (::connect(socket_id, reinterpret_cast<SocketAddress *>(&(*addr)),
                      sizeof(SocketAddressIpv4)) == SCK_SOCKET_ERROR) {
          auto err = SocketError{"Connection can't be established"};
          throw err;
        }
      },
      [&]() {
        // v6 family
        auto addr = toSocketAddressIpv6(remote_address.getHost(),
                                        remote_address.getPort());
        if (!addr) {
          throw Error(to_string(remote_address),
                      " is an invalid server address");
        }
        if (::connect(socket_id, reinterpret_cast<SocketAddress *>(&(*addr)),
                      sizeof(SocketAddressIpv6)) == SCK_SOCKET_ERROR) {
          auto err = SocketError{"Connection can't be established"};
          throw err;
        }
      });
}
} // namespace MinimalSocket