/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/SocketContext.h>

namespace MinimalSocket {
RemoteAddressAware::RemoteAddressAware(const Address &address)
    : remote_address(address) {
  if (nullptr == getRemoteAddress()) {
    throw Error{"Invalid address"};
  }
};
} // namespace MinimalSocket
