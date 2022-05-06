/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <SynchSocket/Address.h>

#include <optional>

#ifdef _WIN32
#include <stdio.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#define SCK_INVALID_SOCKET INVALID_SOCKET
#define SCK_SOCKET_ERROR SOCKET_ERROR
#else
#include <arpa/inet.h>
#include <errno.h>
#include <string.h> //memset
#include <sys/socket.h>
#include <unistd.h> //close
#define SCK_INVALID_SOCKET -1
#define SCK_SOCKET_ERROR -1
#endif

namespace MinCppSock {
/**
 * socket handle
 */
#ifdef _WIN32
using SocketHandler = SOCKET;
#else
using SocketHandler = int;
#endif
/**
 * @brief representation of a generic socket address
 */
#ifdef _WIN32
using SocketIp = SOCKADDR;
#else
using SocketIp = sockaddr;
#endif
/**
 * @brief representation of an ipv4 socket address
 */
#ifdef _WIN32
using SocketIp4 = SOCKADDR_IN;
#else
using SocketIp4 = sockaddr_in;
#endif
/**
 * @brief representation of an ipv6 socket address
 */
#ifdef _WIN32
using SocketIp6 = SOCKADDR_IN6;
#else
using SocketIp6 = sockaddr_in6;
#endif

/**
 * @brief returns the last error code raised by the socket API
 */
int getLastErrorCode();

void throwWithLastErrorCode(const std::string &what);

/**
 * @brief checks the address syntax and in case
 * it's valid as an ipv4, creates the socket API representation
 * of the address
 */
std::optional<SocketIp4> makeSocketIp4(const std::string &raw_address,
                                       const Port &port);
/**
 * @brief checks the address syntax and in case
 * it's valid as an ipv6, creates the socket API representation
 * of the address
 */
std::optional<SocketIp6> makeSocketIp6(const std::string &raw_address,
                                       const Port &port);
/**
 * @brief Convert a SocketAddress_t into an Address, internally
 * deducing the family.
 */
Address make_address(const SocketIp &address);
} // namespace MinCppSock
