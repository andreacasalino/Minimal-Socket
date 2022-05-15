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
