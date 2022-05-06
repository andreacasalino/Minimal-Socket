/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <SynchSocket/Address.h>
#include <SynchSocket/detail/Socket.h>

#include <atomic>
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

/**
 * socket handle
 */
#ifdef _WIN32
using SocketHandlerType = SOCKET;
#else
using SocketHandlerType = int;
#endif

/**
 * An object storing a socket API handler and containing the minimal
 * functionalities for interacting with it.
 */
class SocketHandler {
public:
  SocketHandler(const SocketHandler &) = delete;
  SocketHandler &operator=(const SocketHandler &) = delete;

  /**
   * @brief a closed socket is created
   */
  SocketHandler() = default;

  ~SocketHandler();

  bool empty() const { return socket_id == SCK_INVALID_SOCKET; }

  /**
   * @brief internally creates a new socket
   */
  void reset(const Protocol &type, const AddressFamily &family);

  /**
   * @brief the passed handler should be already created externally
   * by the socket api
   */
  void reset(const SocketHandlerType &hndl);

  /**
   * @brief close and shutdown the current socket
   */
  void close();

  const SocketHandlerType &access() const { return socket_id; };

private:
  SocketHandlerType socket_id = SCK_INVALID_SOCKET;

#ifdef _WIN32
  class SocketHandlerFactory {
  public:
    /**
     * @brief If we are about to open the first socket, WSAStartup() is invoked
     */
    static void beforeOpen();
    /**
     * @brief If we are closing the last socket, WSACleanup() is invoked
     */
    static void afterClose();

  private:
    static std::mutex handlerCounterMtx;
    static std::size_t handlerCounter;
  };
#endif
};
} // namespace MinCppSock
