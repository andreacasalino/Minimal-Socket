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

namespace MinimalSocket::tcp {
class TcpServer;

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

  TcpServer(const Port port_to_bind = ANY_PORT,
            const AddressFamily &accepted_client_family = AddressFamily::IP_V4);

  TcpConnection acceptNewClient(); // blocking

  std::optional<TcpConnection> acceptNewClient(const Timeout &timeout);

  void setClientQueueSize(const std::size_t queue_size);

protected:
  void open_() override;

private:
  std::size_t client_queue_size =
      50; // maximum number of clients put in the queue wiating for connection
          // to be accepted
};
} // namespace MinimalSocket::tcp
