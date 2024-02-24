/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include "SocketHandler.h"

namespace MinimalSocket {
/**
 * @brief representation of a generic socket address
 */
#ifdef _WIN32
using SocketAddress = SOCKADDR;
#else
using SocketAddress = sockaddr;
#endif

#ifdef _WIN32
using SocketAddressLength = int;
#else
using SocketAddressLength = unsigned int;
#endif

/**
 * @brief representation of an ipv4 socket address
 */
#ifdef _WIN32
using SocketAddressIpv4 = SOCKADDR_IN;
#else
using SocketAddressIpv4 = sockaddr_in;
#endif
/**
 * @brief representation of an ipv6 socket address
 */
#ifdef _WIN32
using SocketAddressIpv6 = SOCKADDR_IN6;
#else
using SocketAddressIpv6 = sockaddr_in6;
#endif

static constexpr std::size_t MAX_POSSIBLE_ADDRESS_SIZE =
    std::max<std::size_t>(sizeof(SocketAddressIpv4), sizeof(SocketAddressIpv6));

/**
 * @brief checks the address syntax and in case
 * it's valid as an ipv4, creates the socket API representation
 * of the address
 */
std::optional<SocketAddressIpv4> toSocketAddressIpv4(const std::string &host,
                                                     Port port);
/**
 * @brief checks the address syntax and in case
 * it's valid as an ipv6, creates the socket API representation
 * of the address
 */
std::optional<SocketAddressIpv6> toSocketAddressIpv6(const std::string &host,
                                                     Port port);

std::optional<Port> toPort(const SocketAddress &address);

std::optional<std::string> toHost(const SocketAddress &address);

/**
 * @brief Convert a SocketAddress_t into an Address, internally
 * deducing the family.
 */
Address toAddress(const SocketAddress &address);
} // namespace MinimalSocket
