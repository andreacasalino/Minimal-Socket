/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCMESSANGER_H
#define _CROSS_SOCKET_ASYNCMESSANGER_H

#include <core/AsyncSocket.h>
#include <messanger/MessangerListener.h>
#include <core/components/SendCapable.h>
#include <core/Messanger.h>
#include <vector>

namespace sck::async {
   typedef Talker<MessangerListener> MessageTalker;

   /**
    * @brief An asynchronous client can be any kind of socket that is a MessangerConcrete, that keeps receive messages
    * inside a private thread stored by this class. From the outside it is possible to send messages to the remote host
    * or subscribe to the received messages by setting a MessageListener (calling AsyncDecorator::resetListener(...))
    */
    class AsyncMessanger 
        : public AsyncSocket
        , public MessageTalker
        , public SendCapable {
    public:
        AsyncMessanger(std::unique_ptr<Socket> messanger, const std::size_t& bufferCapacity);

        inline bool send(const std::pair<const char*, std::size_t>& message) final { return this->messPtr->send(message); };

    private:
        void serviceIteration() final;

        std::vector<char> receiveBuffer;

    protected:
        Messanger* messPtr;
    };
}

#endif
