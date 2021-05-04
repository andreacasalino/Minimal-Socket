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
    template<typename Listener>
    class Talker {
    public:
        void resetListener(std::shared_ptr<Listener> listener) {
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
            this->listener->handle(content);
        };

    private:
        std::mutex listenerMtx;
        Listener* listener = nullptr;
    };
}

#endif
