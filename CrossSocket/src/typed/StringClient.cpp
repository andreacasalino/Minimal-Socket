/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <typed/StringClient.h>
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