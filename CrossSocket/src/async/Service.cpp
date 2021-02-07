/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <async/Service.h>

namespace sck::async {
    Service::Service(const std::function<void()>& iterativeAction)
        : listener(nullptr) 
        , loop([this, &iterativeAction](){
            std::function<void()> iter(iterativeAction);
            this->loopLife = true;
            while (this->loopLife) {
                try {
                    iter();
                }
                catch(...) {
                    std::lock_guard<std::mutex> lk(this->listenerMtx);
                    if(nullptr != this->listener) {
                        try {
                            std::rethrow_exception(std::current_exception());
                        }
                        catch(const Error& e) {
                            this->listener->handle(e);
                        }
                        catch(const std::exception& e) {
                            this->listener->handle(e);
                        }
                    }
                }
            }
        }) {
        while (!this->loopLife) {
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
    }

    Service::~Service() {
        this->loopLife = false;
        this->loop.join();
    }

    void Service::resetErrorListener(listener::ErrorListener* listener) {
        std::lock_guard<std::mutex> lk(this->listenerMtx);
        this->listener = listener;
    }
}