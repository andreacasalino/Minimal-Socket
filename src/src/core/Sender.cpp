/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Sender.h>

#include "../SocketAddress.h"
#include "../SocketFunctions.h"
#include "../Utils.h"

namespace MinimalSocket {
bool Sender::send(const BufferViewConst &message) {
  std::scoped_lock lock(send_mtx);
  int sentBytes = ::send(getHandler().accessId(), message.buffer,
                         static_cast<int>(message.buffer_size), 0);
  if (checkResult(sentBytes, SCK_SOCKET_ERROR, "send failed", !isBlocking())) {
    return false;
  }
  return true;
}

bool Sender::send(const std::string &message) {
  return send(makeBufferViewConst(message));
}

std::mutex &SenderTo::getRecipientMtx(const Address &recipient) {
  std::scoped_lock lock{recipients_register_mtx};
  auto &res = recipients_register[recipient];
  if (res == nullptr) {
    res = std::make_unique<std::mutex>();
  }
  return *res;
}

bool SenderTo::sendTo(const BufferViewConst &message,
                      const Address &recipient) {
  int sentBytes;
  {
    std::scoped_lock lock{getRecipientMtx(recipient)};
    visitAddress(
        recipient.getFamily(),
        [&]() {
          auto socketIp4 =
              toSocketAddressIpv4(recipient.getHost(), recipient.getPort());
          sentBytes = ::sendto(
              getHandler().accessId(), message.buffer,
              static_cast<int>(message.buffer_size), 0,
              reinterpret_cast<const SocketAddress *>(&socketIp4.value()),
              sizeof(SocketAddressIpv4));
        },
        [&]() {
          auto socketIp6 =
              toSocketAddressIpv6(recipient.getHost(), recipient.getPort());
          sentBytes = ::sendto(
              getHandler().accessId(), message.buffer,
              static_cast<int>(message.buffer_size), 0,
              reinterpret_cast<const SocketAddress *>(&socketIp6.value()),
              sizeof(SocketAddressIpv6));
        });
  }
  if (checkResult(sentBytes, SCK_SOCKET_ERROR, "send failed", !isBlocking())) {
    return false;
  }
  return true;
}

bool SenderTo::sendTo(const std::string &message, const Address &recipient) {
  return sendTo(makeBufferViewConst(message), recipient);
}
} // namespace MinimalSocket
