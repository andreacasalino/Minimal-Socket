/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGER_H_
#define _CROSS_SOCKET_MESSANGER_H_

#include <chrono>
#include <utility>

namespace sck {   
   class Sender {
   public:
      /**
       * @return true if the message was completely sent
       * @param[in] the message to send
       */
       virtual bool send(const std::pair<const char*, std::size_t>& message) = 0;
   };

   class Receiver {
   public:
      /**
       * @return the number of received bytes
       * @param[in] the recepient
       * @param[in] the timeout to consider
       */
      virtual std::size_t receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) = 0;
   };

   /**    
    * @brief The interface providing functionalities for exchanging data
    */
   class Messanger
      : public Sender
      , public Receiver {
   public:
      Messanger() = default;
      Messanger(const Messanger&) = delete;
      Messanger& operator=(const Messanger&) = delete;

      virtual ~Messanger() = default;
   };
}

#endif