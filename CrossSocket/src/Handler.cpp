/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "Handler.h"
#include <sstream>
#include <Error.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif

namespace sck {
#ifdef _WIN32
   std::shared_ptr<SocketHandler::WSAManager> SocketHandler::WSAManager::managerInstance;

   std::shared_ptr<SocketHandler::WSAManager> SocketHandler::WSAManager::getManager() {
      if (nullptr == managerInstance) {
         managerInstance = std::shared_ptr<SocketHandler::WSAManager>(new SocketHandler::WSAManager()); //not possible to call make_shared because c'tor is private
      }
      return managerInstance;
   }

   SocketHandler::WSAManager::WSAManager() {
      WSADATA wsa;
      WSAStartup(MAKEWORD(2, 0), &wsa);
   }

   SocketHandler::WSAManager::~WSAManager() {
      WSACleanup();
   }
#endif

   int getLastError() {
#ifdef _WIN32
      return WSAGetLastError();
#else
      return static_cast<int>(errno);
#endif
   }

   void throwWithCode(const std::string& what){
      std::stringstream s;
      s << what << " , error code: " << getLastError();
      throw Error(s.str());
   }

   std::unique_ptr<SocketAddressIn_t> convertIpv4(const sck::Address& address) {
      auto tryConversion = [](SocketAddressIn_t& recipient, const sck::Address& address) -> bool {
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

      if (sck::Family::IP_V4 == address.getFamily()) {
         std::unique_ptr<SocketAddressIn_t> resolved = std::make_unique<SocketAddressIn_t>();
         // set everything to 0 first
         ::memset(&(*resolved), 0, sizeof(SocketAddressIn_t));
         resolved->sin_family = AF_INET;
         if (!tryConversion(*resolved, address)) {
            return nullptr;
         }
         resolved->sin_port = htons(address.getPort());
         return resolved;
      }
      return nullptr;
   }

   std::unique_ptr<SocketAddressIn6_t> convertIpv6(const sck::Address& address) {
         auto tryConversion = [](SocketAddressIn6_t& recipient, const sck::Address& address) -> bool { 
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

      if (sck::Family::IP_V6 == address.getFamily()) {
         std::unique_ptr<SocketAddressIn6_t> resolved = std::make_unique<SocketAddressIn6_t>();
         // set everything to 0 first
         ::memset(&(*resolved), 0, sizeof(SocketAddressIn6_t));
         resolved->sin6_family = AF_INET6;
         resolved->sin6_flowinfo = 0;
         if (!tryConversion(*resolved, address)) {
            return nullptr;
         }
         resolved->sin6_port = htons(address.getPort());
         return resolved;
      }
      return nullptr;
   }

   AddressPtr convert(const SocketAddress_t& address) {
      // refer to https://stackoverflow.com/questions/11684008/how-do-you-cast-sockaddr-structure-to-a-sockaddr-in-c-networking-sockets-ubu
      std::string ip;      
      std::uint16_t port;
      if (AF_INET == address.sa_family) {
         // ipv4 address
         // inet_ntoa is deprecated, but using inet_ntop for ipv4 address, leads to an ip that has no sense
         ip = std::string(::inet_ntoa(reinterpret_cast<const SocketAddressIn_t*>(&address)->sin_addr));
         port = ntohs(reinterpret_cast<const SocketAddressIn_t*>(&address)->sin_port);
      }
      else {
         // ipv6 address
         char temp[INET6_ADDRSTRLEN]; //this is the longest one
         // refer to https://www.gnu.org/software/libc/manual/html_node/Host-Address-Functions.html
         ::memset(temp, 0, INET6_ADDRSTRLEN);
         ::inet_ntop(address.sa_family, &address, temp, INET6_ADDRSTRLEN);
         ip = std::string(temp, INET6_ADDRSTRLEN);
         port = ntohs(reinterpret_cast<const SocketAddressIn6_t*>(&address)->sin6_port);
      }
      return sck::Address::create(ip, port);
   }

   Handler::~Handler() {
      if(this->opened){
         this->close();
      }
   }

   Handler::Handler()
      : opened(false)
      , socketId(SCK_INVALID_SOCKET) {
#ifdef _WIN32
      this->manager = WSAManager::getManager();
#endif
   }

   Handler::Handler(Socket_t extOpendSocket)
      : Handler() {
      this->opened = true;
      this->socketId = extOpendSocket;
   }

   void Handler::open(const Protocol& type, const Family& family) {
      if(this->opened) return;

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
         this->socketId = ::socket(toIntFamily(family), SOCK_STREAM, 0);
         if (this->socketId == SCK_INVALID_SOCKET) {
            this->close();
            throwWithCode("Stream socket could not be created");
         }         
         break;
      case Protocol::UDP:
         this->socketId = ::socket(toIntFamily(family), SOCK_DGRAM, 0);
         if (this->socketId == SCK_INVALID_SOCKET) {
            this->close();
            throwWithCode("DataGram socket could not be created");
         }         
      default:
         throw Error("unknown protocol type");
      }

      this->opened = true;
   }

   void Handler::close() {
      if (!this->opened) return;
#ifdef _WIN32
      shutdown(this->socketId, 2);
      closesocket(this->socketId);
#else 
      ::shutdown(this->socketId, SHUT_RDWR);
      ::close(this->socketId);
#endif
      this->opened = false;
      this->socketId = SCK_INVALID_SOCKET;
   }
}