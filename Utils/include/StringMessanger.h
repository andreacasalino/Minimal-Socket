/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MessangerDecorator.h>
#include <string.h>

namespace sck {
    class StringMessanger 
        : public MessangerDecorator<std::string, std::string> {
    public:
        StringMessanger(std::unique_ptr<SocketClient> wrapped, const std::size_t recvCapacity);

    private:
        void encode(const std::string& message) final;
        void decode(std::string& message) final;
    };
}