#ifndef _CROSS_SOCKET_TYPEDCLIENT_H_
#define _CROSS_SOCKET_TYPEDCLIENT_H_

#include "SocketClient.h"

namespace sck {
   template<typename IncomingMessage, typename OutgoingMessage>
   class TypedClient {
   public:
        TypedClient(const TypedClient&) = delete;
        TypedClient& operator=(const TypedClient&) = delete;

        inline void open(){ this->client->open(); };

        inline void close(){ this->client->close(); };

        inline bool isConnected() const { return this->client->isConnected(); }

      virtual bool send(const OutgoingMessage& mex) = 0;

      // incoming message might be corrucpted and a nullptr is returned
      std::unique_ptr<IncomingMessage> receive(const std::size_t& expectedMaxSize, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(0)){
          this->buffer.resize(expectedMaxSize);
          std::size_t recvBytes = this->client->receive(this->buffer.data(), expectedMaxSize);
          if(recvBytes != this->buffer.size()){
            this->buffer.resize(recvBytes);
          }
          std::unique_ptr<IncomingMessage> mex = std::make_unique<IncomingMessage>();
          if(!this->decode(*mex.get())){
              mex.reset();
          }
          return mex;
      };
   protected:
      explicit TypedClient(std::unique_ptr<SocketClient> client) 
        : client(std::move(client)) {
            if(nullptr == this->client){
                throw std::runtime_error("client cannot be empty");
            }
      };

      virtual bool decode(IncomingMessage& toParse) = 0;

      std::unique_ptr<SocketClient> client;

      std::vector<char> buffer;
   };
}

#endif