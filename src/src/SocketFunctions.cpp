/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "SocketAddress.h"
#include "SocketError.h"
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
          throwWithLastErrorCode("can't bind localhost on port: " +
                                 std::to_string(port));
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
          throwWithLastErrorCode("Connection can't be established");
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
          throwWithLastErrorCode("Connection can't be established");
        }
      });
}
} // namespace MinimalSocket