/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_COMMONS_H_
#define _CROSS_SOCKET_COMMONS_H_

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 
#include <ws2ipdef.h>
#define SCK_INVALID_SOCKET INVALID_SOCKET
#define SCK_SOCKET_ERROR   SOCKET_ERROR
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h> //memset
#include <unistd.h> //close
#include <errno.h>
#define SCK_INVALID_SOCKET -1
#define SCK_SOCKET_ERROR   -1
#endif
#include <Ip.h>
#include <atomic>
#include <mutex>

namespace sck {
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

   void throwWithCode(const std::string& what);

   /**
   * @brief checks the address syntax and in case
   * it's valid as an ipv4, creates the socket API representation
   * of the address
   */
   std::unique_ptr<SocketIp4> convertIpv4(const sck::Ip& address);
   /**
   * @brief checks the address syntax and in case
   * it's valid as an ipv6, creates the socket API representation
   * of the address
   */
   std::unique_ptr<SocketIp6> convertIpv6(const sck::Ip& address);
   /**
    * @brief Convert a SocketAddress_t into an Address, internally
    * deducing the family.
    */
   IpPtr convert(const SocketIp& address);
}

#endif