#pragma once

#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>

#include <string>
#include <string_view>

namespace MinimalSocket::test {
class RollingView {
public:
  RollingView(const std::string &buff);
  RollingView(std::size_t buff_size);

  template <typename Pred>
  void forEachView(std::size_t offset, Pred pred) const {
    std::size_t view_begin = 0;
    std::size_t view_end = std::min<std::size_t>(offset, buffer.size());
    while (true) {
      std::size_t processed = pred(
          std::string_view{buffer.data() + view_begin, view_end - view_begin});
      if (processed != offset) {
        throw std::runtime_error{"Wrong number of bytes were processed"};
      }
      if (view_end == buffer.size()) {
        break;
      }
      view_begin = view_end;
      view_end += offset;
      view_end = std::min<std::size_t>(view_end, buffer.size());
    }
  }

  const auto &getBuffer() const { return buffer; }

private:
  std::string buffer;
};

void sliced_send(Sender &subject, const std::string &to_send,
                 std::size_t delta_send);

void sliced_send(SenderTo &subject, const std::string &to_send,
                 const Address &to_send_address, std::size_t delta_send);

std::string sliced_receive(Receiver<true> &subject, std::size_t to_receive,
                           std::size_t delta_receive);

std::string sliced_receive(ReceiverUnkownSender<true> &subject,
                           std::size_t to_receive, std::size_t delta_receive);
} // namespace MinimalSocket::test
