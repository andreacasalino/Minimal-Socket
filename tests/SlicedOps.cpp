#include "SlicedOps.h"

namespace MinimalSocket::test {
MovingPointerBuffer::MovingPointerBuffer(const std::string &buff)
    : buffer(buff) {
  init();
}

MovingPointerBuffer::MovingPointerBuffer(const std::size_t buff_size) {
  buffer.resize(buff_size);
  init();
}

std::size_t MovingPointerBuffer::remainingBytes() const {
  return buffer.size() - buffer_cursor;
}

void MovingPointerBuffer::shift(const std::size_t stride) {
  buffer_cursor += stride;
  buffer_pointer += stride;
}

void MovingPointerBuffer::init() {
  buffer_cursor = 0;
  buffer_pointer = buffer.data();
}

void sliced_send(Sender &subject, const std::string &to_send,
                 const std::size_t delta_send) {
  MovingPointerBuffer buffer(to_send);
  while (buffer.remainingBytes() != 0) {
    std::size_t bytes_to_send =
        std::min<std::size_t>(delta_send, buffer.remainingBytes());
    subject.send(ConstBuffer{buffer.data(), bytes_to_send});
    buffer.shift(bytes_to_send);
  }
}

void sliced_send(SenderTo &subject, const std::string &to_send,
                 const Address &to_send_address, const std::size_t delta_send) {
  MovingPointerBuffer buffer(to_send);
  while (buffer.remainingBytes() != 0) {
    std::size_t bytes_to_send =
        std::min<std::size_t>(delta_send, buffer.remainingBytes());
    subject.sendTo(ConstBuffer{buffer.data(), bytes_to_send}, to_send_address);
    buffer.shift(bytes_to_send);
  }
}

std::string sliced_receive(Receiver &subject, const std::size_t to_receive,
                           const std::size_t delta_receive) {
  MovingPointerBuffer buffer(to_receive);
  while (buffer.remainingBytes() != 0) {
    std::size_t bytes_to_receive =
        std::min<std::size_t>(delta_receive, buffer.remainingBytes());
    auto bytes_received =
        subject.receive(Buffer{buffer.data(), bytes_to_receive});
    buffer.shift(bytes_received);
  }
  return buffer.asString();
}

std::string sliced_receive(ReceiverUnkownSender &subject,
                           const std::size_t to_receive,
                           const std::size_t delta_receive) {
  MovingPointerBuffer buffer(to_receive);
  while (buffer.remainingBytes() != 0) {
    std::size_t bytes_to_receive =
        std::min<std::size_t>(delta_receive, buffer.remainingBytes());
    auto maybe_bytes_received =
        subject.receive(Buffer{buffer.data(), bytes_to_receive});
    if (maybe_bytes_received) {
      buffer.shift(maybe_bytes_received->received_bytes);
    }
  }
  return buffer.asString();
}
} // namespace MinimalSocket::test
