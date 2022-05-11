/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Receiver.h>

#include "../SocketAddress.h"
#include "../SocketError.h"

namespace MinimalSocket {
std::unique_ptr<std::scoped_lock<std::mutex>>
ReceiverBase::lazyUpdateReceiveTimeout(const Timeout &timeout) {
  std::unique_ptr<std::scoped_lock<std::mutex>> lock =
      std::make_unique<std::scoped_lock<std::mutex>>(receive_mtx);
  if (timeout == receive_timeout) {
    return lock;
  }
  receive_timeout = timeout;
  // set new timeout
#ifdef _WIN32
  auto tv = DWORD(this->receive_timeout.count());
  if (setsockopt(getIDWrapper().accessId(), SOL_SOCKET, SO_RCVTIMEO,
                 static_cast<const void *>(&tv),
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
  if (::setsockopt(getIDWrapper().accessId(), SOL_SOCKET, SO_RCVTIMEO,
                   static_cast<const void *>(&tv),
                   sizeof(struct timeval)) < 0) {
#endif
    throw SocketError{"can't set timeout"};
  }
  return lock;
}

namespace {
void check_received_bytes(int &recvBytes, const Timeout &timeout) {
  if (recvBytes != SCK_SOCKET_ERROR) {
    return;
  }
  SocketError error_with_code("receive failed");
  recvBytes = 0;
  if ((error_with_code.getErrorCode() == EAGAIN) && (timeout != NULL_TIMEOUT)) {
    return;
  }
  throw error_with_code;
}
} // namespace

std::size_t Receiver::receive(Buffer &message, const Timeout &timeout) {
  auto lock = lazyUpdateReceiveTimeout(timeout);
  clear(message);
  int recvBytes = ::recv(getIDWrapper().accessId(), message.buffer,
                         static_cast<int>(message.buffer_size), 0);
  check_received_bytes(recvBytes, timeout);
  if (recvBytes > message.buffer_size) {
    // if here, the message received is probably corrupted
    recvBytes = 0;
  }
  return static_cast<std::size_t>(recvBytes);
}

std::string Receiver::receive(std::size_t expected_max_bytes,
                              const Timeout &timeout) {
  std::string buffer;
  buffer.resize(expected_max_bytes);
  auto buffer_temp = makeStringBuffer(buffer);
  auto recvBytes = receive(buffer_temp, timeout);
  buffer.resize(recvBytes);
  return buffer;
}

std::optional<ReceiverUnkownSender::ReceiveResult>
ReceiverUnkownSender::receive(Buffer &message, const Timeout &timeout) {
  auto lock = lazyUpdateReceiveTimeout(timeout);
  clear(message);

  char sender_address[MAX_POSSIBLE_ADDRESS_SIZE];
  SocketAddressLength sender_address_length = MAX_POSSIBLE_ADDRESS_SIZE;

  int recvBytes =
      ::recvfrom(getIDWrapper().accessId(), message.buffer,
                 static_cast<int>(message.buffer_size), 0,
                 reinterpret_cast<SocketAddress *>(&sender_address[0]),
                 &sender_address_length);
  check_received_bytes(recvBytes, timeout);
  if (recvBytes > message.buffer_size) {
    // if here, the message received is probably corrupted
    return std::nullopt;
  }
  return ReceiveResult{
      toAddress(reinterpret_cast<const SocketAddress &>(sender_address)),
      static_cast<std::size_t>(recvBytes)};
}

std::optional<ReceiverUnkownSender::ReceiveStringResult>
ReceiverUnkownSender::receive(std::size_t expected_max_bytes,
                              const Timeout &timeout) {
  std::string buffer;
  buffer.resize(expected_max_bytes);
  auto buffer_temp = makeStringBuffer(buffer);
  auto result = receive(buffer_temp, timeout);
  if (!result) {
    return std::nullopt;
  }
  buffer.resize(buffer_temp.buffer_size);
  return ReceiveStringResult{result->sender, std::move(buffer)};
}
} // namespace MinimalSocket
