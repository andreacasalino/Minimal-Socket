/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/RemoteAddressAware.h>
#include <MinimalSocket/core/Sender.h>

namespace MinimalSocket::tcp {
class TcpClient : public Sender, public Receiver, public RemoteAddressAware {
public:
  TcpClient(TcpClient &&o);
  TcpClient &operator=(TcpClient &&o);

  TcpClient(const Address &server_address);
};
} // namespace MinimalSocket::tcp
