#include "../include/SocketClient.h"
#include "SocketHandler.h"

namespace sck {
   SocketClient::SocketClient(const sck::Address& remoteAddress)
      : Socket()
      , remoteAddress(remoteAddress)
      , actualTimeOut(std::chrono::milliseconds(0)) {
   }

   SocketClient::SocketClient(const sck::Address& remoteAddress, std::unique_ptr<SocketHandler> channel)
      : Socket(std::move(channel))
      , remoteAddress(remoteAddress)
      , actualTimeOut(std::chrono::milliseconds(0)) {
   }

   const sck::Address& SocketClient::getRemoteAddress() const {
      return this->remoteAddress;
   }

   sck::Family SocketClient::getFamily() {
      return this->remoteAddress.getFamily();
   }

   std::size_t SocketClient::send(const char* buffer, const std::size_t& bufferSize) {
      int sentBytes = ::send(this->channel->handle, buffer, static_cast<int>(bufferSize), 0);
      if (sentBytes == SCK_SOCKET_ERROR) {
         sentBytes = 0;
         throwWithCode("send failed");
      }
      return static_cast<std::size_t>(sentBytes);
   }

   std::size_t SocketClient::receive(char* buffer, const std::size_t & bufferMaxSize, const std::chrono::milliseconds& timeout) {
      this->resetTimeOut(timeout);
      int recvBytes = ::recv(this->channel->handle, buffer, static_cast<int>(bufferMaxSize), 0);
      if (recvBytes == SCK_SOCKET_ERROR) {
         recvBytes = 0;
         throwWithCode("receive failed");
      }
      if (recvBytes > bufferMaxSize) {
         // if here, the message received is probably corrupted
         recvBytes = 0;
      }
      return static_cast<std::size_t>(recvBytes);
   }

   void SocketClient::resetTimeOut(const std::chrono::milliseconds& timeout) {
      if (timeout.count() != this->actualTimeOut.count()) {
         //set new timeout
         this->actualTimeOut = timeout;
#ifdef _WIN32
         auto tv = DWORD(this->actualTimeOut.count());
         if (setsockopt(this->channel->handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(DWORD)) == SOCKET_ERROR) {
#else
         struct timeval tv = { 0,0 };
         if (this->actualTimeOut.count() >= 1000) {
            tv.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(this->actualTimeOut).count();
         }
         else {
            tv.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(this->actualTimeOut).count();
         }
         if (::setsockopt(this->channel->handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(struct timeval)) < 0) {
#endif
            throwWithCode("can't set timeout");
         }
      }
   }

   void SocketClient::openConnection() {
      if (!this->remoteAddress.isValid()) {
         throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid server address");
      }
      if (sck::Family::IP_V4 == this->getFamily()) {
         //v4 family
         auto addr = resolveIPv4(this->remoteAddress);
         if (!addr) {
            throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid server address");
         }
         if (::connect(this->channel->handle, reinterpret_cast<SocketAddress_t*>(&(*addr)), sizeof(SocketAddressIn_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("Connection can't be established");
         }
      }
      else {
         //v6 family
         auto addr = resolveIPv6(this->remoteAddress);
         if (!addr) {
            throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid server address");
         }
         if (::connect(this->channel->handle, reinterpret_cast<SocketAddress_t*>(&(*addr)), sizeof(SocketAddressIn6_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("Connection can't be established");
         }
      }
   }
}