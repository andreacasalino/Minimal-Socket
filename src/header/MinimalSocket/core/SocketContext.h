/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>

namespace MinimalSocket {
class RemoteAddressAware {
public:
  /**
   * @return the address of the peer that can exchange messages with this
   * socket.
   */
  const Address &getRemoteAddress() const { return remote_address; }

  RemoteAddressAware(const RemoteAddressAware &) = default;
  RemoteAddressAware &operator=(const RemoteAddressAware &) = default;

protected:
  /**
   * @throw in case the passed address is invalid (i.e. address == nullptr is
   * true).
   */
  RemoteAddressAware(const Address &address);

private:
  Address remote_address;
};

class PortToBindAware {
public:
  /**
   * @return the port that will be reserved, in case the socket was not already
   * opened, or the port actually reserved when the socket was opened.
   */
  Port getPortToBind() const { return port_to_bind; }

  PortToBindAware(const PortToBindAware &) = default;
  PortToBindAware &operator=(const PortToBindAware &) = default;

protected:
  PortToBindAware(const Port &port) : port_to_bind(port){};

  void setPort(const Port &port) { port_to_bind = port; };

private:
  Port port_to_bind;
};

class RemoteAddressFamilyAware {
public:
  /**
   * @return the address family of the peer that can exchange messages with this
   * socket.
   */
  AddressFamily getRemoteAddressFamily() const { return remote_address_family; }

  RemoteAddressFamilyAware(const RemoteAddressFamilyAware &) = default;
  RemoteAddressFamilyAware &
  operator=(const RemoteAddressFamilyAware &) = default;

protected:
  RemoteAddressFamilyAware(const AddressFamily &family)
      : remote_address_family(family){};

private:
  AddressFamily remote_address_family;
};
} // namespace MinimalSocket
