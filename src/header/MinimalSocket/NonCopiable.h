/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

namespace MinimalSocket {
class NonCopiable {
public:
  NonCopiable(const NonCopiable &) = delete;
  NonCopiable &operator=(const NonCopiable &) = delete;

protected:
  NonCopiable() = default;
};
} // namespace MinimalSocket
