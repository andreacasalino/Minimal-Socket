/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCMESSANGER_H
#define _CROSS_SOCKET_ASYNCMESSANGER_H

#include <AsyncSocket.h>
#include <messanger/MessangerListener.h>
#include <core/components/SendCapable.h>
#include <vector>

namespace sck::async {
   /**
    * @brief An asynchronous client can be any kind of socket that is a MessangerConcrete, that keeps receive messages
    * inside a private thread stored by this class. From the outside it is possible to send messages to the remote host
    * or subscribe to the received messages by setting a MessageListener (calling AsyncDecorator::resetListener(...))
    */
    class AsyncClient 
        : public AsyncSocket<MessangerListener>
        , public SendCapable {
    public:
        AsyncClient(std::unique_ptr<SocketOpenable> client, const std::size_t& bufferCapacity);

        inline bool send(const std::pair<const char*, std::size_t>& message) final {
            return dynamic_cast<Sender*>(this->wrapped.get())->send(message);
        };

    private:
        std::vector<char> buffer;
        class ReceiveService;

        std::unique_ptr<Service> make_service() final;
    };
}

#endif