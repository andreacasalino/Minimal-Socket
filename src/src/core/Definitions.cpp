/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Definitions.h>

#include "../SocketHandler.h"

namespace MinimalSocket {
void clear(BufferView &subject) {
  ::memset(subject.buffer, 0, subject.buffer_size);
}

BufferView makeBufferView(std::string &subject) {
  return BufferView{subject.data(), subject.size()};
}

BufferViewConst makeBufferViewConst(const std::string &subject) {
  return BufferViewConst{subject.data(), subject.size()};
}
} // namespace MinimalSocket