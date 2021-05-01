/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_OPENDECORATOR_H_
#define _CROSS_SOCKET_OPENDECORATOR_H_

#include <core/components/OpenCapable.h>
#include <memory>

namespace sck {
    class OpenDecorator : public OpenCapable {
    public:
        inline void open(const std::chrono::milliseconds& timeout) override { this->wrapped->open(timeout); };

        inline void close() override { this->wrapped->close(); };

        inline bool isOpen() const override { return this->wrapped->isOpen(); };

    protected:
        explicit OpenDecorator(std::unique_ptr<OpenCapable> wrapped);

        std::unique_ptr<OpenCapable> wrapped;
    };
}

#endif