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
class TcpConnection : public Sender,
                      public Receiver,
                      public RemoteAddressAware {
public:
  TcpConnection(TcpConnection &&o);
  TcpConnection &operator=(TcpConnection &&o);
};

class TcpServer : public BindedPortAware {
public:
  TcpServer(TcpServer &&o);
  TcpServer &operator=(TcpServer &&o);

  TcpServer(const Port &port);

  TcpConnection &&acceptNewClient();
};
} // namespace MinimalSocket::tcp
