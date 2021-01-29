/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SERVICE_H_
#define _CROSS_SOCKET_SERVICE_H_

#include <thread>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <async/ErrorListener.h>
#include <memory>

namespace sck::async {
    class Service {
    public:
        // start
        Service(const std::function<void()>& iterativeAction);
        // stop
        ~Service();

        void resetErrorListener(ErrorListener* listener);

    private:
        class Barrier;
        std::unique_ptr<Barrier> barrier = std::make_unique<Barrier>();

        ErrorListener* listener;
        std::mutex listenerMtx;

        std::atomic_bool loopLife;
        std::thread loop;
    };
}

#endif