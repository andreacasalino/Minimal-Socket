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
class UdpBinded;
class UdpConnected;

// can only send as no port was reserved
class UdpSender : public SenderTo {
public:
  UdpSender(UdpSender &&o);
  UdpSender &operator=(UdpSender &&o);

  UdpSender();

  UdpBinded bind(const Port &port);
};

// can send and receive (from anyonw hitting it) as a port was reserved
class UdpBinded : public SenderTo,
                  public ReceiverUnkownSender,
                  public BindedPortAware,
                  public Openable {
public:
  UdpBinded(UdpBinded &&o);
  UdpBinded &operator=(UdpBinded &&o);

  UdpBinded(const Port &port);
  UdpBinded(UdpSender &&previous_phase, const Port &port);

  UdpConnected connect(); // to first sending 1 byte
  UdpConnected connect(const Address &remote_address);

protected:
  bool open_() override;
};

// can send and receive only from the specific remote address the socket was
// connected to
class UdpConnected : public Sender,
                     public Receiver,
                     public BindedPortAware,
                     public RemoteAddressAware,
                     public Openable {
public:
  UdpConnected(UdpConnected &&o);
  UdpConnected &operator=(UdpConnected &&o);

  UdpConnected(const Port &port); // to first sending 1 byte
  UdpConnected(const Port &port, const Address &remote_address);
  UdpConnected(UdpBinded &&previous_phase,
               const Port &port); // to first sending 1 byte
  UdpConnected(UdpBinded &&previous_phase, const Port &port,
               const Address &remote_address);

  UdpBinded disconnect();

protected:
  bool open_() override;
};
} // namespace MinimalSocket::udp
