/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGERDECORATOR_H_
#define _CROSS_SOCKET_MESSANGERDECORATOR_H_

#include <MessangerConcrete.h>
#include <vector>

namespace sck {   
   /**    
    * @brief The interface providing functionalities for exchanging data
    */
   template<typename Send_t, typename Recv_t>
   class MessangerDecorator {
   private:
       bool send(const Send_t& message) final {
           this->encode(message);
           this->messanger->send({this->sendBuffer.data(), this->sendBuffer.size()});
       };

       std::size_t receive(Recv_t& message, const std::chrono::milliseconds& timeout) final {
           auto recvBytes = this->messanger->receive({this->recvBuffer.data(), this->recvBuffer.capacity()}, timeout);
           if(recvBytes != this->recvBuffer.capacity()) {
               this->recvBuffer.resize(recvBytes);
           }
           this->decode(message);
           return recvBytes;
       };
      
    protected:
        MessangerDecorator(MessangerConcrete& mess, const std::size_t recvCapacity) {
            this->messanger = &mess;
            this->recvBuffer.resize(recvCapacity);
        };
        MessangerConcrete* messanger;

        /**
         * @brief write the message into sendBuffer, as a series of raw bytes
         */
        virtual encode(const Send_t& message) = 0;
        std::vector<char> sendBuffer;

        /**
         * @brief convert the content of recvBuffer as a received message
         */
        virtual decode(Recv_t& message) = 0;
        std::vector<char> recvBuffer;
   };
}

#endif