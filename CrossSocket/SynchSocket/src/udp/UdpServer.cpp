/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <udp/UdpServer.h>
#include "../Channel.h"
#include <Error.h>

namespace sck::udp {

   sck::Ip getInitialAddress(const sck::Family& family) {
      if (sck::Family::IP_V4 == family) {
          return *sck::Ip::createLocalHost(0, sck::Family::IP_V4);
      }
      if (sck::Family::IP_V6 == family) {
         return *sck::Ip::createLocalHost(0, sck::Family::IP_V6);
      }
      throw Error("unrecognized family");
   }

   UdpServer::UdpServer(const std::uint16_t& localPort, const sck::Family& protocol)
      : UdpConnection(getInitialAddress(protocol), localPort) {
   }

   void UdpServer::openSteps() {
      this->bindToPort(this->port);

      // receive a message from the client, that from now on will become the recognized one.
      char bf[MAX_UDP_RECV_MESSAGE];
      SocketIp remoteAddr;
#ifdef _WIN32
      int
#else
      unsigned int
#endif
      remoteAddrLen = sizeof(SocketIp);
      if (::recvfrom(**this->channel, &bf[0], static_cast<int>(MAX_UDP_RECV_MESSAGE), 0, &remoteAddr, &remoteAddrLen) == SCK_SOCKET_ERROR) {
         throwWithCode("recvfrom failed while identifying the target");
      }
      IpPtr remoteConverted = convert(remoteAddr);
      if(nullptr == remoteConverted) {
         throw Error(remoteAddr.sa_data, " is an invalid data for udp serer remote address");
      }
      this->remoteAddress = std::move(remoteConverted);
      this->Connection::openSteps();
   }
}