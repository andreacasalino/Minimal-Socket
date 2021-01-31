/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGERCRYPTED_H_
#define _CROSS_SOCKET_MESSANGERCRYPTED_H_

#include <MessangerConcrete.h>
#include <vector>

namespace sck {   
   template<typename Send_t, typename Recv_t>
   class MessangerCrypted
    : public Messanger<Send_t, Recv_t> {
   private:
       bool send(const Send_t& message) final {
           this->encode(message);
           return this->wrapped->send({this->sendBuffer.data(), this->sendBuffer.size()});
       };

       std::size_t receive(Recv_t& message, const std::chrono::milliseconds& timeout) final {
           auto pr = std::make_pair<char*, std::size_t>(this->recvBuffer.data(), this->recvBuffer.capacity());
           auto recvBytes = this->wrapped->receive(pr, timeout);
           if(recvBytes != this->recvBuffer.capacity()) {
               this->recvBuffer.resize(recvBytes);
           }
           this->decode(message);
           return recvBytes;
       };

    protected:
        MessangerDecorator(MessangerConcrete_t& wrapped, const std::size_t recvCapacity) {
            this->wrapped = &wrapped;
            this->recvBuffer.resize(recvCapacity);
        };
        MessangerConcrete_t* wrapped;

        /**
         * @brief write the message into sendBuffer, as a series of raw bytes
         */
        virtual void encode(const Send_t& message) = 0;
        std::vector<char> sendBuffer;

        /**
         * @brief convert the content of recvBuffer as a received message
         */
        virtual void decode(Recv_t& message) = 0;
        std::vector<char> recvBuffer;
   };
}

#endif