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
#include <atomic>
#include <mutex>
#include <async/listener/ErrorListener.h>

namespace sck::async {
   /**
    * @brief A service store a thread that keep executes a specific function, i.e. the iterativeAction,
    * untill the object is destroyed from the outside
    */
    class Service {
    public:
        Service(const Service&) = delete;
        Service& operator=(const Service&) = delete;

        // service is started when building
        Service(const std::function<void()>& iterativeAction, listener::ErrorListener* list);
        // service is stop when destroying
        ~Service();

        void resetErrorListener(listener::ErrorListener* list);

    private:    
        std::mutex listenerMtx;
        listener::ErrorListener* listener;

        std::atomic_bool loopLife = false;
        std::thread loop;
    };
}

#endif