/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Sender.h>

#include "../SocketAddress.h"
#include "../SocketError.h"
#include "../Utils.h"

namespace MinimalSocket {
bool Sender::send(const ConstBuffer &message) {
  std::scoped_lock lock(send_mtx);
  int sentBytes = ::send(getIDWrapper().accessId(), message.buffer,
                         static_cast<int>(message.buffer_size), 0);
  if (sentBytes == SCK_SOCKET_ERROR) {
    sentBytes = 0;
    auto err = SocketError{"send failed"};
    throw err;
  }
  return (sentBytes == static_cast<int>(message.buffer_size));
}

bool Sender::send(const std::string &message) {
  return send(makeStringConstBuffer(message));
}

bool SenderTo::sendTo(const ConstBuffer &message, const Address &recipient) {
  std::scoped_lock lock(send_mtx);
  int sentBytes;
  visitAddress(
      recipient.getFamily(),
      [&]() {
        auto socketIp4 =
            toSocketAddressIpv4(recipient.getHost(), recipient.getPort());
        sentBytes = ::sendto(
            getIDWrapper().accessId(), message.buffer,
            static_cast<int>(message.buffer_size), 0,
            reinterpret_cast<const SocketAddress *>(&socketIp4.value()),
            sizeof(SocketAddressIpv4));
      },
      [&]() {
        auto socketIp6 =
            toSocketAddressIpv6(recipient.getHost(), recipient.getPort());
        sentBytes = ::sendto(
            getIDWrapper().accessId(), message.buffer,
            static_cast<int>(message.buffer_size), 0,
            reinterpret_cast<const SocketAddress *>(&socketIp6.value()),
            sizeof(SocketAddressIpv6));
      });
  if (sentBytes == SCK_SOCKET_ERROR) {
    sentBytes = 0;
    auto err = SocketError{"sendto failed"};
    throw err;
  }
  return (sentBytes == static_cast<int>(message.buffer_size));
}

bool SenderTo::sendTo(const std::string &message, const Address &recipient) {
  return sendTo(makeStringConstBuffer(message), recipient);
}
} // namespace MinimalSocket
