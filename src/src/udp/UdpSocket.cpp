/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/udp/UdpSocket.h>

#include "../Commons.h"

namespace MinimalSocket::udp {
UdpSender::UdpSender() { getIDWrapper().reset(UDP, ); }

UdpBindable UdpSender::bind(const Port &port) {
  UdpBindable result(port);
  Socket::transferIDWrapper(*this, result);
  result.open();
  return std::move(result);
}

UdpSender::UdpSender(UdpSender &&o) { Socket::transferIDWrapper(o, *this); }
UdpSender &UdpSender::operator=(UdpSender &&o) {
  Socket::transferIDWrapper(o, *this);
}

} // namespace MinimalSocket::udp
