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

} // namespace MinimalSocket::udp
