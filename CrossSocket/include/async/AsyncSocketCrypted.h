/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCSOCKETCRYPTED_H_
#define _CROSS_SOCKET_ASYNCSOCKETCRYPTED_H_

#include <MessangerCrypted.h>
#include <async/AsyncClient.h>

namespace sck {
    template<typename MessangerCrypt_t, typename Send_t, typename Recv_t>
    class SocketCrypted
        : public SocketDecorator
        , public Messanger<Send_t, Recv_t> {
        static_assert(std::is_base_of<MessangerCrypted<Send_t, Recv_t>, MessangerCrypt_t>::value, "invalid MessangerCrypt_t");
    public:
        SocketCrypted(std::unique_ptr<AsyncClient> socket) 
            : SocketDecorator(std::move(socket))
            , coder(*this->wrapped) {
        };

        inline void resetListener(CryptedMessageListener<Recv_t>* listener) {
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            this->listener = listener;
        };

    private:
        class CoderListener
            : public MessangerCrypt_t
            , public MessageListener {
        public:
            CoderListener(SocketCrypted& user) 
                : MessangerCrypt_t(*user.wrapped)
                , user(user) {
                dynamic_cast<AsyncClient*>(this->user.wrapped.get())->resetListener(this);
            };

        private:
            void handle(const std::pair<const char*, std::size_t>& message) final {
                std::lock_guard<std::mutex> lk(this->listenerMtx);
                if(nullptr != user.listener) {
                    Recv_t decoded;
                    this->decode(decoded, ... usare message buffer );
                    this->user.listener->handle(decoded);
                }
            };

            SocketCrypted& user;
        };

        MessangerCrypt_t coder;

        inline bool send(const Send_t& message) final {
            return this->coder.send(message);
        };

        std::size_t receive(Recv_t& message, const std::chrono::milliseconds& timeout) final {
            return this->coder.receive(message, timeout);
        };

        std::mutex listenerMtx;
        CryptedMessageListener<Recv_t>* listener = nullptr;
    };
}

#endif