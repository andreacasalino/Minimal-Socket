#include "../include/UdpServer.h"
#include "SocketHandler.h"

namespace sck {

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

      //listen for a client, launching a receive of 1 byte (no filter will be applied since the socket is opening when arriving here and establishConnection was not already called)
      char bf;
      SocketAddress_t remoteAddr;
#ifdef _WIN32
      int
#else
      unsigned int
#endif
      remoteAddrLen = sizeof(SocketAddress_t);
      if (::recvfrom(this->channel->handle, &bf, 1, 0, &remoteAddr, &remoteAddrLen) == SCK_SOCKET_ERROR) {
         throwWithCode("recvfrom failed while identifying the target");
      }
      this->remoteAddress = convert(remoteAddr);
      if (!this->remoteAddress.isValid()) {
         throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid address parsed for the target");
      }
      this->SocketClient::openConnection();
   }
}