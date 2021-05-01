/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCCLIENT_H
#define _CROSS_SOCKET_ASYNCCLIENT_H

#include <AsyncDecorator.h>
#include <core/Client.h>
#include <client/MessageListener.h>
#include <vector>

namespace sck::async {
   /**
    * @brief An asynchronous client can be any kind of socket that is a MessangerConcrete, that keeps receive messages
    * inside a private thread stored by this class. From the outside it is possible to send messages to the remote host
    * or subscribe to the received messages by setting a MessageListener (calling AsyncDecorator::resetListener(...))
    */
    class AsyncClient 
        : public AsyncDecorator<MessageListener>
        , public SendCapable {
    public:
        AsyncClient(std::unique_ptr<Client> client, const std::size_t& bufferCapacity);

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