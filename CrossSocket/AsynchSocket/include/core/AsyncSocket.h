/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCSOCKET_H
#define _CROSS_SOCKET_ASYNCSOCKET_H

#include <core/SocketDecorator.h>
#include <core/ErrorListener.h>
#include <core/Talker.h>
#include <atomic>
#include <thread>

namespace sck::async {
   typedef Talker<ErrorListener> ErrorTalker;

    /**
     * @brief An asynchronous socket internally stores a service, which is a private thread using in some way the wrapped socket.
     * The internal service is created and spawned everytime the object is opened and stop and destroyed everytime is closed.
     */
    class AsyncSocket
        : public SocketDecorator
        , public ErrorTalker {
    public:
        virtual ~AsyncSocket() { this->close(); };

        /**
         * @return true only when the internal service was already spawned.
         * IMPORTANT!! the object might be built passing an already opened socket. However the 
         * service is not spawned inside the c'tor and calling this function after construction will return false, 
         * even if the passed socket was opened. It is however possible to call open to actually start the service.
         */
        inline bool isOpen() const override { return this->serviceLife; };

        /**
         * @brief tries to open the wrapped socket if not already opened and then spawn the internal service.
         * @throw if the service was already spawned.
         */
        void open(const std::chrono::milliseconds& timeout) final;

        void close() final;
        
    protected:
        AsyncSocket(std::unique_ptr<Socket> socket);

        virtual void serviceIteration() = 0;

    private:
        void spawnService();

        std::atomic_bool serviceLife = false;
        std::unique_ptr<std::thread> service;
    };
}

#endif
