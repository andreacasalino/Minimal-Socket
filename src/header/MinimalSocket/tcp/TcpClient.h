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
class TcpClient : public Openable,
                  public Sender,
                  public Receiver,
                  public RemoteAddressAware {
public:
  TcpClient(TcpClient &&o);
  TcpClient &operator=(TcpClient &&o);

  /**
   * @brief The connection to the server is not asked in this c'tor which
   * simply initialize this object. Such a connection is tried to be established
   * when calling open(...)
   * @param server_address the server to reach when opening this socket
   */
  TcpClient(const Address &server_address);

protected:
  void open_() override;
};
} // namespace MinimalSocket::tcp
