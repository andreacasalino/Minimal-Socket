/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>
#include <MinimalSocket/core/SocketContext.h>

#include <atomic>
#include <mutex>

namespace MinimalSocket::tcp {
class TcpServer;
/**
 * @brief An already accepted connection to a client.
 * An istance of this object can be built by before creating a TcpServer, open
 * it and call acceptNewClient().
 */
class TcpConnection : public Sender,
                      public Receiver,
                      public RemoteAddressAware {
  friend class TcpServer;

public:
  TcpConnection(TcpConnection &&o);
  TcpConnection &operator=(TcpConnection &&o);

private:
  TcpConnection(const Address &remote_address);
};

class TcpServer : public PortToBindAware,
                  public RemoteAddressFamilyAware,
                  public virtual Socket,
                  public Openable {
public:
  TcpServer(TcpServer &&o);
  TcpServer &operator=(TcpServer &&o);

  /**
   * @brief The port is not reserved in this c'tor which
   * simply initialize this object. Such a thing is done when
   * when calling open(...) which does:
   * bind to the specified port this server
   * start listening for clients on the same port
   * @param port_to_bind the port that will be reserved when opening this server
   * @param accepted_client_family family of the client that will ask the
   * connection to this server
   */
  TcpServer(const Port port_to_bind = ANY_PORT,
            const AddressFamily &accepted_client_family = AddressFamily::IP_V4);

  /**
   * @brief Wait till accepting the connection from a new client. This is a
   * blocking operation.
   */
  TcpConnection acceptNewClient(); // blocking

  /**
   * @brief Wait till accepting the connection from a new client. In case such a
   * connection is not asked within the specified timeout, a nullopt is
   * returned.
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking accept.
   */
  std::optional<TcpConnection> acceptNewClient(const Timeout &timeout);

  /**
   * @param queue_size the backlog size to assume when the server will be
   * opened, refer also to
   * https://www.linuxjournal.com/files/linuxjournal.com/linuxjournal/articles/023/2333/2333s2.html#:~:text=TCP%20listen()%20Backlog,queue%20of%20partially%20open%20connections.
   * (valid also for Windows)
   * @throw in case the server was already opened.
   */
  void setClientQueueSize(const std::size_t queue_size);

protected:
  void open_() override;

private:
  std::atomic<std::size_t> client_queue_size =
      50; // maximum number of clients put in the queue wiating for connection
  // to be accepted

  std::mutex accept_mtx;
};
} // namespace MinimalSocket::tcp
