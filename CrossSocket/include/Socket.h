/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKET_H_
#define _CROSS_SOCKET_SOCKET_H_

#include <Address.h>
#include <atomic>
#include <chrono>

namespace sck {
   /**
    * @brief interface to the socket APIs
    */
   class SocketHandler;

   /**    
    * @brief The interface every socket must derive from.
    */
   class Socket {
   public:
      Socket(const Socket&) = delete;
      Socket& operator=(const Socket&) = delete;

      virtual ~Socket();
      
      void open(const std::chrono::milliseconds& timeout = std::chrono::milliseconds(0));

      void close();

      inline bool isOpen() const { return this->opened; }
      
   protected:
      Socket();
      /**
       * @param[in] an already created handler to steal
       */
      Socket(std::unique_ptr<SocketHandler> channel);

      virtual void openConnection() = 0;

      virtual void closeConnection();

      std::unique_ptr<SocketHandler> channel;

      virtual void initHandle() = 0;
 
      /**
       * @brief The value should be deduced from object to object
       */
      virtual sck::Family getFamily() = 0;

      void bindToPort(const std::uint16_t& port);

      std::atomic_bool opened;
   };
}

#endif