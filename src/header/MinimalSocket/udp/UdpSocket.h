/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/NonCopiable.h>
#include <MinimalSocket/core/Receiver.h>
#include <MinimalSocket/core/Sender.h>
#include <MinimalSocket/core/SocketContext.h>

namespace MinimalSocket::udp {
/**
 * @brief refer to
 * https://en.wikipedia.org/wiki/User_Datagram_Protocol#:~:text=The%20field%20size%20sets%20a,−%2020%20byte%20IP%20header).
 */
static constexpr std::size_t MAX_UDP_RECV_MESSAGE = 65507;

template <bool BlockMode> class UdpConnected;

class UdpBase : public NonCopiable,
                public SenderTo,
                public PortToBindAware,
                public RemoteAddressFamilyAware,
                public Openable {
protected:
  UdpBase(UdpBase &&o);

  void stealBase(UdpBase &o);

  UdpBase(Port port_to_bind, AddressFamily accepted_connection_family,
          bool blockMode);

  void open_() override;
};

class UdpBlocking : public UdpBase, public ReceiverUnkownSender<true> {
public:
  /**
   * @brief Connects the udp socket to the specified remote address.
   * This leads to transfer the ownership of the underlying socket to the
   * returned object while this socket is left empty and closed.
   * @param remote_address the address to use for connecting the socket
   * @return a socket connected to the passed remote address
   */
  UdpConnected<true> connect(const Address &remote_address);

  /**
   * @brief similar to connect(const Address &). Here, the remote address is not
   * explicitly specified, but is assumed to be equal to the one of the first
   * socket sending a message to this one. The first sent message is not lost,
   * and can be moved into initial_message if not set to nullptr.
   * Notice that this is a  blocking operation.
   * @param initial_message the initial message sent from the remote peer.
   */
  UdpConnected<true> connect(std::string *initial_message = nullptr);

  /**
   * @brief similar to connect(std::string *initial_message), but non blocking.
   * If no remote peer sends a message within the timeout, a nullopt is
   * returned.
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking connect.
   * @param initial_message the initial message sent from the remote peer to
   * detect its address.
   */
  std::optional<UdpConnected<true>>
  connect(const Timeout &timeout, std::string *initial_message = nullptr);

protected:
  template <typename... Args>
  UdpBlocking(Args &&...args) : UdpBase{std::forward<Args>(args)...} {}
};

class UdpNonBlocking : public UdpBase, public ReceiverUnkownSender<false> {
public:
  /**
   * @brief Connects the udp socket to the specified remote address.
   * This leads to transfer the ownership of the underlying socket to the
   * returned object while this socket is left empty and closed.
   * @param remote_address the address to use for connecting the socket
   * @return a socket connected to the passed remote address
   */
  UdpConnected<false> connect(const Address &remote_address);

  /**
   * @brief similar to connect(const Address &). Here, the remote address is not
   * explicitly specified, but is assumed to be equal to the first socket
   * sending a message to this one. The first sent message is not lost, and can
   * be moved into initial_message if not set to nullptr.
   * Notice that this is a non blocking operation, meaning that if no message
   * was sent to this socket before calling this method, a nullopt is
   * immediately returned.
   * @param initial_message the initial message sent from the remote peer to
   * detect its address.
   */
  std::optional<UdpConnected<false>>
  connect(std::string *initial_message = nullptr);

protected:
  template <typename... Args>
  UdpNonBlocking(Args &&...args) : UdpBase{std::forward<Args>(args)...} {}
};

template <bool BlockMode> class Udp_ {};
template <> class Udp_<true> : public UdpBlocking {
protected:
  template <typename... Args>
  Udp_(Args &&...args) : UdpBlocking{std::forward<Args>(args)...} {}
};
template <> class Udp_<false> : public UdpNonBlocking {
protected:
  template <typename... Args>
  Udp_(Args &&...args) : UdpNonBlocking{std::forward<Args>(args)...} {}
};

/**
 * @brief This kind of udp is agnostic of the remote address (which can also
 * change over the time) of the socket(s) exchanging messages with this one.
 * This udp can be reached by any other udp, on the port reserved when opening
 * this socket.
 * At the same time, this udp can send messages to any other udp
 * sockets.
 */
template <bool BlockMode> class Udp : public Udp_<BlockMode> {
public:
  /**
   * @brief After construction, the socket is left closed.
   * The port is actually reserved after opening the socket.
   * @param port_to_bind the port to reserve by this udp
   * @param accepted_connection_family the kind of udp that can reach this one
   */
  Udp(Port port_to_bind = ANY_PORT,
      AddressFamily accepted_connection_family = AddressFamily::IP_V4)
      : Udp_<BlockMode>{port_to_bind, accepted_connection_family, BlockMode} {}

  Udp(Udp<BlockMode> &&o) : Udp_<BlockMode>{std::forward<Udp_<BlockMode>>(o)} {}
  Udp &operator=(Udp<BlockMode> &&o) {
    this->stealBase(o);
    return *this;
  }
};

class UdpConnectedBase : public NonCopiable,
                         public Sender,
                         public PortToBindAware,
                         public RemoteAddressAware,
                         public Openable {
protected:
  UdpConnectedBase(UdpConnectedBase &&o);

  void stealBase(UdpConnectedBase &o);

  UdpConnectedBase(const Address &remote_address, Port port, bool blockMode);

  void open_() override;
};

/**
 * @brief A udp that is permanently "connected" to a specific remote address.
 * Messages sent from senders with an address different from the specified
 * remote ones are ignored.
 *
 * Attention!! Differently from tcp connections, udp
 * socket are not connection oriented. The attribute "connected" for this socket
 * simply means that the os filter out message incoming from peers different
 * from the specified one. At the same time, the "connected" remote peer
 * might also not exist at the time of opening or using the socket.
 */
template <bool BlockMode>
class UdpConnected : public UdpConnectedBase, public Receiver<BlockMode> {
public:
  /**
   * @brief After construction, the socket is left closed.
   * The port is actually reserved after opening the socket.
   * @param remote_address remote address of the peer
   * @param port the port to reserve by this udp
   */
  UdpConnected(const Address &remote_address, Port port_to_bind = ANY_PORT)
      : UdpConnectedBase{remote_address, port_to_bind, BlockMode} {}

  UdpConnected(UdpConnected<BlockMode> &&o)
      : UdpConnectedBase{std::forward<UdpConnectedBase>(o)} {}
  UdpConnected &operator=(UdpConnected<BlockMode> &&o) {
    this->stealBase(o);
    return *this;
  }
};

/**
 * @brief builds from zero a connected udp socket. The connection is done to the
 * first udp sending a message on the specified port.
 * This is a blocking operation.
 * @param port the port to reserve by the udp to build
 * @param accepted_connection_family the kind of remote udp that can asks the
 * connection
 * @param initial_message the message sent from the remote peer to detect its
 * address
 */
UdpConnected<true>
makeUdpConnectedToUnknown(Port port, AddressFamily accepted_connection_family,
                          std::string *initial_message = nullptr);

} // namespace MinimalSocket::udp
