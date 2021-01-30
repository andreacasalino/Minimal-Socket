/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_HANDLER_H_
#define _CROSS_SOCKET_HANDLER_H_

#include <Address.h>
#include <SocketConcrete.h>
#include <atomic>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 
#include <ws2ipdef.h>
#define SCK_INVALID_SOCKET INVALID_SOCKET
#define SCK_SOCKET_ERROR   SOCKET_ERROR
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h> //memset
#include <unistd.h> //close
#include <errno.h>
#define SCK_INVALID_SOCKET -1
#define SCK_SOCKET_ERROR   -1
#endif

namespace sck {
   /**
   * socket handle
   */
#ifdef _WIN32
   using Socket_t = SOCKET;
#else
   using Socket_t = int;
#endif
   /**
   * @brief representation of a generic socket address
   */
#ifdef _WIN32
   using SocketAddress_t = SOCKADDR;
#else
   using SocketAddress_t = sockaddr;
#endif
   /**
   * @brief representation of an ipv4 socket address
   */
#ifdef _WIN32
   using SocketAddressIn_t = SOCKADDR_IN;
#else
   using SocketAddressIn_t = sockaddr_in;
#endif
   /**
   * @brief representation of an ipv6 socket address
   */
#ifdef _WIN32
   using SocketAddressIn6_t = SOCKADDR_IN6;
#else
   using SocketAddressIn6_t = sockaddr_in6;
#endif

   /**
    * @brief returns the last error code raised by the socket API
    */
   int getLastError();

   void throwWithCode(const std::string& what);

   /**
   * @brief checks the address syntax and in case
   * it's valid, creates the socket API representation
   * of the address
   */
   std::unique_ptr<SocketAddressIn_t> convertIpv4(const sck::Address& address);
   /**
   * @brief checks the address syntax and in case
   * it's valid, creates the socket API representation
   * of the address
   */
   std::unique_ptr<SocketAddressIn6_t> convertIpv6(const sck::Address& address);
   /**
    * @brief Convert a SocketAddress_t into an Address
    */
   AddressPtr convert(const SocketAddress_t& address);

   /**
   * contains the required things to work with the socket API
   */
   class Handler {
   public:
      Handler(const Handler&) = delete;
      Handler& operator=(const Handler&) = delete;

      /**
       * @brief a closed socket is created
       */
      Handler();

      /**
       * @brief the passed socket should be already opened
       */
      Handler(Socket_t extOpendSocket);

      virtual ~Handler();

      /**
       * @brief internally creates a new socket handler
       */
      void open(const Protocol& type, const Family& family);
      /**
       * @brief close and shutdown the current socket handler
       */
      void close();
      inline bool isOpen() const { return this->opened; };

      inline const Socket_t& getSocketId() const { return this->socketId; }

   private:
      std::atomic_bool opened;
      Socket_t socketId;

#ifdef _WIN32
      /**
       * @brief When the manager is created WSAStartup is called,
       * when is destroyed WSACleanup is called
       */
      class WSAManager {
      public:
         ~WSAManager();
         static std::shared_ptr<WSAManager> getManager();

      private:
         WSAManager();
         static std::shared_ptr<WSAManager> managerInstance;
      };
      std::shared_ptr<WSAManager> manager;
#endif
   };

}

#endif