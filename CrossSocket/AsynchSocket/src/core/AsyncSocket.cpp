/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/AsyncSocket.h>

namespace sck::async {
    AsyncSocket::AsyncSocket(std::unique_ptr<Socket> socket)
        : SocketDecorator(std::move(socket)) {
        if (this->wrapped->isOpen()) {
            this->spawnService();
        }
    };

    void AsyncSocket::spawnService() {
        this->serviceLife = false;
        this->service = std::make_unique<std::thread>([this]() {
            this->serviceLife = true;
            while (this->serviceLife) {
                try {
                    this->serviceIteration();
                }
                catch (...) {
                    try {
                        std::rethrow_exception(std::current_exception());
                    }
                    catch (const Error & e) {
                        this->Talker<ErrorListener>::notify(e);
                    }
                    catch (const std::exception & e) {
                        this->Talker<ErrorListener>::notify(e);
                    }
                }
            }
        });
        // wait till the thread is actually spawned
        while (!this->serviceLife) {
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
    }

    void AsyncSocket::open(const std::chrono::milliseconds& timeout) {
        if (nullptr != this->service) {
            throw Error("The socket was already opened");
        }
        this->SocketDecorator::open(timeout);
        if (this->wrapped->isOpen()) {
            this->spawnService();
        }
    };

    void AsyncSocket::close() {
        if (nullptr == this->service) {
            return;
        }
        this->serviceLife = false;
        this->wrapped->close();
        if (this->service->joinable()) {
            this->service->join();
        }
        this->service.reset();
    };
}
