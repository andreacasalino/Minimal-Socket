/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Receiver.h>

#include "../SocketAddress.h"
#include "../SocketFunctions.h"
#ifndef _WIN32
#include <sys/time.h>
#endif

namespace MinimalSocket {
void ReceiverWithTimeout::updateTimeout_(const Timeout &timeout) {
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

std::size_t ReceiverBlocking::receive(BufferView message,
                                      const Timeout &timeout) {
  std::scoped_lock lock{recv_mtx};
  updateTimeout_(timeout);
  clear(message);

  int recvBytes = ::recv(getHandler().accessId(), message.buffer,
                         static_cast<int>(message.buffer_size), 0);
  if (checkResult(recvBytes, SCK_SOCKET_ERROR, "receive failed",
                  timeout != NULL_TIMEOUT) ||
      recvBytes > message.buffer_size) {
    return 0;
  }
  return static_cast<std::size_t>(recvBytes);
}

std::size_t ReceiverNonBlocking::receive(BufferView message) {
  std::scoped_lock lock{recv_mtx};
  clear(message);

  int recvBytes = ::recv(getHandler().accessId(), message.buffer,
                         static_cast<int>(message.buffer_size), 0);
  if (checkResult(recvBytes, SCK_SOCKET_ERROR, "receive failed", true) ||
      recvBytes > message.buffer_size) {
    return 0;
  }
  return static_cast<std::size_t>(recvBytes);
}

namespace {
template <typename Pred, typename... Args>
std::string receive_into_string(std::size_t expected_max_bytes, Pred pred,
                                const Args &...args) {
  std::string buffer;
  buffer.resize(expected_max_bytes);
  auto buffer_temp = makeBufferView(buffer);
  auto recvBytes = pred(buffer_temp, args...);
  buffer.resize(recvBytes);
  return buffer;
}
} // namespace

std::string ReceiverBlocking::receive(std::size_t expected_max_bytes,
                                      const Timeout &timeout) {
  return receive_into_string(
      expected_max_bytes,
      [this](BufferView message, const Timeout &timeout) {
        return this->receive(message, timeout);
      },
      timeout);
}

std::string ReceiverNonBlocking::receive(std::size_t expected_max_bytes) {
  return receive_into_string(expected_max_bytes, [this](BufferView message) {
    return this->receive(message);
  });
}

std::optional<ReceiveResult>
ReceiverUnkownSenderBlocking::receive(BufferView message,
                                      const Timeout &timeout) {
  std::scoped_lock lock{recv_mtx};
  updateTimeout_(timeout);
  clear(message);

  std::optional<ReceiveResult> res;

  char sender_address[MAX_POSSIBLE_ADDRESS_SIZE];
  SocketAddressLength sender_address_length = MAX_POSSIBLE_ADDRESS_SIZE;

  int recvBytes =
      ::recvfrom(getHandler().accessId(), message.buffer,
                 static_cast<int>(message.buffer_size), 0,
                 reinterpret_cast<SocketAddress *>(&sender_address[0]),
                 &sender_address_length);
  if (checkResult(recvBytes, SCK_SOCKET_ERROR, "receive failed",
                  timeout != NULL_TIMEOUT) ||
      recvBytes > message.buffer_size) {
    return std::nullopt;
  }

  res = ReceiveResult{
      toAddress(reinterpret_cast<const SocketAddress &>(sender_address)),
      static_cast<std::size_t>(recvBytes)};

  return res;
}

std::optional<ReceiveResult>
ReceiverUnkownSenderNonBlocking::receive(BufferView message) {
  std::scoped_lock lock{recv_mtx};
  clear(message);

  std::optional<ReceiveResult> res;

  char sender_address[MAX_POSSIBLE_ADDRESS_SIZE];
  SocketAddressLength sender_address_length = MAX_POSSIBLE_ADDRESS_SIZE;

  int recvBytes =
      ::recvfrom(getHandler().accessId(), message.buffer,
                 static_cast<int>(message.buffer_size), 0,
                 reinterpret_cast<SocketAddress *>(&sender_address[0]),
                 &sender_address_length);
  if (checkResult(recvBytes, SCK_SOCKET_ERROR, "receive failed", true) ||
      recvBytes > message.buffer_size) {
    return std::nullopt;
  }

  res = ReceiveResult{
      toAddress(reinterpret_cast<const SocketAddress &>(sender_address)),
      static_cast<std::size_t>(recvBytes)};

  return res;
}

namespace {
template <typename Pred, typename... Args>
std::optional<ReceiveStringResult>
receive_unknown_into_string(std::size_t expected_max_bytes, Pred pred,
                            const Args &...args) {
  std::string buffer;
  buffer.resize(expected_max_bytes);
  auto buffer_temp = makeBufferView(buffer);
  auto result = pred(buffer_temp, args...);
  if (!result) {
    return std::nullopt;
  }
  buffer.resize(result->received_bytes);
  return ReceiveStringResult{std::move(result->sender), std::move(buffer)};
}
} // namespace

std::optional<ReceiveStringResult>
ReceiverUnkownSenderBlocking::receive(std::size_t expected_max_bytes,
                                      const Timeout &timeout) {
  return receive_unknown_into_string(
      expected_max_bytes,
      [this](BufferView message, const Timeout &timeout) {
        return this->receive(message, timeout);
      },
      timeout);
}

std::optional<ReceiveStringResult>
ReceiverUnkownSenderNonBlocking::receive(std::size_t expected_max_bytes) {
  return receive_unknown_into_string(
      expected_max_bytes,
      [this](BufferView message) { return this->receive(message); });
}

} // namespace MinimalSocket
