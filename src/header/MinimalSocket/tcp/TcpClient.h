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

namespace MinimalSocket::tcp {
class TcpClientBase : public NonCopiable,
                      public OpenableWithTimeout,
                      public Sender,
                      public RemoteAddressAware {
protected:
  TcpClientBase(TcpClientBase &&o);

  void stealBase(TcpClientBase &o);

  TcpClientBase(const Address &server_address, bool block_mode);

  void open_() override;
};

template <bool BlockMode>
class TcpClient : public TcpClientBase, public Receiver<BlockMode> {
public:
  /**
   * @brief After construction, the socket is left closed. Indeed, the
   * connection to the server is actually asked when opening the client.
   * @param server_address the server to reach when opening this socket
   */
  TcpClient(const Address &server_address)
      : TcpClientBase{server_address, BlockMode} {}

  TcpClient(TcpClient<BlockMode> &&o)
      : TcpClientBase{std::forward<TcpClientBase>(o)} {}
  TcpClient &operator=(TcpClient<BlockMode> &&o) {
    this->stealBase(o);
    return *this;
  }
};

/**
 * @return a client ready to ask the connection to the same server targeted by
 * the passed client. Beware that a closed socket is returned, which can be
 * later opened.
 */
template <bool BlockMode>
TcpClient<BlockMode> clone(const TcpClient<BlockMode> &o) {
  return TcpClient<BlockMode>{o.getRemoteAddress()};
}

} // namespace MinimalSocket::tcp
