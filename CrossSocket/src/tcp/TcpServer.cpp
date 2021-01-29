/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpServer.h>
#include <tcp/TcpClient.h>
#include "../SocketHandler.h"

namespace sck::tcp {

   constexpr std::size_t LISTEN_BACKLOG = 50;

   class ClientHandler : public TcpClient {
   public:
      explicit ClientHandler(const sck::Address& remoteAddress, std::unique_ptr<SocketHandler> channel)
         : TcpClient(remoteAddress, std::move(channel)) {
            this->opened = true;
      };

      ~ClientHandler() override = default;

   private:
      void openConnection() final {
         throw std::runtime_error("ClientHandler from TcpServer are not re-openable!");
      };
   };

   TcpServer::TcpServer(const std::uint16_t& port, const Family& family) 
      : SocketServer(port, family) {
   }

   std::unique_ptr<SocketClient> TcpServer::acceptClient() {
      SocketAddress_t acceptedClientAddress;
      std::unique_ptr<SocketHandler> acceptedClientHandler  = std::make_unique<SocketHandler>();
#ifdef _WIN32
      int acceptedAddressLength
#else
      unsigned int acceptedAddressLength
#endif
         = sizeof(SocketAddress_t);
      // accept: wait for a client to call connect and hit this server and get a pointer to this client.
      acceptedClientHandler->handle = ::accept(this->channel->handle, &acceptedClientAddress, &acceptedAddressLength);
      if (acceptedClientHandler->handle == SCK_INVALID_SOCKET) {
         throwWithCode("Error: accepting new client");
      }
      AddressPtr remoteAddress = convert(acceptedClientAddress);
      if (nullptr != remoteAddress) {
         throw std::runtime_error("accepted client remote address is not resolvable");
      }

      return std::make_unique<ClientHandler>(*remoteAddress, std::move(acceptedClientHandler));
   }

   void TcpServer::initHandle() {
      this->channel->handle = ::socket(castFamily(this->SocketServer::getFamily()), SOCK_STREAM, 0);
      if (this->channel->handle == SCK_INVALID_SOCKET) {
         throwWithCode("Stream socket could not be created");
      }
   }

   void TcpServer::openConnection() {
      this->bindToPort(this->getPort());
      // listen to the port to allow all the following clients acceptance
      if (::listen(this->channel->handle, LISTEN_BACKLOG) == SCK_SOCKET_ERROR) {
         throwWithCode("Error: listening on port: " + std::to_string(this->getPort()));
      }
   }

}