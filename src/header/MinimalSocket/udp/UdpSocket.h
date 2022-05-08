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

class UdpSender;
class UdpBindable;
class UdpConnectable;

// can only send as no port was reserved
class UdpSender : public SenderTo, public RemoteAddressFamilyAware {
public:
  UdpSender(UdpSender &&o);
  UdpSender &operator=(UdpSender &&o);

  UdpSender(
      const AddressFamily &accepted_connection_family = AddressFamily::IP_V4);

  UdpBindable bind(const Port port_to_bind = ANY_PORT);
};

// can send and receive (from anyonw hitting it) as a port was reserved
class UdpBindable : public SenderTo,
                    public ReceiverUnkownSender,
                    public PortToBindAware,
                    public RemoteAddressFamilyAware,
                    public Openable {
public:
  UdpBindable(UdpBindable &&o);
  UdpBindable &operator=(UdpBindable &&o);

  UdpBindable(
      const Port port_to_bind = ANY_PORT,
      const AddressFamily &accepted_connection_family = AddressFamily::IP_V4);

  // throw in case address family is inconsistent
  UdpConnectable connect(const Address &remote_address);

  UdpConnectable connect(); // to first sending 1 byte

protected:
  void open_() override;
};

// can send and receive only from the specific remote address the socket was
// connected to
class UdpConnectable : public Sender,
                       public Receiver,
                       public PortToBindAware,
                       public RemoteAddressAware,
                       public Openable {
public:
  UdpConnectable(UdpConnectable &&o);
  UdpConnectable &operator=(UdpConnectable &&o);

  UdpConnectable(const Address &remote_address, const Port &port = ANY_PORT);

  UdpBindable disconnect();

protected:
  void open_() override;
};
} // namespace MinimalSocket::udp
