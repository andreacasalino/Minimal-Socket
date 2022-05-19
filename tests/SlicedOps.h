#pragma once

#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>

namespace MinimalSocket::test {
class MovingPointerBuffer {
public:
  MovingPointerBuffer(const std::string &buff);
  MovingPointerBuffer(const std::size_t buff_size);

  const std::string &asString() const { return buffer; }
  char *data() { return buffer_pointer; }
  const char *data() const { return buffer_pointer; }

  std::size_t remainingBytes() const;
  void shift(const std::size_t stride);

private:
  void init();

  std::string buffer;
  std::size_t buffer_cursor;
  char *buffer_pointer;
};

void sliced_send(Sender &subject, const std::string &to_send,
                 const std::size_t delta_send);

void sliced_send(SenderTo &subject, const std::string &to_send,
                 const Address &to_send_address, const std::size_t delta_send);

std::string sliced_receive(Receiver &subject, const std::size_t to_receive,
                           const std::size_t delta_receive);

std::string sliced_receive(ReceiverUnkownSender &subject,
                           const std::size_t to_receive,
                           const std::size_t delta_receive);
} // namespace MinimalSocket::test
