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
#elif defined(__linux__) || defined(__APPLE__)
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
