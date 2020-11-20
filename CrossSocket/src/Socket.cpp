#include "../include/Socket.h"
#include "SocketHandler.h"

#ifdef _WIN32
#define REBIND_OPTION SO_REUSEADDR
#else
#define REBIND_OPTION SO_REUSEPORT
#endif

namespace sck {
   Socket::Socket()
      : channel(std::make_unique<SocketHandler>())
      , connected(false) {
   }

   Socket::Socket(std::unique_ptr<SocketHandler> channel)
      : channel(std::move(channel))
      , connected(false) {
   }

   Socket::~Socket() {
      this->close();
   }

   void Socket::open() {
      if (this->connected) {
         return;
      }
      try {
         if (!this->channel->isActive()) {
            this->channel.reset();
            this->channel = std::make_unique<SocketHandler>();
         }
         this->initHandle();
         this->openConnection();
      }
      catch (...) {
         this->close();
         return;
      }
      this->connected = true;
   }

   void Socket::close() {
      if (!this->connected) {
         return;
      }
      try {
         this->closeConnection();
      }
      catch (...) {
      }
      this->connected = false;
   }

   void Socket::bindToPort(const std::uint16_t& port) {
      int reusePortOptVal = 1;
      ::setsockopt(this->channel->handle, SOL_SOCKET, REBIND_OPTION, reinterpret_cast<const
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
         addr.sin_port = ::htons(port);
#ifdef _WIN32
         addr.sin_addr.s_addr = ADDR_ANY;
#else
         addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
#endif
         if (::bind(this->channel->handle, reinterpret_cast<SocketAddress_t*>(&addr), sizeof(SocketAddressIn_t)) == SCK_SOCKET_ERROR) {
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
         addr.sin6_port = ::htons(port);
         if (::bind(this->channel->handle, reinterpret_cast<SocketAddress_t*>(&addr), sizeof(SocketAddressIn6_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("can't bind localhost on port: " + std::to_string(port));
         }

      }
   }

   void Socket::closeConnection() {
      this->channel->close();
   }
}
