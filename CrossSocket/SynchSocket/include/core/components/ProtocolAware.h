/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_PROTOCOLAWARE_H_
#define _CROSS_SOCKET_PROTOCOLAWARE_H_

namespace sck {
    enum Protocol { UDP, TCP };

    class ProtocolAware {
    protected:
        virtual sck::Protocol getProtocol() const = 0;
    };
}

#endif