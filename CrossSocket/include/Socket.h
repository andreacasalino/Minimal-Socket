#ifndef _CROSS_SOCKET_SOCKET_H_
#define _CROSS_SOCKET_SOCKET_H_

#include "Address.h"
#include <memory>

namespace sck {
   /**
    * @brief interface to the socket APIs
    */
   class SocketHandler;

   /**    
    * @class Socket Socket.h <wb/connect/Socket.h>
    * @brief The interface every socket must derive from.
    * Open and close are implements the state transitions and relies 
    * on openConnection and closeConnection, which should contain
    * the sequence of operations to actually open and close a particular socket.
    * @anchor SocketConnection
    */
   class Socket {
   public:
      virtual ~Socket();
      
      void open();

      void close();
   protected:
      /**
       * @param[in] the name to use for building Loggable
       */
      Socket();
      /**
       * @param[in] the name to use for building Loggable
       * @param[in] an already created handler to steal
       */
      Socket(std::unique_ptr<SocketHandler> channel);

      /**
       * @brief Put here the sequence of commands to fully open an object and not inside initHandle
       */
      virtual void openConnection() = 0;

      /**
       * @brief By default, simply calls channel->close()
       */
      virtual void closeConnection();

      std::unique_ptr<SocketHandler> channel;

      /**
       * @brief Initialize the socket handler (channel should not be empty when calling)
       */
      virtual void initHandle() = 0;
 
      /**
       * @brief get the protocol family of the socket (ipv4 or ipv6, refer to https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rzab6/address.htm).
       * The value should be deduced from object to object
       */
      virtual sck::Family getFamily() = 0;

      /**
       * @brief Binds the specified port.
       * It might be used by tcp server, udp server and udp client
       * @param[in] the port to bind
       */
      void bindToPort(const std::uint16_t& port);

      bool connected;
   };
}

#endif