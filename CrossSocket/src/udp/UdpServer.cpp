/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <udp/UdpServer.h>
#include "../SocketHandler.h"

namespace sck::udp {

   sck::Address getInitialAddress(const sck::Family& protocol) {
      if (sck::Family::IP_V6 == protocol) {
         return sck::Address::Localhost(0, sck::Family::IP_V6);
      }
      return sck::Address::Localhost(0, sck::Family::IP_V4);
   }

   UdpServer::UdpServer(const std::uint16_t& localPort, const sck::Family& protocol)
      : UdpClient(getInitialAddress(protocol), localPort) {
   }

   void UdpServer::openConnection() {
      this->bindToPort(this->port);

      // receive a message from the client, that from now on will beccome the recognized one.
      char bf[MAX_UDP_RECV_MESSAGE];
      SocketAddress_t remoteAddr;
#ifdef _WIN32
      int
#else
      unsigned int
#endif
      remoteAddrLen = sizeof(SocketAddress_t);
      if (::recvfrom(this->channel->handle, &bf, MAX_UDP_RECV_MESSAGE, 0, &remoteAddr, &remoteAddrLen) == SCK_SOCKET_ERROR) {
         throwWithCode("recvfrom failed while identifying the target");
      }
      this->remoteAddress = convert(remoteAddr);
      if (!this->remoteAddress.isValid()) {
         throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid address parsed for the target");
      }
      this->SocketClient::openConnection();
   }
}