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
struct BufferView {
  char *buffer;
  std::size_t buffer_size;
};

/**
 * @brief sets all values inside the passed buffer to 0
 */
void clear(BufferView &subject);

/**
 * @param subject the string to convert
 * @return a buffer pointing to the first element of the subject, and a lenght
 * equal to the current size of subject
 */
BufferView makeBufferView(std::string &subject);

struct BufferViewConst {
  const char *buffer;
  std::size_t buffer_size;
};

/**
 * @param subject the string buffer to convert
 * @return an immutable buffer pointing to the first element of the subject, and
 * a lenght equal to the current size of subject
 */
BufferViewConst makeBufferViewConst(const std::string &subject);

enum class SocketType { UDP, TCP };

using Timeout = std::chrono::milliseconds;

/**
 * @brief actually, a null timeout is associated to a possibly infinite wait
 */
static constexpr Timeout NULL_TIMEOUT = Timeout{0};
} // namespace MinimalSocket
