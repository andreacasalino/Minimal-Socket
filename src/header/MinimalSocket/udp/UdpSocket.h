/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>
#include <MinimalSocket/core/SocketContext.h>

namespace MinimalSocket::udp {
/**
 * @brief refer to
 * https://en.wikipedia.org/wiki/User_Datagram_Protocol#:~:text=The%20field%20size%20sets%20a,−%2020%20byte%20IP%20header).
 */
static constexpr std::size_t MAX_UDP_RECV_MESSAGE = 65507;

class UdpConnected;

// can send and receive (from anyone hitting this socket) as a port was reserved
class UdpBinded : public SenderTo,
                  public ReceiverUnkownSender,
                  public PortToBindAware,
                  public RemoteAddressFamilyAware,
                  public Openable {
public:
  UdpBinded(UdpBinded &&o);
  UdpBinded &operator=(UdpBinded &&o);

  UdpBinded(
      const Port port_to_bind = ANY_PORT,
      const AddressFamily &accepted_connection_family = AddressFamily::IP_V4);

  // throw in case address family is inconsistent
  // leave this socket empty after success
  UdpConnected connect(const Address &remote_address);

  std::optional<UdpConnected>
  connect(const Timeout &timeout = NULL_TIMEOUT); // to first sending 1 byte

protected:
  void open_() override;
};

// can send and receive only from the specific remote address the socket was
// connected to
class UdpConnected : public Sender,
                     public Receiver,
                     public PortToBindAware,
                     public RemoteAddressAware,
                     public Openable {
public:
  UdpConnected(UdpConnected &&o);
  UdpConnected &operator=(UdpConnected &&o);

  UdpConnected(const Address &remote_address, const Port &port = ANY_PORT);

  // leave this socket empty after success
  UdpBinded disconnect();

protected:
  void open_() override;
};

UdpConnected makeUdpConnected(
    const Port &port = ANY_PORT,
    const AddressFamily &accepted_connection_family = AddressFamily::IP_V4,
    const Timeout &timeout = NULL_TIMEOUT);
} // namespace MinimalSocket::udp
