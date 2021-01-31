/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETCRYPTED_H_
#define _CROSS_SOCKET_SOCKETCRYPTED_H_

#include <MessangerCrypted.h>
#include <SocketDecorator.h>
#include <SocketClient.h>

namespace sck {
    template<typename MessangerCrypt_t, typename Send_t, typename Recv_t>
    class SocketCrypted
        : public SocketDecorator
        , public Messanger<Send_t, Recv_t> {
        static_assert(std::is_base_of<MessangerCrypted<Send_t, Recv_t>, MessangerCrypt_t>::value, "invalid MessangerCrypt_t");
    public:
        SocketCrypted(std::unique_ptr<SocketClient> socket) 
            : SocketDecorator(std::move(socket))
            , coder(*this->wrapped) {
        };

    private:
        MessangerCrypt_t coder;

        inline bool send(const Send_t& message) final {
            return this->coder.send(message);
        };

        std::size_t receive(Recv_t& message, const std::chrono::milliseconds& timeout) final {
            return this->coder.receive(message, timeout);
        };
    };
}

#endif