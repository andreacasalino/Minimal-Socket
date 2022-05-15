/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Definitions.h>

#include "../SocketId.h"

namespace MinimalSocket {
void clear(const Buffer &subject) {
  ::memset(subject.buffer, 0, subject.buffer_size);
}

Buffer makeStringBuffer(std::string &subject) {
  return Buffer{subject.data(), subject.size()};
}

ConstBuffer makeStringConstBuffer(const std::string &subject) {
  return ConstBuffer{subject.data(), subject.size()};
}
} // namespace MinimalSocket