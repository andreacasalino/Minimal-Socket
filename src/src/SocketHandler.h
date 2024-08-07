/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

// https://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch
#ifdef _WIN32
#include <stdio.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <errno.h>
#include <string.h> //memset
#include <sys/socket.h>
#include <unistd.h> //close
#else
#error "Not supported system by MinimalSocket"
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
class SocketHandler {
public:
  SocketHandler(const SocketHandler &) = delete;
  SocketHandler &operator=(const SocketHandler &) = delete;
  SocketHandler(SocketHandler &&) = delete;
  SocketHandler &operator=(SocketHandler &&) = delete;

  auto accessId() const { return socket_id; };

  /**
   * @brief an invalid socket id is created
   */
  SocketHandler() = default;

  /**
   * @brief internally calls shut down
   */
  ~SocketHandler();

  void shutDown();

  /**
   * @brief regenerates the socket descriptor, i.e. creates a new socket
   */
  void reset(SocketType type, AddressFamily family);

  /**
   * @brief the passed handler should be already externally created and setup
   * (for blocking or non blocking node).
   */
  void reset(SocketID hndl);

private:
  SocketID socket_id = SCK_INVALID_SOCKET;
};

#ifdef _WIN32
class WSALazyInitializer {
public:
  static void lazyInit();

  ~WSALazyInitializer();

private:
  WSALazyInitializer(const WSAVersion &version);

  static std::mutex lazy_proxy_mtx;
  static std::unique_ptr<WSALazyInitializer> lazy_proxy;

  const WSAVersion configured_version;
};
#endif
} // namespace MinimalSocket
