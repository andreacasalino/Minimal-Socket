/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include "SocketHandler.h"

namespace MinimalSocket {
// return port actually binded (as you could pass to the function also AnyPort)
Port bind(SocketID socket_id, AddressFamily family, Port port,
          bool must_be_free_port);

void listen(SocketID socket_id, std::size_t backlog_size);

void connect(SocketID socket_id, const Address &remote_address);
} // namespace MinimalSocket
