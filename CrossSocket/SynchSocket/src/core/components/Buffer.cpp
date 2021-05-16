/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/components/Buffer.h>
#include <Error.h>

namespace sck {
    Buffer::Buffer(const std::size_t capacity) {
        this->resetBufferCapacity(capacity);
    }

    void Buffer::resetBufferSize(const std::size_t newSize) {
        if (0 == newSize) {
            this->buffer.resize(this->bufferCapacity);
        }
        else if(newSize > this->bufferCapacity) {
            throw Error("New buffer size can't exceed buffer capacity");
        }
        this->buffer.resize(newSize);
    }

    void Buffer::resetBufferCapacity(const std::size_t newCapacity) {
        if (0 == newCapacity) {
            throw Error("null capacity is invalid");
        }
        this->bufferCapacity = newCapacity;
    }
}
