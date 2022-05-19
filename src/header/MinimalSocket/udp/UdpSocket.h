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

/**
 * @brief This kind of udp is agnostic of the remote address (which can also
 * change over the time) exchanging messages with socket.
 * This udp can be reached by any other udp, on the port reserved when opening
 * this socket.
 * At the same time, this udp can send messages to any other non connected udp
 * sockets.
 */
class UdpBinded : public SenderTo,
                  public ReceiverUnkownSender,
                  public PortToBindAware,
                  public RemoteAddressFamilyAware,
                  public Openable {
public:
  UdpBinded(UdpBinded &&o);
  UdpBinded &operator=(UdpBinded &&o);

  /**
   * @brief The port is not reserved in this c'tor which
   * simply initialize this object. Such a thing is done when
   * when opening this socket
   * @param port_to_bind the port to reserve by this udp
   * @param accepted_connection_family the kind of udp that can reach this one
   */
  UdpBinded(
      const Port port_to_bind = ANY_PORT,
      const AddressFamily &accepted_connection_family = AddressFamily::IP_V4);

  /**
   * @brief Connects the udo socket to the specified remote address.
   * This leads to transfer the ownership of the underlying socket to the
   * returned object while this socket is left empty and closed.
   * @param remote_address the address to use for connecting the socket
   * @return a socket connected to the passed remote address
   */
  UdpConnected connect(const Address &remote_address);

  /**
   * @brief similar to connect(const Address &). Here, the remote address is not
   * explicitly specified, but is assumed to be equal to the first udp sending a
   * message to this one. The first sent message is not lost, and can be moved
   * into initial_message if not set to nullptr.
   * This is a blocking operation.
   * @param initial_message the initial message sent from the remote peer to
   * detect its address.
   */
  UdpConnected connect(std::string *initial_message = nullptr);

  /**
   * @brief similar to connect(std::string *initial_message), but non blocking.
   * If no remote peer sends a message within the timeout, a nullopt is
   * returned.
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking connect.
   * @param initial_message the initial message sent from the remote peer to
   * detect its address.
   */
  std::optional<UdpConnected> connect(const Timeout &timeout,
                                      std::string *initial_message = nullptr);

protected:
  void open_() override;
};

/**
 * @brief A udp that is permanently connected to a specific remote address.
 * Messages that are sent to this udp from different remote peer are ignored.
 * Attention!! Differently from tcp connections, udp socket are not connection
 * oriented.
 * The attribute "connected" for this socket simply means that messages
 * incoming from udp sockets different from the remote address are filtered out.
 * At the same time, the remote address might also not exists at all.
 */
class UdpConnected : public Sender,
                     public Receiver,
                     public PortToBindAware,
                     public RemoteAddressAware,
                     public Openable {
public:
  UdpConnected(UdpConnected &&o);
  UdpConnected &operator=(UdpConnected &&o);

  /**
   * @brief The connection to the remote address is not done in this c'tor which
   * simply initialize this object. Such a thing is done when
   * when opening this socket.
   * @param remote_address remote address of the peer
   * @param port the port to reserve by this udp
   */
  UdpConnected(const Address &remote_address, const Port &port = ANY_PORT);

  /**
   * @brief disconnect the underlying socket, generating an unbinded udp that
   * reserves the same port reserved by this one. This leaves this onbject
   * empty and closed.
   */
  UdpBinded disconnect();

protected:
  void open_() override;
};

/**
 * @brief builds from 0 a connected udp socket. The connection is done to the
 * first udp sending a message on the specified port.
 * This is a blocking operation.
 * @param port the port to reserve by the udp to build
 * @param accepted_connection_family the kind of remote udp that can asks the
 * connection
 * @param initial_message the message sent from the remote peer to detect its
 * address
 */
UdpConnected
makeUdpConnectedToUnknown(const Port &port,
                          const AddressFamily &accepted_connection_family,
                          std::string *initial_message = nullptr);

/**
 * @brief non blocking version of makeUdpConnectedToUnknown(const Port &, const
 * AddressFamily &, std::string *). In case no remote peer sends at least 1 byte
 * within the timeout, a nullopt is returned.
 */
std::optional<UdpConnected> makeUdpConnectedToUnknown(
    const Port &port, const AddressFamily &accepted_connection_family,
    const Timeout &timeout, std::string *initial_message = nullptr);
} // namespace MinimalSocket::udp
