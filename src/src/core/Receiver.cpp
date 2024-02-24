/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Receiver.h>

#include "../SocketAddress.h"
#ifndef _WIN32
#include <sys/time.h>
#endif

namespace MinimalSocket {
void ReceiverBase::updateTimeout_(const Timeout &timeout) {
  if (timeout == receive_timeout) {
    return;
  }
  receive_timeout = timeout;
  // set new timeout
#ifdef _WIN32
  auto tv = DWORD(this->receive_timeout.count());
  if (setsockopt(getHandler().accessId(), SOL_SOCKET, SO_RCVTIMEO,
                 reinterpret_cast<const char *>(&tv),
                 sizeof(DWORD)) == SOCKET_ERROR) {
#else
  struct timeval tv = {0, 0};
  if (this->receive_timeout.count() >= 1000) {
    tv.tv_sec =
        std::chrono::duration_cast<std::chrono::seconds>(receive_timeout)
            .count();
  } else {
    tv.tv_usec =
        std::chrono::duration_cast<std::chrono::microseconds>(receive_timeout)
            .count();
  }
  if (::setsockopt(getHandler().accessId(), SOL_SOCKET, SO_RCVTIMEO,
                   static_cast<const void *>(&tv),
                   sizeof(struct timeval)) != 0) {
#endif
    throw SocketError{"can't set timeout"};
  }
}

namespace {
#ifdef _WIN32
static constexpr int TIMEOUT_CODE = 10060;
#else
static constexpr int TIMEOUT_CODE = EAGAIN;
#endif

void check_received_bytes(int &recvBytes, const Timeout &timeout) {
  if (recvBytes != SCK_SOCKET_ERROR) {
    return;
  }
  SocketError error_with_code("receive failed");
  recvBytes = 0;
  if ((error_with_code.getErrorCode() == TIMEOUT_CODE) &&
      (timeout != NULL_TIMEOUT)) {
    // just out of time: tolerable
    return;
  }
  throw error_with_code;
}
} // namespace

std::size_t Receiver::receive(BufferView message, const Timeout &timeout) {
  std::size_t res = 0;

  lazyUpdateAndUseTimeout(
      timeout, [&message, &res, this](const Timeout &timeout) {
        clear(message);

        int recvBytes = ::recv(getHandler().accessId(), message.buffer,
                               static_cast<int>(message.buffer_size), 0);
        check_received_bytes(recvBytes, timeout);
        if (recvBytes > message.buffer_size) {
          // if here, the message received is probably corrupted
          recvBytes = 0;
        }
        res = static_cast<std::size_t>(recvBytes);
      });

  return res;
}

std::string Receiver::receive(std::size_t expected_max_bytes,
                              const Timeout &timeout) {
  std::string buffer;
  buffer.resize(expected_max_bytes);
  auto buffer_temp = makeBufferView(buffer);
  auto recvBytes = receive(buffer_temp, timeout);
  buffer.resize(recvBytes);
  return buffer;
}

std::optional<ReceiverUnkownSender::ReceiveResult>
ReceiverUnkownSender::receive(BufferView message, const Timeout &timeout) {
  std::optional<ReceiverUnkownSender::ReceiveResult> res;

  lazyUpdateAndUseTimeout(
      timeout, [&message, &res, this](const Timeout &timeout) {
        clear(message);

        char sender_address[MAX_POSSIBLE_ADDRESS_SIZE];
        SocketAddressLength sender_address_length = MAX_POSSIBLE_ADDRESS_SIZE;

        int recvBytes =
            ::recvfrom(getHandler().accessId(), message.buffer,
                       static_cast<int>(message.buffer_size), 0,
                       reinterpret_cast<SocketAddress *>(&sender_address[0]),
                       &sender_address_length);
        check_received_bytes(recvBytes, timeout);
        if (recvBytes > message.buffer_size) {
          // if here, the message received is probably corrupted
          return;
        }
        if (0 == recvBytes) {
          // if here, timeout was reached
          return;
        }

        res = ReceiveResult{
            toAddress(reinterpret_cast<const SocketAddress &>(sender_address)),
            static_cast<std::size_t>(recvBytes)};
      });

  return res;
}

std::optional<ReceiverUnkownSender::ReceiveStringResult>
ReceiverUnkownSender::receive(std::size_t expected_max_bytes,
                              const Timeout &timeout) {
  std::string buffer;
  buffer.resize(expected_max_bytes);
  auto buffer_temp = makeBufferView(buffer);
  auto result = receive(buffer_temp, timeout);
  if (!result) {
    return std::nullopt;
  }
  buffer.resize(result->received_bytes);
  return ReceiveStringResult{std::move(result->sender), std::move(buffer)};
}
} // namespace MinimalSocket
