#include "SocketHandler.h"
#include <sstream>
#include <Error.h>

#ifdef _WIN32
#include <ws2tcpip.h>
constexpr int shutDownMode = 2; //inactivate both outgoing and incoming messages
#define WSA_VERSION_PREFIX 2 //no reason for using something different
#define WSA_VERSION_SUFFIX 0 //no reason for using something different
#else
#include <netdb.h>
#endif

namespace sck {
#ifdef _WIN32
   std::shared_ptr<SocketHandler::WSAManagerSingleton> SocketHandler::WSAManagerSingleton::managerInstance;

   std::shared_ptr<SocketHandler::WSAManagerSingleton> SocketHandler::WSAManagerSingleton::getManager() {
      if (nullptr == managerInstance) {
         managerInstance = std::shared_ptr<SocketHandler::WSAManagerSingleton>(new SocketHandler::WSAManagerSingleton()); //not possible to call make_shared because c'tor is private
      }
      return managerInstance;
   }

   SocketHandler::WSAManagerSingleton::WSAManagerSingleton() {
      WSADATA wsa;
      WSAStartup(MAKEWORD(WSA_VERSION_PREFIX, WSA_VERSION_SUFFIX), &wsa);
   }

   SocketHandler::WSAManagerSingleton::~WSAManagerSingleton() {
      WSACleanup();
   }
#endif

   /**
    * @brief tries to convert an address into a SocketAddressIn_t.
    * Returns false in case the conversion was not possible
    * There is no need to expose this function to the outside
    */
   bool tryConversion(SocketAddressIn_t& recipient, const sck::Address& address) {
#if !defined(_WIN32)
      in_addr ia;
      if (1 == ::inet_pton(AF_INET, address.getHost().c_str(), &ia)) {
         recipient.sin_addr.s_addr = ia.s_addr;
         return true;
      }
#endif

      addrinfo* res, hints = addrinfo{};
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;

      int gai_err = ::getaddrinfo(address.getHost().c_str(), NULL, &hints, &res);

#if !defined(_WIN32)
      if (gai_err == EAI_SYSTEM) {
         return false;
      }
#endif
      if (gai_err != 0) {
         return false;
      }

      auto ipv4 = reinterpret_cast<sockaddr_in*>(res->ai_addr);
      ::freeaddrinfo(res);
      recipient.sin_addr.s_addr = ipv4->sin_addr.s_addr;
      return true;
   };

   std::unique_ptr<SocketAddressIn_t> convertIpv4(const sck::Address& address) {
      if (sck::Family::IP_V4 == address.getFamily()) {
         std::unique_ptr<SocketAddressIn_t> resolved = std::make_unique<SocketAddressIn_t>();
         // set everything to 0 first
         ::memset(&(*resolved), 0, sizeof(SocketAddressIn_t));
         resolved->sin_family = AF_INET;
         if (!tryConversion(*resolved, address)) {
            return nullptr;
         }
         resolved->sin_port = ::htons(address.getPort());
         return resolved;
      }
      return nullptr;
   }

   /**
    * @brief tries to convert an address into a SocketAddressIn6_t.
    * Returns false in case the conversion was not possible
    * There is no need to expose this function to the outside
    */
   bool tryConversion(SocketAddressIn6_t& recipient, const sck::Address& address) {
#if !defined(_WIN32)
      in6_addr ia;
      if (1 == ::inet_pton(AF_INET6, address.getHost().c_str(), &ia)) {
         recipient.sin6_addr = ia;
         return true;
      }
#endif

      addrinfo* res, hints = addrinfo{};
      hints.ai_family = AF_INET6;
      hints.ai_socktype = SOCK_STREAM;

      int gai_err = ::getaddrinfo(address.getHost().c_str(), NULL, &hints, &res);

#if !defined(_WIN32)
      if (gai_err == EAI_SYSTEM) {
         return false;
      }
#endif
      if (gai_err != 0) {
         return false;
      }

      auto ipv6 = reinterpret_cast<sockaddr_in6*>(res->ai_addr);
      ::freeaddrinfo(res);
      recipient.sin6_addr = ipv6->sin6_addr;
      return true;
   };

   std::unique_ptr<SocketAddressIn6_t> convertIpv6(const sck::Address& address) {
      if (sck::Family::IP_V6 == address.getFamily()) {
         std::unique_ptr<SocketAddressIn6_t> resolved = std::make_unique<SocketAddressIn6_t>();
         // set everything to 0 first
         ::memset(&(*resolved), 0, sizeof(SocketAddressIn6_t));
         resolved->sin6_family = AF_INET6;
         resolved->sin6_flowinfo = 0;
         if (!tryConversion(*resolved, address)) {
            return nullptr;
         }
         resolved->sin6_port = ::htons(address.getPort());
         return resolved;
      }
      return nullptr;
   }

   AddressPtr convert(SocketAddress_t& address) {
      // refer to https://stackoverflow.com/questions/11684008/how-do-you-cast-sockaddr-structure-to-a-sockaddr-in-c-networking-sockets-ubu
      std::string ip;      
      std::uint16_t port;
      if (AF_INET == address.sa_family) {
         // ipv4 address
         // inet_ntoa is deprecated, but using inet_ntop for ipv4 address, leads to an ip that has no sense
         ip = std::string(::inet_ntoa(reinterpret_cast<const SocketAddressIn_t*>(&address)->sin_addr));
         port = ::ntohs(reinterpret_cast<const SocketAddressIn_t*>(&address)->sin_port);
      }
      else {
         // ipv6 address
         char temp[INET6_ADDRSTRLEN]; //this is the longest one
         // refer to https://www.gnu.org/software/libc/manual/html_node/Host-Address-Functions.html
         ::memset(temp, 0, INET6_ADDRSTRLEN);
         ::inet_ntop(address.sa_family, &address, temp, INET6_ADDRSTRLEN);
         ip = std::string(temp, INET6_ADDRSTRLEN);
         port = ::ntohs(reinterpret_cast<const SocketAddressIn6_t*>(&address)->sin6_port);
      }
      return sck::Address::create(ip, port);
   }

   int castFamily(const sck::Family& family) {
      switch (family) {
      case sck::Family::IP_V4:
         return AF_INET;
      case sck::Family::IP_V6:
         return AF_INET6;
      default:
         return -1;
      }
   }

   SocketHandler::~SocketHandler() {
      this->close();
   }

   SocketHandler::SocketHandler()
      : active(true)
      , handle(SCK_INVALID_SOCKET) {
#ifdef _WIN32
      this->manager = WSAManagerSingleton::getManager();
#endif
   }

   SocketHandler::SocketHandler(SocketHandler&& o)
      : active(o.active)
      , handle(o.handle) {
      o.active = false;
      o.handle = SCK_INVALID_SOCKET;
#ifdef _WIN32
      this->manager = o.manager;
#endif
   }

   int getLastError() {
#ifdef _WIN32
      return WSAGetLastError();
#else
      return static_cast<int>(errno);
#endif
   }

   void SocketHandler::close() {
      if (!this->active) {
         return;
      }
#ifdef _WIN32
      shutdown(this->handle, shutDownMode);
      closesocket(this->handle);
#else 
      ::shutdown(this->handle, SHUT_RDWR);
      ::close(this->handle);
#endif
      this->active = false;
      this->handle = SCK_INVALID_SOCKET;
   }

   bool SocketHandler::isActive() const {
      return this->active;
   }

   void throwWithCode(const std::string& what){
      std::stringstream s;
      s << what << " , error code: " << getLastError();
      throw Error(s.str());
   }
}