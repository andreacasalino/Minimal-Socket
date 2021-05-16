/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_BUFFER_H_
#define _CROSS_SOCKET_BUFFER_H_

#include <string>
#include <utility>

namespace sck {
    class Buffer {
    protected:
        Buffer(const std::size_t capacity);

        inline std::pair<char*, std::size_t> getBuffer() { return std::make_pair(this->buffer.data(), this->buffer.size()); };

        inline const std::string& getStringBuffer() const { return this->buffer; };

        // with 0 reset to size equal to capacity
        // throw if the new size is higher than the max capacity
        void resetBufferSize(const std::size_t newSize = 0);

        void resetBufferCapacity(const std::size_t newCapacity);

    private:
        std::string buffer;
        std::size_t bufferCapacity;
    };
}

#endif
