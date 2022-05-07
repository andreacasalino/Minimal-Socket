/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/BindedPortAware.h>
#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/RemoteAddressAware.h>
#include <MinimalSocket/core/Sender.h>

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

class TcpServer : public BindedPortAware, public Socket, public Openable {
public:
  TcpServer(TcpServer &&o);
  TcpServer &operator=(TcpServer &&o);

  TcpServer(const Port &port,
            const AddressFamily &kind_of_client = AddressFamily::IP_V4);

  TcpConnection acceptNewClient();

protected:
  bool open_() override;

private:
  AddressFamily kind_of_client_to_accept;
};
} // namespace MinimalSocket::tcp
