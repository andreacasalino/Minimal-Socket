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
#include <vector>

namespace sck::async {
   /**
    * @brief An asynchronous client can be any kind of socket that is a MessangerConcrete, that keeps receive messages
    * inside a private thread stored by this class. From the outside it is possible to send messages to the remote host
    * or subscribe to the received messages by setting a MessageListener (calling AsyncDecorator::resetListener(...))
    */
    class AsyncMessanger 
        : public AsyncSocket
        , public SendCapable
        , public Talker<MessangerListener> {
    public:
        AsyncMessanger(std::unique_ptr<Socket> messanger, const std::size_t& bufferCapacity);

        bool send(const std::pair<const char*, std::size_t>& message) final;

    private:
        void serviceIteration() override;

        std::vector<char> receiveBuffer;

        ReceiveCapable* recvPtr;
        //SendCapable* sendPtr;
    };
}

#endif