/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <chrono>
#include <string>

namespace MinimalSocket {
struct Buffer {
  char *buffer;
  const std::size_t buffer_size;
};
void clear(Buffer &subject);
Buffer makeStringBuffer(std::string &subject);

struct ConstBuffer {
  const char *buffer;
  const std::size_t buffer_size;
};
ConstBuffer makeStringConstBuffer(const std::string &subject);

enum SocketType { UDP, TCP };

using Timeout = std::chrono::milliseconds;

static constexpr Timeout NULL_TIMEOUT = Timeout{0};
} // namespace MinimalSocket
