/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CHANNEL_H_
#define _CROSS_SOCKET_CHANNEL_H_

#include "Commons.h"
#include <core/components/ProtocolAware.h>

namespace sck {
    /**
     * An object storing a socket API handler and containing the minimal
     * functionalities for interacting with it.
     */
    class Channel {
    public:
        Channel(const Channel&) = delete;
        Channel& operator=(const Channel&) = delete;

        /**
         * @brief a closed socket is created
         */
        Channel();

        /**
         * @brief the passed handler should be already created externally
         * by the socket api
         */
        explicit Channel(const SocketHandler& hndl);

        ~Channel();

        /**
         * @brief internally creates a new socket
         */
        void open(const Protocol& type, const Family& family);
        /**
         * @brief close and shutdown the current socket
         */
        void close();

        inline bool isOpen() const { return this->opened; };

        inline const SocketHandler& operator*() const { return this->hndl; };

    private:
        std::atomic_bool opened;
        SocketHandler hndl;

#ifdef _WIN32
        class SocketHandlerFactory {
        public:
            /**
             * @brief If we are about to open the first socket, WSAStartup() is invoked
             */
            static void beforeOpen();
            /**
             * @brief If we are closing the last socket, WSACleanup() is invoked
             */
            static void afterClose();

        private:
            static std::mutex handlerCounterMtx;
            static std::size_t handlerCounter;
        };
#endif
    };

}

#endif