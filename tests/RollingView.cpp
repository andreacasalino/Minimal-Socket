#include "RollingView.h"

namespace MinimalSocket::test {
RollingView::RollingView(const std::string &buff) : buffer{buff} {}

RollingView::RollingView(std::size_t buff_size) { buffer.resize(buff_size); }

void sliced_send(Sender &subject, const std::string &to_send,
                 std::size_t delta_send) {
  RollingView buff{to_send};
  buff.forEachView(delta_send, [&](const std::string_view &view) {
    bool ok = subject.send(BufferViewConst{view.data(), view.size()});
    if (!ok) {
      throw std::runtime_error{"wasn't able to send all data"};
    }
    return delta_send;
  });
}

void sliced_send(SenderTo &subject, const std::string &to_send,
                 const Address &to_send_address, std::size_t delta_send) {
  RollingView buff{to_send};
  buff.forEachView(delta_send, [&](const std::string_view &view) {
    bool ok = subject.sendTo(BufferViewConst{view.data(), view.size()},
                             to_send_address);
    if (!ok) {
      throw std::runtime_error{"wasn't able to send all data"};
    }
    return delta_send;
  });
}

std::string sliced_receive(Receiver<true> &subject, std::size_t to_receive,
                           std::size_t delta_receive) {
  RollingView buff{to_receive};
  buff.forEachView(delta_receive, [&](const std::string_view &view) {
    BufferView buff{const_cast<char *>(view.data()), view.size()};
    return subject.receive(buff);
  });
  return buff.getBuffer();
}

std::string sliced_receive(ReceiverUnkownSender<true> &subject,
                           std::size_t to_receive, std::size_t delta_receive) {
  RollingView buff{to_receive};
  buff.forEachView(delta_receive, [&](const std::string_view &view) {
    BufferView buff{const_cast<char *>(view.data()), view.size()};
    auto maybe_bytes_received = subject.receive(buff);
    if (!maybe_bytes_received.has_value()) {
      throw std::runtime_error{"wasn'table tor receive the data"};
    }
    return maybe_bytes_received->received_bytes;
  });
  return buff.getBuffer();
}
} // namespace MinimalSocket::test
