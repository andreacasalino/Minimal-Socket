/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <SocketConcrete.h>
#include "Handler.h"
#include <thread>
#include <condition_variable>

#ifdef _WIN32
#define REBIND_OPTION SO_REUSEADDR
#else
#define REBIND_OPTION SO_REUSEPORT
#endif

namespace sck {
    SocketConcrete::SocketConcrete(std::shared_ptr<Handler> channel)
        : channel(channel) {
    }

    SocketConcrete::~SocketConcrete() {
        if(this->isOpen()) {
            this->close();
        }
    }

    void SocketConcrete::open(const std::chrono::milliseconds& timeout) {
      if (this->isOpen()) {
        return;
      }
      
      std::atomic_bool stopWait(false);
      auto openSteps = [this, &stopWait]() {
         try {
            this->channel->open(this->getProtocol(), this->getFamily());
            this->openSpecific();
         }
         catch (...) {
            this->close();
            stopWait = true; 
            return;
         }
         stopWait = true;
      };

      if(0 == timeout.count()) {
         openSteps();
      }
      else {
         std::condition_variable notification;
         std::mutex notificationMtx;
         std::thread opener(openSteps);

         std::unique_lock<std::mutex> notificationLck(notificationMtx);
         notification.wait_for(notificationLck, timeout, [&stopWait](){ return static_cast<bool>(stopWait); });
         if(!this->isOpen()) {
            this->close();
         }
      }
   }

    void SocketConcrete::close() {
        if (!this->isOpen()) {
            return;
        }
        try {
            this->closeSpecific();
        }
        catch (...) {
        }
    }

    void SocketConcrete::closeSpecific() {
        this->channel->close();
    }

    void SocketConcrete::bindToPort(const std::uint16_t& port) {
      int reusePortOptVal = 1;
      ::setsockopt(this->channel->getSocketId(), SOL_SOCKET, REBIND_OPTION, reinterpret_cast<const
#ifdef _WIN32
         char*  // not sure it would work with void* also in Windows
#else
         void*
#endif
      >(&reusePortOptVal), sizeof(int));

      // bind the server to the port
      if (sck::Family::IP_V4 == this->getFamily()) {
         //v4 family
         SocketAddressIn_t addr;
         ::memset(&addr, 0, sizeof(SocketAddressIn_t));
         addr.sin_family = AF_INET;
         addr.sin_port = htons(port);
#ifdef _WIN32
         addr.sin_addr.s_addr = ADDR_ANY;
#else
         addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
         if (::bind(this->channel->getSocketId(), reinterpret_cast<SocketAddress_t*>(&addr), sizeof(SocketAddressIn_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("can't bind localhost on port: " + std::to_string(port));
         }
      }
      else {
         //v6 family
         SocketAddressIn6_t addr;
         ::memset(&addr, 0, sizeof(SocketAddressIn6_t));
         addr.sin6_family = AF_INET6;
         addr.sin6_flowinfo = 0;
         addr.sin6_addr = IN6ADDR_ANY_INIT;  // apparently, there is no such a cross-system define for ipv4 addresses
         addr.sin6_port = htons(port);
         if (::bind(this->channel->getSocketId(), reinterpret_cast<SocketAddress_t*>(&addr), sizeof(SocketAddressIn6_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("can't bind localhost on port: " + std::to_string(port));
         }
      }
    }
}