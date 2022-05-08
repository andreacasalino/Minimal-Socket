/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

#ifdef _WIN32
#include <mutex>
#include <stdio.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <errno.h>
#include <string.h> //memset
#include <sys/socket.h>
#include <unistd.h> //close
#endif

namespace MinimalSocket {
#ifdef _WIN32
#define SCK_INVALID_SOCKET INVALID_SOCKET
#define SCK_SOCKET_ERROR SOCKET_ERROR
#else
#define SCK_INVALID_SOCKET -1
#define SCK_SOCKET_ERROR -1
#endif

/**
 * socket id
 */
#ifdef _WIN32
using SocketID = SOCKET;
#else
using SocketID = int;
#endif

/**
 * An object storing a socket API handler and containing the minimal
 * functionalities for interacting with it.
 */
class SocketIdWrapper {
public:
  SocketIdWrapper(const SocketIdWrapper &) = delete;
  SocketIdWrapper &operator=(const SocketIdWrapper &) = delete;

  const SocketID &accessId() const { return socket_id; };

  /**
   * @brief an invalid socket id is created
   */
  SocketIdWrapper() = default;

  /**
   * @brief close and shutdown the current socket
   */
  ~SocketIdWrapper();

  /**
   * @brief internally creates a new socket
   */
  void reset(const SocketType &type, const AddressFamily &family);

  /**
   * @brief the passed handler should be already created externally
   * by the socket api
   */
  void reset(const SocketID &hndl);

private:
  SocketID socket_id = SCK_INVALID_SOCKET;

#ifdef _WIN32
  class SocketIDFactory {
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

} // namespace MinimalSocket
