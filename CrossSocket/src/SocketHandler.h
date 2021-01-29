#ifndef _CROSS_SOCKET_SOCKETHANDLER_H_
#define _CROSS_SOCKET_SOCKETHANDLER_H_

#include <Address.h>
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
    * @brief returns the last error code raised by the socket API
    */
   int getLastError();

   void throwWithCode(const std::string& what);

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
   * @brief the integer representing a family type
   */
   int castFamily(const sck::Family& family);

   /**
   * contains the required things to work with the socket API
   */
   class SocketHandler {
   public:
      /**
       * @brief an empty socket is actually created
       * only after a call to SocketConnection::open this socket will be activated
       */
      SocketHandler();
      /**
       * @brief the handle is transferred into the object to create
       */
      SocketHandler(SocketHandler&& o);
      /**
       * @brief if the socket was activated, it's shut down and then closed
       */
      virtual ~SocketHandler();
      /**
       * @brief When before calling the destroyer invalidates the socket.
       * The socket cannot be used in any way after calling this method
       */
      void close();

      SocketHandler(const SocketHandler&) = delete;
      void operator=(const SocketHandler&) = delete;

      Socket_t handle;

      bool isActive() const;
   private:
      bool active;
#ifdef _WIN32
      /**
       * @brief When the manager is created WSAStartup is called,
       * when is destroyed WSACleanup is called
       */
      class WSAManagerSingleton {
      public:
         ~WSAManagerSingleton();
         static std::shared_ptr<WSAManagerSingleton> getManager();
      private:
         WSAManagerSingleton();
         static std::shared_ptr<WSAManagerSingleton> managerInstance;
      };
      std::shared_ptr<WSAManagerSingleton> manager;
#endif
   };

}

#endif