/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDCLIENT_H_
#define _CROSS_SOCKET_TYPEDCLIENT_H_

#include "SocketClient.h"
#include <stdexcept>

namespace sck {
   /**
    * @brief A typed client exchanges typed message with another peer.
    * A typed connection consumes a SocketClient, which must be outside created
    * and captured in the c'tor.
    */
   template<typename IncomingMessage, typename OutgoingMessage>
   class TypedClient {
   public:
        TypedClient(const TypedClient&) = delete;
        TypedClient& operator=(const TypedClient&) = delete;

        inline void open(){ this->client->open(); };

        inline void close(){ this->client->close(); };

        inline bool isConnected() const { return this->client->isConnected(); }

     /**
      * @brief returns true if the message was successfully sent.
      * @param[in] the typed message to send
      */
      virtual bool send(const OutgoingMessage& mex) = 0;

     /**
      * @brief receive the required number of bytes, in order to later decode it
      * into a typed message. Since the operation does not succeed, an empty 
      * smart pointer is returned.
      * @param[in] the maximal size of the message to receive
      * @param[in] the timeout to consider for receiving the message
      */
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