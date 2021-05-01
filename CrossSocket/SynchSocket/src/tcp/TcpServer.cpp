/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpServer.h>
#include <tcp/TcpClient.h>
#include "../Channel.h"
#include <Error.h>

namespace sck::tcp {
   constexpr std::size_t LISTEN_BACKLOG = 50;

   class ClientHandler : public TcpClient {
   public:
      explicit ClientHandler(const sck::Ip& remoteAddress, std::unique_ptr<Channel> channel)
         : TcpClient(remoteAddress, std::move(channel)) {
      };

      ~ClientHandler() override = default;

   private:
      void openSpecific() final {
         throw Error("connection returned from TcpServer::acceptClient are not re-openable");
      };
   };

   TcpServer::TcpServer(const std::uint16_t& port, const Family& family) 
      : OpenConcrete(std::make_unique<Channel>())
      , port(port)
      , family(family) {
   }

   std::unique_ptr<Client> TcpServer::acceptClient() {
       if (!this->isOpen()) {
           throw Error("a tcp server should be opened before accepting a new client");
       }
      SocketIp acceptedClientAddress;
#ifdef _WIN32
      int acceptedAddressLength
#else
      unsigned int acceptedAddressLength
#endif
         = sizeof(SocketIp);
      // accept: wait for a client to call connect and hit this server and get a pointer to this client.
      SocketHandler temp = ::accept(**this->channel, &acceptedClientAddress, &acceptedAddressLength);
      if (temp == SCK_INVALID_SOCKET) {
         throwWithCode("Error: accepting new client");
      }
      std::unique_ptr<Channel> acceptedClientHandler  = std::make_unique<Channel>(temp);
      
      IpPtr remoteAddress = convert(acceptedClientAddress);
      if (nullptr == remoteAddress) {
         throw Error("accepted client remote address is not resolvable");
      }

      return std::make_unique<ClientHandler>(*remoteAddress, std::move(acceptedClientHandler));
   }

   void TcpServer::openSpecific() {
      this->bindToPort(this->port);
      // listen to the port to allow all the following clients acceptance
      if (::listen(**this->channel, LISTEN_BACKLOG) == SCK_SOCKET_ERROR) {
         throwWithCode("Error: listening on port: " + std::to_string(this->port));
      }
   }
}