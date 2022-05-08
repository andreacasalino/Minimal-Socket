/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>

#include "Utils.h"

namespace MinimalSocket {
void visitAddress(const AddressFamily &family,
                  const std::function<void()> &ipv4_case,
                  const std::function<void()> &ipv6_case) {
  switch (family) {
  case AddressFamily::IP_V4:
    ipv4_case();
    break;
  case AddressFamily::IP_V6:
    ipv6_case();
    break;
  default:
    throw Error{"Unrecognized AddressFamily"};
    break;
  }
}
} // namespace MinimalSocket
