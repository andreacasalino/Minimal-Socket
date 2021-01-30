/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGER_H_
#define _CROSS_SOCKET_MESSANGER_H_

#include <chrono>

namespace sck {   
   /**    
    * @brief The interface providing functionalities for exchanging data
    */
   template<typename Send_t, typename Recv_t>
   class Messanger {
   public:
      Messanger() = default;
      Messanger(const Messanger&) = delete;
      Messanger& operator=(const Messanger&) = delete;

      virtual ~Messanger();
      
      /**
       * @return true if the message was completely sent
       * @param[in] the message to send
       */
       virtual bool send(const Send_t& message) = 0;

      /**
       * @return the number of received bytes
       * @param[in] the recepient
       * @param[in] the timeout to consider
       */
      virtual std::size_t receive(Recv_t& message, const std::chrono::milliseconds& timeout) = 0;
   };
}

#endif