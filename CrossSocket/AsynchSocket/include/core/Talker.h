/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TALKER_H_
#define _CROSS_SOCKET_TALKER_H_

#include <mutex>
#include <Error.h>

namespace sck::async {
    /**
     * @brief An object able to notify to an external listener.
     * After construction, no listener is assumed, meaning that calling notify will
     * actually have no effect.
     */
    template<typename Listener>
    class Talker {
    public:    
        /**
         * @brief Sets the listener that should receive the notifications
         * @param the new listener to set. Only 1 listener at a time can be supported
         */
        void resetListener(Listener* listener) {
            if (nullptr == listener) {
                throw Error("The passed listener is empty");
            }
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            this->listener = listener;
        };

    protected:
        Talker() = default;

        template<typename T>
        void notify(T content) {
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            if (nullptr == this->listener) {
                return;
            }
            this->listener->handle(std::forward<T>(content));
        };

    private:
        std::mutex listenerMtx;
        Listener* listener = nullptr;
    };
}

#endif
