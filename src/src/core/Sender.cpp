/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Sender.h>

#include "../Commons.h"

namespace MinimalSocket {
bool Sender::send(const Buffer &message) {
  std::scoped_lock lock(send_mtx);
  int sentBytes = ::send(getIDWrapper().access(), message.data(),
                         static_cast<int>(message.size()), 0);
  if (sentBytes == SCK_SOCKET_ERROR) {
    sentBytes = 0;
    throwWithLastErrorCode("send failed");
  }
  return (sentBytes == static_cast<int>(message.size()));
}

bool SenderTo::sendTo(const Buffer &message, const Address &recipient) {
  std::scoped_lock lock(send_mtx);
  int sentBytes;
  address_case(
      recipient.getFamily(),
      [&]() {
        auto socketIp4 =
            makeSocketIp4(recipient.getHost(), recipient.getPort());
        sentBytes =
            ::sendto(getIDWrapper().access(), message.data(),
                     static_cast<int>(message.size()), 0,
                     reinterpret_cast<const SocketIp *>(&socketIp4.value()),
                     sizeof(SocketIp4));
      },
      [&]() {
        auto socketIp6 =
            makeSocketIp6(recipient.getHost(), recipient.getPort());
        sentBytes =
            ::sendto(getIDWrapper().access(), message.data(),
                     static_cast<int>(message.size()), 0,
                     reinterpret_cast<const SocketIp *>(&socketIp6.value()),
                     sizeof(SocketIp6));
      });
  if (sentBytes == SCK_SOCKET_ERROR) {
    sentBytes = 0;
    throwWithLastErrorCode("send to failed");
  }
  return (sentBytes == static_cast<int>(message.size()));
}
} // namespace MinimalSocket
