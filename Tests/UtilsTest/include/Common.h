/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_COMMON_H
#define SAMPLE_COMMON_H

#include <core/BindCapable.h>
#include <core/components/Openable.h>
#include <core/components/Closable.h>
#include <gtest/gtest.h>

namespace sck::sample {
    class PortFactory {
    public:
        static std::uint16_t makePort();
    };

    template <typename T>
    void open(T& openable) {
        openable.open(std::chrono::milliseconds(0));
        EXPECT_TRUE(openable.isOpen());
    }

    template <typename T>
    void close(T& closable) {
        closable.close();
        EXPECT_FALSE(closable.isOpen());
    };
}

#endif