/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/BindedPortAware.h>
#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/RemoteAddressAware.h>
#include <MinimalSocket/core/Sender.h>

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
class UdpSender : public SenderTo {
public:
  UdpSender(UdpSender &&o);
  UdpSender &operator=(UdpSender &&o);

  UdpSender();

  UdpBindable bind(const Port &port);
};

// can send and receive (from anyonw hitting it) as a port was reserved
class UdpBindable : public SenderTo,
                    public ReceiverUnkownSender,
                    public BindedPortAware,
                    public Openable {
public:
  UdpBindable(UdpBindable &&o);
  UdpBindable &operator=(UdpBindable &&o);

  UdpBindable(const Port &port);
  UdpBindable(UdpSender &&previous_phase, const Port &port);

  UdpConnectable connect(); // to first sending 1 byte
  UdpConnectable connect(const Address &remote_address);

protected:
  bool open_() override;
};

// can send and receive only from the specific remote address the socket was
// connected to
class UdpConnectable : public Sender,
                       public Receiver,
                       public BindedPortAware,
                       public RemoteAddressAware,
                       public Openable {
public:
  UdpConnectable(UdpConnectable &&o);
  UdpConnectable &operator=(UdpConnectable &&o);

  UdpConnectable(const Port &port); // to first sending 1 byte
  UdpConnectable(const Port &port, const Address &remote_address);
  UdpConnectable(UdpBindable &&previous_phase,
                 const Port &port); // to first sending 1 byte
  UdpConnectable(UdpBindable &&previous_phase, const Port &port,
                 const Address &remote_address);

  UdpBindable disconnect();

protected:
  bool open_() override;
};
} // namespace MinimalSocket::udp
