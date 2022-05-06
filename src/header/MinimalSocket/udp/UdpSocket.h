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

#include <variant>

namespace MinimalSocket::udp {
class UdpSender;
class UdpBinded;
class UdpConnected;

class UdpSender : public SenderTo {
public:
  UdpSender(UdpSender &&o);
  UdpSender &operator=(UdpSender &&o);

  UdpBinded &&bind(const Port &port);
};

class UdpBinded : public SenderTo,
                  public ReceiverUnkownSender,
                  public BindedPortAware {
public:
  UdpBinded(UdpBinded &&o);
  UdpBinded &operator=(UdpBinded &&o);

  UdpBinded(const Port &port);
  UdpBinded(UdpSender &&previous_phase, const Port &port);

  UdpSender &&unbind();

  UdpConnected &&connect(); // to first sending 1 byte
  UdpConnected &&connect(const Address &remote_address);
};

class UdpConnected : public Sender,
                     public Receiver,
                     public BindedPortAware,
                     public RemoteAddressAware {
public:
  UdpConnected(UdpConnected &&o);
  UdpConnected &operator=(UdpConnected &&o);

  UdpConnected(const Port &port); // to first sending 1 byte
  UdpConnected(const Port &port, const Address &remote_address);
  UdpConnected(UdpBinded &&previous_phase,
               const Port &port); // to first sending 1 byte
  UdpConnected(UdpBinded &&previous_phase, const Port &port,
               const Address &remote_address);

  UdpBinded &&disconnect();
};

using UdpSocket = std::variant<UdpSender, UdpBinded, UdpConnected>;
} // namespace MinimalSocket::udp
