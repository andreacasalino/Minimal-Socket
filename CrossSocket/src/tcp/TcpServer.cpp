/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpServer.h>
#include <tcp/TcpClient.h>
#include "../Handler.h"

namespace sck::tcp {

   constexpr std::size_t LISTEN_BACKLOG = 50;

   class ClientHandler : public TcpClient {
   public:
      explicit ClientHandler(const sck::Address& remoteAddress, std::shared_ptr<Handler> channel)
         : TcpClient(remoteAddress, channel) {
      };

      ~ClientHandler() override = default;

   private:
      void openSpecific() final {
         throw std::runtime_error("ClientHandler from TcpServer are not re-openable!");
      };
   };

   TcpServer::TcpServer(const std::uint16_t& port, const Family& family) 
      : SocketConcrete(std::make_shared<Handler>())
      , port(port)
      , protocol(family) {
   }

   std::unique_ptr<SocketClient> TcpServer::acceptClient() {
      SocketAddress_t acceptedClientAddress;
#ifdef _WIN32
      int acceptedAddressLength
#else
      unsigned int acceptedAddressLength
#endif
         = sizeof(SocketAddress_t);
      // accept: wait for a client to call connect and hit this server and get a pointer to this client.
      Socket_t temp = ::accept(this->channel->getSocketId(), &acceptedClientAddress, &acceptedAddressLength);
      if (temp == SCK_INVALID_SOCKET) {
         throwWithCode("Error: accepting new client");
      }
      std::shared_ptr<Handler> acceptedClientHandler  = std::make_shared<Handler>(temp);
      
      AddressPtr remoteAddress = convert(acceptedClientAddress);
      if (nullptr != remoteAddress) {
         throw std::runtime_error("accepted client remote address is not resolvable");
      }

      return std::make_unique<ClientHandler>(*remoteAddress, acceptedClientHandler);
   }

   void TcpServer::openSpecific() {
      this->bindToPort(this->port);
      // listen to the port to allow all the following clients acceptance
      if (::listen(this->channel->getSocketId(), LISTEN_BACKLOG) == SCK_SOCKET_ERROR) {
         throwWithCode("Error: listening on port: " + std::to_string(this->port));
      }
   }
}