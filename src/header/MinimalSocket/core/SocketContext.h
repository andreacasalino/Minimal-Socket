/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>

#include <atomic>
#include <mutex>

namespace MinimalSocket {
class RemoteAddressAware {
public:
  RemoteAddressAware(const RemoteAddressAware &o)
      : remote_address{o.remote_address} {}
  RemoteAddressAware &operator=(const RemoteAddressAware &o) {
    remote_address = o.remote_address;
    return *this;
  }

  /**
   * @return the address of the peer that can exchange messages with this
   * socket.
   */
  Address getRemoteAddress() const;

protected:
  /**
   * @throw in case the passed address is invalid (i.e. address == nullptr is
   * true).
   */
  RemoteAddressAware(const Address &address);

private:
  mutable std::mutex remote_address_mtx;
  Address remote_address;
};

class PortToBindAware {
public:
  PortToBindAware(const PortToBindAware &o)
      : port_to_bind{o.port_to_bind.load()}, must_be_free_port{
                                                 o.must_be_free_port.load()} {}
  PortToBindAware &operator=(const PortToBindAware &o) {
    port_to_bind = o.port_to_bind.load();
    must_be_free_port = o.must_be_free_port.load();
    return *this;
  }

  /**
   * @return the port that will be reserved, in case the socket was not already
   * opened, or the port actually reserved when the socket was opened.
   */
  Port getPortToBind() const { return port_to_bind; }

  /**
   * @brief Used to enforce the fact that this port should be not previously
   * binded by anyone else when opening the socket. Beware that the default
   * behaviour is the opposite: you don't call this function the port will be
   * possibly re-used.
   */
  void mustBeFreePort() { must_be_free_port = true; };
  bool shallBeFreePort() const { return must_be_free_port; }

protected:
  PortToBindAware(Port port) : port_to_bind(port){};

  void setPort(Port port) { port_to_bind = port; };

private:
  std::atomic<Port> port_to_bind;
  std::atomic_bool must_be_free_port = false;
};

class RemoteAddressFamilyAware {
public:
  RemoteAddressFamilyAware(const RemoteAddressFamilyAware &o)
      : remote_address_family{o.remote_address_family.load()} {}
  RemoteAddressFamilyAware &operator=(const RemoteAddressFamilyAware &o) {
    remote_address_family = o.remote_address_family.load();
    return *this;
  }

  /**
   * @return the address family of the peer that can exchange messages with this
   * socket.
   */
  AddressFamily getRemoteAddressFamily() const { return remote_address_family; }

protected:
  RemoteAddressFamilyAware(AddressFamily family)
      : remote_address_family(family){};

private:
  std::atomic<AddressFamily> remote_address_family;
};
} // namespace MinimalSocket
