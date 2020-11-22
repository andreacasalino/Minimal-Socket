#include "../include/StringClient.h"
#include <cstring>

namespace sck {
    bool StringClient::send(const std::string& mex) {
        return (this->client->send(mex.data(), mex.size()) == mex.size()); // check the # bytes sent is equal to the string lengths
    };
    bool StringClient::decode(std::string& toParse) {
        toParse.resize(this->buffer.size());
        std::memcpy(toParse.data(), this->buffer.data(), this->buffer.size());
        return true;
    };
}