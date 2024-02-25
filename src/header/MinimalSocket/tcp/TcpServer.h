/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/NonCopiable.h>
#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>
#include <MinimalSocket/core/SocketContext.h>

#include <atomic>
#include <mutex>

namespace MinimalSocket::tcp {
class TcpServerBase;
class TcpConnectionNonBlocking;

/**
 * @brief Handler of an already established connection.
 */
class TcpConnectionBlocking : public NonCopiable,
                              public Sender,
                              public Receiver<true>,
                              public RemoteAddressAware {
  friend class TcpServerBase;

public:
  TcpConnectionBlocking(TcpConnectionBlocking &&o);
  TcpConnectionBlocking &operator=(TcpConnectionBlocking &&o);

  /**
   * @brief Beware that the giver objet is left empty.
   */
  TcpConnectionNonBlocking turnToNonBlocking();

private:
  TcpConnectionBlocking(const Address &remote_address);
};

/**
 * @brief Similar to TcpConnectionBlocking, but representing the non blocking
 * version.
 */
class TcpConnectionNonBlocking : public NonCopiable,
                                 public Sender,
                                 public Receiver<false>,
                                 public RemoteAddressAware {
public:
  TcpConnectionNonBlocking(TcpConnectionNonBlocking &&o);
  TcpConnectionNonBlocking &operator=(TcpConnectionNonBlocking &&o);

  TcpConnectionNonBlocking(TcpConnectionBlocking &&connection);
};

class TcpServerBase : public NonCopiable,
                      public PortToBindAware,
                      public RemoteAddressFamilyAware,
                      public Openable {
protected:
  TcpServerBase(TcpServerBase &&o);

  void stealBase(TcpServerBase &o);

  TcpServerBase(Port port_to_bind, AddressFamily accepted_client_family,
                bool block_mode);

public:
  /**
   * @param the maximum size of the queue of clients waiting to establish a
   * connection with this server.
   * https://www.linuxjournal.com/files/linuxjournal.com/linuxjournal/articles/023/2333/2333s2.html#:~:text=TCP%20listen()%20Backlog,queue%20of%20partially%20open%20connections.
   * (valid also for Windows)
   * @throw in case the server was already opened.
   */
  void setClientQueueSize(std::size_t queue_size);

protected:
  void open_() override;

  struct AcceptedSocket;
  void acceptClient_(AcceptedSocket &recipient);

  static TcpConnectionBlocking makeClient(const AcceptedSocket &acceptedSocket);

private:
  std::atomic<std::size_t> client_queue_size = 50;
};

class AcceptorBlocking : public TcpServerBase {
public:
  /**
   * @brief Wait till accepting the connection from a new client. This is a
   * blocking operation.
   */
  TcpConnectionBlocking acceptNewClient();

  /**
   * @brief Similar to AcceptorBlocking::acceptNewClient, but returning a non
   * blocking socket after the connection is established.
   * Notice that in any case, this operation is blocking.
   */
  TcpConnectionNonBlocking acceptNewNonBlockingClient() {
    return acceptNewClient().turnToNonBlocking();
  }

protected:
  template <typename... Args>
  AcceptorBlocking(Args &&...args)
      : TcpServerBase{std::forward<Args>(args)...} {}

private:
  std::mutex accept_mtx;
};

class AcceptorNonBlocking : public TcpServerBase {
public:
  /**
   * @brief Check if a new client tried to connect and eventually immediately
   * return the object handling the connection. This is a non blocking
   * operation: in case no client actually asked for the connection, the method
   * immeditely returns a nullopt.
   *
   * Notice that even though this operation is non
   * blocking, a blocking socket is returned.
   */
  std::optional<TcpConnectionBlocking> acceptNewClient();

  /**
   * @brief Similar to AcceptorNonBlocking::acceptNewClient, but returning a non
   * blocking socket.
   */
  std::optional<TcpConnectionNonBlocking> acceptNewNonBlockingClient() {
    auto client = acceptNewClient();
    if (client.has_value()) {
      return client->turnToNonBlocking();
    }
    return std::nullopt;
  }

protected:
  template <typename... Args>
  AcceptorNonBlocking(Args &&...args)
      : TcpServerBase{std::forward<Args>(args)...} {}

private:
  std::mutex accept_mtx;
};

template <bool BlockMode> class Acceptor {};
template <> class Acceptor<true> : public AcceptorBlocking {
protected:
  template <typename... Args>
  Acceptor(Args &&...args) : AcceptorBlocking{std::forward<Args>(args)...} {}
};
template <> class Acceptor<false> : public AcceptorNonBlocking {
protected:
  template <typename... Args>
  Acceptor(Args &&...args) : AcceptorNonBlocking{std::forward<Args>(args)...} {}
};

template <bool BlockMode> class TcpServer : public Acceptor<BlockMode> {
public:
  /**
   * @brief After construction, the socket is left closed. Only after calling
   * open(...), the port is binded and the server starts to listen for
   * connection request on that port.
   * @param the port that will be reserved when opening this server
   * @param family of the client that will ask the
   * connection to this server
   */
  TcpServer(Port port_to_bind = 0,
            AddressFamily accepted_client_family = AddressFamily::IP_V4)
      : Acceptor<BlockMode>{port_to_bind, accepted_client_family, BlockMode} {}

  TcpServer(TcpServer<BlockMode> &&o)
      : Acceptor<BlockMode>{std::forward<Acceptor<BlockMode>>(o)} {}
  TcpServer &operator=(TcpServer<BlockMode> &&o) {
    this->stealBase(o);
    return *this;
  }
};
} // namespace MinimalSocket::tcp
