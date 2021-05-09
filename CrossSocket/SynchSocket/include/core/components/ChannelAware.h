/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CHANNELAWARE_H_
#define _CROSS_SOCKET_CHANNELAWARE_H_

#include <memory>

namespace sck {
    class Channel;

    class ChannelAware {
    public:
        virtual ~ChannelAware();
        
    protected:
        ChannelAware() = default;

        std::unique_ptr<Channel> channel;
    };
}

#endif