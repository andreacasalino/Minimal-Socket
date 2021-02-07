/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKET_H_
#define _CROSS_SOCKET_SOCKET_H_

#include <chrono>

namespace sck {   
   /**    
    * @brief The interface every socket must derive from.
    */
   class Socket {
   public:
      Socket() = default;
      Socket(const Socket&) = delete;
      Socket& operator=(const Socket&) = delete;

      virtual ~Socket() = default;
      
      virtual void open(const std::chrono::milliseconds& timeout) = 0;

      virtual void close() = 0;

      virtual bool isOpen() const = 0;
   };
}

#endif