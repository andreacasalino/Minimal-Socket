/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CORE_H_
#define _CROSS_SOCKET_CORE_H_

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
#include <Ip.h>
#include <core/SocketConcrete.h>
#include <atomic>
#include <mutex>

namespace sck {
   /**
   * socket handle
   */
#ifdef _WIN32
   using SocketHandler = SOCKET;
#else
   using SocketHandler = int;
#endif
   /**
   * @brief representation of a generic socket address
   */
#ifdef _WIN32
   using SocketIp = SOCKADDR;
#else
   using SocketIp = sockaddr;
#endif
   /**
   * @brief representation of an ipv4 socket address
   */
#ifdef _WIN32
   using SocketIp4 = SOCKADDR_IN;
#else
   using SocketIp4 = sockaddr_in;
#endif
   /**
   * @brief representation of an ipv6 socket address
   */
#ifdef _WIN32
   using SocketIp6 = SOCKADDR_IN6;
#else
   using SocketIp6 = sockaddr_in6;
#endif

   /**
    * @brief returns the last error code raised by the socket API
    */
   int getLastErrorCode();

   void throwWithCode(const std::string& what);

   /**
   * @brief checks the address syntax and in case
   * it's valid as an ipv4, creates the socket API representation
   * of the address
   */
   std::unique_ptr<SocketIp4> convertIpv4(const sck::Ip& address);
   /**
   * @brief checks the address syntax and in case
   * it's valid as an ipv6, creates the socket API representation
   * of the address
   */
   std::unique_ptr<SocketIp6> convertIpv6(const sck::Ip& address);
   /**
    * @brief Convert a SocketAddress_t into an Address, internally
    * deducing the family.
    */
   IpPtr convert(const SocketIp& address);

   /**
    * An object storing a socket API handler and containing the minimal 
    * functionalities for interacting with it.
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
       * @brief the passed handler should be already created externally
       * by the socket api
       */
      explicit Handler(const SocketHandler& hndl);

      ~Handler();

      /**
       * @brief internally creates a new socket
       */
      void open(const Protocol& type, const Family& family);
      /**
       * @brief close and shutdown the current socket
       */
      void close();

      inline bool isOpen() const { return this->opened; };

      inline const SocketHandler& operator*() const { return this->hndl; };

   private:
      std::atomic_bool opened;
      SocketHandler hndl;

#ifdef _WIN32
      class SocketHandlerFactory {
      public:
          /**
           * @brief If we are about to open the first socket, WSAStartup() is invoked
           */
          static void beforeOpen();
          /**
           * @brief If we are closing the last socket, WSACleanup() is invoked
           */
          static void afterClose();

      private:
          static std::mutex handlerCounterMtx;
          static std::size_t handlerCounter;
      };
#endif
   };

}

#endif