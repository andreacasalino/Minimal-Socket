/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "IsTimeout.h"

namespace MinimalSocket::test {
bool is_timeout(const Error *subject) {
  return dynamic_cast<const TimeoutError *>(subject) != nullptr;
}
} // namespace MinimalSocket::test
