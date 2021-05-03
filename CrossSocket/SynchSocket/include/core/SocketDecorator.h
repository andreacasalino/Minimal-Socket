/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETDECORATOR_H_
#define _CROSS_SOCKET_SOCKETDECORATOR_H_

#include <core/Socket.h>

namespace sck {
    class SocketDecorator
        : public StateAware
        , public Openable
        , public Closable {
    public:
        inline bool isOpen() const final { return this->wrapped->isOpen(); };

        inline void close() override { this->wrapped->close(); };

        /**
         * @brief Tries to open the wrapped socket
         * @throw when the wrapped socket is not openable
         */
        void open(const std::chrono::milliseconds& timeout) override;

    protected:
        explicit SocketDecorator(std::unique_ptr<Socket> channel);

        std::unique_ptr<Socket> wrapped;
    };
}

#endif