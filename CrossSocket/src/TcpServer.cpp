#include "../include/TcpServer.h"
#include "SocketHandler.h"
#include "../include/TcpClient.h"

constexpr std::size_t listenBacklog = 50;

namespace sck {

   class TcpServerClient : public TcpClient {
   public:
      explicit TcpServerClient(const sck::Address& remoteAddress, std::unique_ptr<SocketHandler> channel)
         : TcpClient(remoteAddress, std::move(channel)) {
            this->connected = true;
      }

      ~TcpServerClient() final = default;
   private:
      void openConnection() final {
         throw std::runtime_error("TcpClients from TcpServer not reopenable!");
      }
   };

   std::unique_ptr<SocketClient> TcpServer::acceptNewClient() {
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
      sck::Address remoteAddress = convert(acceptedClientAddress);
      if (!remoteAddress.isValid()) {
         throw std::runtime_error("accepted client remote address is not resolvable");
      }

      return std::make_unique<TcpServerClient>(remoteAddress, std::move(acceptedClientHandler));
   }

   void TcpServer::doListen() {
      // listen to the port to allow all the following clients acceptance
      if (::listen(this->channel->handle, ::listenBacklog) == SCK_SOCKET_ERROR) {
         throwWithCode("Error: listening on port: " + std::to_string(this->getPort()));
      }
   }

   void TcpServer::initHandle() {
      this->channel->handle = ::socket(castFamily(this->SocketServer::getFamily()), SOCK_STREAM, 0);
      if (this->channel->handle == SCK_INVALID_SOCKET) {
         throwWithCode("Stream socket could not be created");
      }
   }

   void TcpServer::openConnection() {
      this->bindToPort(this->getPort());
      this->doListen();
   }

}