/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Sender.h>

#include "../SocketAddress.h"
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

std::future<void> SenderTo::reserveAddress(const Address &to_reserve) {
  std::scoped_lock lock(recipients_register_mtx);
  auto it = recipients_register.find(to_reserve);
  if (it == recipients_register.end()) {
    auto &promises = recipients_register[to_reserve];
    promises.emplace_back();
    auto &promise = promises.back();
    auto result = promise.get_future();
    promise.set_value();
    return result;
  }
  auto &promises = it->second;
  promises.emplace_back();
  auto &promise = promises.back();
  return promise.get_future();
}

void SenderTo::freeAddress(const Address &to_reserve) {
  std::scoped_lock lock(recipients_register_mtx);
  auto it = recipients_register.find(to_reserve);
  auto &promises = it->second;
  if (1 == promises.size()) {
    recipients_register.erase(it);
  } else {
    promises.pop_front();
    promises.front().set_value();
  }
}

bool SenderTo::sendTo(const ConstBuffer &message, const Address &recipient) {
  auto send_allowed = reserveAddress(recipient);
  send_allowed.wait();
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
  freeAddress(recipient);
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
