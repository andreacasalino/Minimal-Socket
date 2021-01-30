/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <async/Service.h>

namespace sck::async {
    void Service::Barrier::wait() {
        std::unique_lock<std::mutex> lk(this->queueMtx);
        ++this->queue;
        if(2 == this->queue) {
            this->stopWait = true;
            this->notification.notify_all();
        }
        else {
            this->notification.wait(lk, [this](){ return static_cast<bool>(this->stopWait); });
        }
    }

    Service::Service(const std::function<void()>& iterativeAction)
        : listener(nullptr) 
        , loop([this, &iterativeAction](){
            std::function<void()> iter(iterativeAction);
            this->barrier.wait();
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
        this->loopLife = true;
        this->barrier.wait();
    }

    Service::~Service() {
        this->loopLife = false;
        this->loop.join();
    }

    void Service::resetErrorListener(ErrorListener* listener) {
        std::lock_guard<std::mutex> lk(this->listenerMtx);
        this->listener = listener;
    }
}