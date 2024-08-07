/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Definitions.h>

#ifdef _WIN32
#include <array>
#endif
#include <atomic>
#include <memory>
#include <mutex>

namespace MinimalSocket {
#ifdef _WIN32
using WSAVersion = std::array<std::uint16_t, 2>;

/**
 * @brief Refer to
 * https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
 * When operating under Windows, any application using sockets need to
 * initialize the WSA data structure. When doing this, the version to use must
 * be specified. This is internally handled, before using any relevant
 * functionality. The WSA data will be automatically cleaned up when the process
 * terminates.
 *
 * This object allows you to specify the WSA version to use. When calling
 * setWsaVersion(...) you can change the WSA version, as this information will
 * be used next time the WSA data will be (automatically and internally)
 * initialized. Clearly, in case the WSA data was already initialized, as a
 * consequence of creating any kind of sockets or using one of the features
 * defined in Address.h, setWsaVersion has actually no effect.
 */
class WSAManager {
public:
  static void setWsaVersion(const WSAVersion &version);
  static WSAVersion getWsaVersion();

private:
  static std::mutex wsa_version_mtx;
  static WSAVersion wsa_version;
};
#endif

class SocketHandler;

/**
 * @brief The base object for any socket.
 */
class Socket {
public:
  virtual ~Socket();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  /**
   * @return the file descriptor associated to this socket.
   *
   * This could be:
   *
   * https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
   * in Windows is the SOCKET (cast as an integer) used to identify the
   * underlying socket and returned by ::socket(...)
   *
   * https://man7.org/linux/man-pages/man2/socket.2.html
   * in UNIX is the integer used to identify the underlying socket and returned
   * by ::socket(...)
   *
   * Normally, all the operations involving the socket should be handled by the
   * functions provided by this library. Therefore, you shouldn't need to
   * access this value. However, it might happen that you want to
   * specify additional options, and in order to do so you need to access such
   * value. Beware that you should really know what you are doing when using
   * this value.
   */
  int getSocketDescriptor() const;

  /**
   * @return true in case the socket is blocking, i.e. all possible operations
   * absorb the calling thread till completion. Otherwise false, for a
   * socket configured to be non blocking, i.e. operations immediately
   * succeed or not.
   */
  bool isBlocking() const { return isBlocking_; }

  /**
   * @brief shutdown this socket. Any pending operation like blocking read or blocking client
   * accept done from any other thread are consequently terminated.
   */
  void shutDown();

protected:
  Socket();

  void steal(Socket &giver);
  void transfer(Socket &recipient) { recipient.steal(*this); }

  const SocketHandler &getHandler() const;
  SocketHandler &getHandler();
  void resetHandler();

  void setNonBlocking() { isBlocking_ = false; }
  void setUp();

private:
  bool isBlocking_ = true;
  std::unique_ptr<SocketHandler> socket_id_wrapper;
};

class OpenableBase : public virtual Socket {
public:
  virtual ~OpenableBase() = default;
  bool wasOpened() const { return opened; }

protected:
  OpenableBase() = default;

  void steal(OpenableBase &giver); // Socket::steal(...) is also called
  void transfer(OpenableBase &recipient) { recipient.steal(*this); }

  virtual void open_() = 0;

  std::mutex open_procedure_mtx;
  std::atomic_bool opened = false;
};

class Openable : public OpenableBase {
public:
  /**
   * @brief Tries to execute all required steps in order to open this socket.
   * Expections (but actually only those extending the Error class) thrown will
   * doing so, are interally catched, terminating the opening phase and
   * consequently leaving the socket closed. On the opposite, unexpected
   * expections are passed to the caller. In both cases, the object is inernally
   * closed and left in a state for which open may be tried again.
   */
  bool open();
};

class OpenableWithTimeout : public OpenableBase {
public:
  /**
   * @brief Tries to execute all required steps in order to open this socket.
   * Expections (but actually only those extending the Error class) thrown will
   * doing so, are interally catched, terminating the opening phase and
   * consequently leaving the socket closed. On the opposite, unexpected
   * expections are passed to the caller. In both cases, the object is inernally
   * closed and left in a state for which open may be tried again.
   * @param the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking open. On the contrary, in case the open steps are not
   * completely within the specified timeout, function returns to the caller and
   * leave the socket closed.
   */
  bool open(const Timeout &timeout = NULL_TIMEOUT);
};
} // namespace MinimalSocket
