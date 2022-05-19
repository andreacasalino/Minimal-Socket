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
#include <optional>

namespace MinimalSocket {
#ifdef _WIN32
using WSAVersion = std::array<std::uint16_t, 2>;

/**
 * @brief Refer to
 * https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
 * In windows, any application using sockets, need to initialize the WSA data
 * structure. When doing this, the version to use must be specified. This will
 * be internally handled by this library, before calling any fucntion that
 * requires the WSA data to be already initialized.
 * The WSA data will be automatically cleaned up when the process terminates.
 *
 * This object allows you specify the WSA version used to initialize the WSA
 * data. When calling setWsaVersion(...) you can change the WSA version and such
 * information will be used next time the WSA data will be (automatically and
 * internally) initialized. Clearly, in case the WSA data was already
 * initialized, as a consequence of creating any kind of sockets or using one of
 * the features defined in Address.h, setWsaVersion has actually no effect.
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

class SocketIdWrapper;

/**
 * @brief The base onject of any kind of socket.
 */
class Socket {
public:
  virtual ~Socket();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  /**
   * @return the socket id associated to this object.
   *
   * This might be:
   *
   * https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
   * in windows is the SOCKET (cast as in integer) data used to identify the
   * underlying socket and returned by ::socket(...)
   *
   * https://man7.org/linux/man-pages/man2/socket.2.html
   * in UNIX is the integer used to identify the underlying socket and returned
   * by ::socket(...)
   *
   * Normally, all the operations involving the socket should be handled by the
   * functions provided by this library. Therefore, you shouldn't need to
   * access this number However, it might happen that sometimes you want to
   * specify additional socket option, and in order to do that you need this
   * number. Beware that you should really know what you are doing when using
   * this number.
   */
  int accessSocketID() const;

protected:
  Socket();

  static void transfer(Socket &receiver, Socket &giver);

  const SocketIdWrapper &getIDWrapper() const;
  SocketIdWrapper &getIDWrapper();
  void resetIDWrapper();

private:
  std::unique_ptr<SocketIdWrapper> socket_id_wrapper;
};

class Openable : public virtual Socket {
public:
  bool wasOpened() const { return opened; }

  /**
   * @brief Tries to do all the steps required to open this socket. In case
   * potentially expected expections are raised, they are interally catched and
   asserted.
   * On the opposite, unexpected expections are passed to the caller.
   * In both cases, the object is inernally closed and left in a state for which
   * open may be tried again.
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking open.
   */
  bool open(const Timeout &timeout = NULL_TIMEOUT);

protected:
  Openable() = default;

  static void transfer(Openable &receiver,
                       Openable &giver); // Socket::transfer(...) is also called

  virtual void open_() = 0;

private:
  std::mutex open_procedure_mtx;
  std::atomic_bool opened = false;
};
} // namespace MinimalSocket
