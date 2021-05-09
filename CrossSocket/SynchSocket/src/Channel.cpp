/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "Channel.h"
#include <Error.h>

namespace sck {
#ifdef _WIN32
    std::size_t Channel::SocketHandlerFactory::handlerCounter = 0;
    std::mutex Channel::SocketHandlerFactory::handlerCounterMtx;

    void Channel::SocketHandlerFactory::beforeOpen() {
        std::lock_guard<std::mutex> hndLck(handlerCounterMtx);
        ++handlerCounter;
        if (1 == handlerCounter) {
            // first socket opened
            WSADATA wsa;
            WSAStartup(MAKEWORD(2, 0), &wsa);
        }
    }

    void Channel::SocketHandlerFactory::afterClose() {
        std::lock_guard<std::mutex> hndLck(handlerCounterMtx);
        --handlerCounter;
        if (0 == handlerCounter) {
            // last socket closed
            WSACleanup();
        }
    }
#endif

    Channel::~Channel() {
        if (this->opened) {
            this->close();
        }
    }

    Channel::Channel()
        : opened(false)
        , hndl(SCK_INVALID_SOCKET) {
    }

    Channel::Channel(const SocketHandler& hndl)
        : opened(true)
        , hndl(hndl) {
#ifdef _WIN32
        SocketHandlerFactory::beforeOpen();
#endif
    }

    void Channel::open(const Protocol& type, const Family& family) {
        if (this->opened) return;
#ifdef _WIN32
        SocketHandlerFactory::beforeOpen();
#endif
        auto toIntFamily = [](const Family& family) -> int {
            switch (family) {
            case sck::Family::IP_V4:
                return AF_INET;
            case sck::Family::IP_V6:
                return AF_INET6;
            default:
                throw Error("unknown address family type");
            }
            throw Error("unknown address family type");
        };

        switch (type) {
        case Protocol::TCP:
            this->hndl = ::socket(toIntFamily(family), SOCK_STREAM, 0);
            if (this->hndl == SCK_INVALID_SOCKET) {
                this->close();
                throwWithCode("Stream socket could not be created");
            }
            break;
        case Protocol::UDP:
            this->hndl = ::socket(toIntFamily(family), SOCK_DGRAM, 0);
            if (this->hndl == SCK_INVALID_SOCKET) {
                this->close();
                throwWithCode("DataGram socket could not be created");
            }
            break;
        default:
            throw Error("unknown protocol type");
        }

        this->opened = true;
    }

    void Channel::close() {
        if (!this->opened) return;
#ifdef _WIN32
        shutdown(this->hndl, 2);
        closesocket(this->hndl);
#else 
        ::shutdown(this->hndl, SHUT_RDWR);
        ::close(this->hndl);
#endif
        this->opened = false;
        this->hndl = SCK_INVALID_SOCKET;
#ifdef _WIN32
        SocketHandlerFactory::afterClose();
#endif
    }
}